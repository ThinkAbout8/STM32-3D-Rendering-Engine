#include "stm32f429i_discovery_lcd.h"

#include "renderer.h"

RENDER_MODE r_m = RENDER_MODE_FILLED;

uint8_t render_flags = 0b00000001;

// <128> -
//  <64> -
//  <32> -
//  <16> - Lighting
//   <8> -
//   <4> -
//   <2> -
//   <1> - Culling


// Camera
float fov = 60.0f;
float aspect = 1;
float near_plane = 1.0f;
float far_plane  = 400.0f;
Vec3f camera = {0 ,0 ,200};
float scale = 100.0f;
uint16_t cent_x = 120;
uint16_t cent_y = 160;

//Light
#define LIGHT_COUNT 8

uint8_t light_flags = 0b00000000;
LightParams lights[LIGHT_COUNT];
uint8_t light_enabled = 0;
float ambient = 0.0f;

// Scene render queue
#define TRIS_CAP 512

static Triangle tris[TRIS_CAP];
static uint16_t tri_count = 0;

static inline void ComputeDepths()
{
	for (uint16_t i=0; i<tri_count; i++)
	{
		Triangle* tri = &(tris[i]);
		tri->avg_z = (tri->a.Z + tri->b.Z + tri->c.Z) / 3.0f;
	}
}

static inline Vec3f ARGB2Color(uint32_t base)
{
	Vec3f col;
	col.X = ((base >> 16) & 0xFF) / 255.0f;
	col.Y = ((base >> 8)  & 0xFF) / 255.0f;
	col.Z = ((base)       & 0xFF) / 255.0f;
	return col;
}

static inline uint32_t Color2ARGB(Vec3f base)
{
	uint8_t r = (uint8_t)(base.X * 255.0f);
	uint8_t g = (uint8_t)(base.Y * 255.0f);
	uint8_t b = (uint8_t)(base.Z * 255.0f);
	return (0xFF000000) | (r << 16) | (g << 8) | b;
}

static inline int clamp(int val, int _min, int _max)
{
	return (val > _max) ? _max : ((val < _min) ? _min : val );
}


uint32_t ComputeLighting(uint32_t color, Vec3f normal)
{
	Vec3f base = ARGB2Color(color);
	Vec3f N = Vec3norm(normal);

	Vec3f accum = Vec3scale(base, ambient);

	for (uint8_t i=0; i<LIGHT_COUNT; i++)
	{
		if (!(light_flags & (1 << i))) continue;

		Vec3f L = Vec3norm(lights[i].dir);

		float diffuse = Vec3dot(N, L);

		Vec3f lcol = ARGB2Color(lights[i].col);

		Vec3f contrib = Vec3scale(Vec3mul(base, lcol),diffuse*lights[i].a);

		accum = Vec3add(accum, contrib);

	}

	if (accum.X > 1.0f) accum.X = 1.0f;
	if (accum.Y > 1.0f) accum.Y = 1.0f;
	if (accum.Z > 1.0f) accum.Z = 1.0f;

	return Color2ARGB(accum);
}

int CompareZ(const void* a, const void* b)
{
    float az = (*(Triangle*)a).avg_z;
    float bz = (*(Triangle*)b).avg_z;
    if (az < bz) return -1;
    if (az > bz) return 1;
    return 0;
}

Vec3f GetNormal(Triangle tri)
{
	return Vec3cross(Vec3sub(tri.b,tri.a),Vec3sub(tri.c,tri.a));
}

int IsBackFace(Triangle tri)
{
    Vec3f view = Vec3sub(tri.a, camera);

    return Vec3dot(GetNormal(tri),view) >= 0.0f;
}

void SortTriangles()
{
	qsort(tris, tri_count, sizeof(Triangle), CompareZ);
}


void RenderBegin()
{
	tri_count = 0;
}

void EnableLighting(float _ambient)
{
	light_enabled = 1;
	ambient = _ambient;
}

void DisableLighting() { light_enabled = 0; }

void EnableLight(LIGHT index)
{
	if (index >= LIGHT_COUNT || index < 0) return;

	light_flags |= 1 << index;
}

void DisableLight(LIGHT index)
{
	if (index < 0 || index >= LIGHT_COUNT) return;

	light_flags &= ~(1 << index);
}

void SetLight(LIGHT index, LightParams params)
{
	if (index >= LIGHT_COUNT || index < 0) return;

	lights[index] = params;
}

LightParams GetLight(LIGHT index)
{
	if (index >= LIGHT_COUNT || index < 0) return lights[0];

	return lights[index];
}



void SetCameraPos(Vec3f newpos) { camera = newpos; }

Vec3f GetCameraPos() { return camera; }

void SetRenderMode(RENDER_MODE mode) { r_m = mode; }

void RenderEnable(RENDER_FLAG flags) { render_flags |= flags; }

void RenderDisable(RENDER_FLAG flags) {	render_flags &= ~flags; }

uint8_t RenderIsEnabled(RENDER_FLAG flag) { return render_flags & flag; };

void PushTriangle(Triangle tri)
{
	if (tri_count >= TRIS_CAP)
	        return;

	if (!IsBackFace(tri) || !RenderIsEnabled(RENDER_FLAG_CULLING))
		tris[tri_count++] = tri;
}

void ProjectFrustum(Vec3f world, int16_t *sx, int16_t *sy)
{
	float x = world.X - camera.X;
	float y = world.Y - camera.Y;
	float z = camera.Z - world.Z;;

    float f = 1.0f / tanf((fov * 0.5f) * M_PI / 180.0f);
    if (z <= near_plane) z = near_plane;

    float xp = (f / aspect) * (x / z);
    float yp = (f) * (y / z);

    *sx = (uint16_t)((xp * scale) + cent_x);
    *sy = (uint16_t)((-yp * scale) + cent_y);
}

void DrawTriangle(Triangle tri)
{
	uint32_t color = tri.col;

	if (light_enabled)
	{
		color = ComputeLighting(tri.col, GetNormal(tri));
	}

    BSP_LCD_SetTextColor(color);
    Point pp[3];


    ProjectFrustum(tri.a, &pp[0].X, &pp[0].Y);
    ProjectFrustum(tri.b, &pp[1].X, &pp[1].Y);
    ProjectFrustum(tri.c, &pp[2].X, &pp[2].Y);

    if (r_m == RENDER_MODE_WIREFRAME)
    {
    	BSP_LCD_DrawLine(pp[0].X, pp[0].Y, pp[1].X, pp[1].Y);
    	BSP_LCD_DrawLine(pp[1].X, pp[1].Y, pp[2].X, pp[2].Y);
    	BSP_LCD_DrawLine(pp[2].X, pp[2].Y, pp[0].X, pp[0].Y);
    }
    else if (r_m == RENDER_MODE_FILLED)
	{
		for (int i = 0; i < 3 - 1; i++)
			for (int j = i + 1; j < 3; j++)
				if (pp[j].Y < pp[i].Y) {
					Point tmp = pp[i];
					pp[i] = pp[j];
					pp[j] = tmp;
				}

		int x0 = pp[0].X, y0 = pp[0].Y;
		int x1 = pp[1].X, y1 = pp[1].Y;
		int x2 = pp[2].X, y2 = pp[2].Y;

		if (y0 == y2) return;

		float dxdy_long = (float)(x2 - x0) / (float)(y2 - y0);
		float dxdy_top  = (y1 == y0) ? 0 : (float)(x1 - x0) / (float)(y1 - y0);
		float dxdy_bot  = (y2 == y1) ? 0 : (float)(x2 - x1) / (float)(y2 - y1);

		float xl = (float)x0;
		float xr = (float)x0;

		for (int y = y0; y < y1; y++)
		{
			if (y < 0 || y >= BSP_LCD_GetYSize()) { xl += dxdy_long; xr += dxdy_top; continue; }

			int x_start = (int)(xl + 0.5f);
			int x_end   = (int)(xr + 0.5f);
			if (x_start > x_end) { int tmp = x_start; x_start = x_end; x_end = tmp; }

			x_start = clamp(x_start, 0, BSP_LCD_GetXSize()-1);
			x_end = clamp(x_end, 0, BSP_LCD_GetXSize()-1);

			BSP_LCD_DrawHLine(x_start, y, x_end - x_start + 1);

			xl += dxdy_long;
			xr += dxdy_top;
		}

		xl = (float)(x0 + (y1 - y0) * dxdy_long);
		xr = (float)x1;

		for (int y = y1; y <= y2; y++)
		{
			if (y < 0 || y >= BSP_LCD_GetYSize()) { xl += dxdy_long; xr += dxdy_bot; continue; }

			int x_start = (int)(xl + 0.5f);
			int x_end   = (int)(xr + 0.5f);
			if (x_start > x_end) { int tmp = x_start; x_start = x_end; x_end = tmp; }

			x_start = clamp(x_start, 0, BSP_LCD_GetXSize()-1);
			x_end = clamp(x_end, 0, BSP_LCD_GetXSize()-1);

			BSP_LCD_DrawHLine(x_start, y, x_end - x_start + 1);

			xl += dxdy_long;
			xr += dxdy_bot;
		}
	}
}

void RenderEnd()
{
	ComputeDepths();
	SortTriangles();

	for (int i=0; i < tri_count; i++)
	{
		DrawTriangle(tris[i]);
	}
}
