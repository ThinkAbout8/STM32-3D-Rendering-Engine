#include "primitives.h"

//Vector3 size: 3 floats -> 12B
//Triangle size: 3*Vec3, uint32, float -> 3*12B + 4B + 4B -> 44B

void CreatePlane(Object* obj, float width, float length, uint32_t color)
{
	//Triangles: 2
	//Size -> 2*44B -> 88B
	float w = width / 2;
	float l = length / 2;

	Vec3f xz = (Vec3f){ -w, 0, l };
	Vec3f Xz = (Vec3f){  w, 0, l };
	Vec3f xZ = (Vec3f){ -w, 0,-l };
	Vec3f XZ = (Vec3f){  w, 0,-l };

	obj->_tcount = 2;
	obj->_tris = malloc(sizeof(Triangle)*2);

	obj->_tris[0] = (Triangle){.a = xz, .b = Xz, .c = xZ, .col = color};
	obj->_tris[1] = (Triangle){.a = Xz, .b = XZ, .c = xZ, .col = color};
}

void CreateCircle(Object* obj, float radius, int corners, uint32_t color)
{
	//Triangles: corners
	//Size -> corners*44B
	if (corners < 3) corners = 3;

	Vec3f verts[corners];

	for (int i=0; i<corners; i++)
	{
		verts[i] = (Vec3f){radius, 0, 0};
		RotateY(&(verts[i]), Vec3f_ZERO, i*(M_PI*2/corners));
	}

	obj->_tcount = corners;
	obj->_tris = malloc(sizeof(Triangle)*corners);

	for (int i=0; i<corners-1; i++)
	{
		obj->_tris[i] = (Triangle){.a = verts[i], .b = verts[i+1], .c = Vec3f_ZERO, .col = color};
	}
	obj->_tris[corners-1] = (Triangle){.a = verts[corners-1], .b = verts[0], .c = Vec3f_ZERO, .col = color};
}

void CreateRing(Object* obj, float outer_rad, float inner_rad, int corners, uint32_t color)
{
	//Triangles: corners*2
	//Size -> corners*2*44B -> corners*88B
	if (corners < 3) corners = 3;

	Vec3f iverts[corners];
	Vec3f overts[corners];

	for (int i=0; i<corners; i++)
	{
		iverts[i] = (Vec3f){inner_rad, 0, 0};
		RotateY(&(iverts[i]), Vec3f_ZERO, i*(M_PI*2/corners));

		overts[i] = (Vec3f){outer_rad, 0, 0};
		RotateY(&(overts[i]), Vec3f_ZERO, i*(M_PI*2/corners));
	}

	obj->_tcount = corners*2;
	obj->_tris = malloc(sizeof(Triangle)*corners*2);

	for(int i=0; i<corners-1; i++)
	{
		obj->_tris[i*2]   = (Triangle){.a = overts[i], .b = overts[i+1], .c = iverts[i], .col = color};
		obj->_tris[i*2+1] = (Triangle){.a = iverts[i], .b = overts[i+1], .c = iverts[i+1], .col = color};
	}
	obj->_tris[2*corners-2] = (Triangle){.a = overts[corners-1], .b = overts[0], .c = iverts[corners-1], .col = color};
	obj->_tris[2*corners-1] = (Triangle){.a = iverts[corners-1], .b = overts[0], .c = iverts[0], .col = color};
}

void CreateCube(Object* obj, float width, float length, float height, uint32_t color)
{
	//Triangles: 12
	//Size -> 12*44B -> 528B
	float w = width / 2;
	float l = length / 2;
	float h = height / 2;

	Vec3f xyz = (Vec3f){ -w , -h ,  l };
	Vec3f Xyz = (Vec3f){  w , -h ,  l };
	Vec3f xYz = (Vec3f){ -w ,  h ,  l };
	Vec3f XYz = (Vec3f){  w ,  h ,  l };
	Vec3f xyZ = (Vec3f){ -w , -h , -l };
	Vec3f XyZ = (Vec3f){  w , -h , -l };
	Vec3f xYZ = (Vec3f){ -w ,  h , -l };
	Vec3f XYZ = (Vec3f){  w ,  h , -l };

	obj->_tcount = 12;
	obj->_tris = malloc(sizeof(Triangle)*12);

	obj->_tris[0]  = (Triangle){.a = xyZ, .b = xYZ, .c = XyZ, .col = color};
	obj->_tris[1]  = (Triangle){.a = XYZ, .b = XyZ, .c = xYZ, .col = color};
	obj->_tris[2]  = (Triangle){.a = Xyz, .b = XYz, .c = xyz, .col = color};
	obj->_tris[3]  = (Triangle){.a = xYz, .b = xyz, .c = XYz, .col = color};
	obj->_tris[4]  = (Triangle){.a = Xyz, .b = xyz, .c = XyZ, .col = color};
	obj->_tris[5]  = (Triangle){.a = xyZ, .b = XyZ, .c = xyz, .col = color};
	obj->_tris[6]  = (Triangle){.a = XYZ, .b = xYZ, .c = XYz, .col = color};
	obj->_tris[7]  = (Triangle){.a = xYz, .b = XYz, .c = xYZ, .col = color};
	obj->_tris[8]  = (Triangle){.a = xyZ, .b = xyz, .c = xYZ, .col = color};
	obj->_tris[9]  = (Triangle){.a = xYz, .b = xYZ, .c = xyz, .col = color};
	obj->_tris[10] = (Triangle){.a = Xyz, .b = XyZ, .c = XYz, .col = color};
	obj->_tris[11] = (Triangle){.a = XYZ, .b = XYz, .c = XyZ, .col = color};
}

void CreateCyllinder(Object* obj, float radius, float height, int corners, uint32_t color)
{
	//Triangles: corners*4
	//Size -> corners*4*44B -> corners*176B
	if (corners < 3) corners = 3;
	float h = height / 2;

	Vec3f u = (Vec3f){0,h,0};
	Vec3f d = (Vec3f){0,-h,0};

	obj->_tcount = corners*4;
	obj->_tris = malloc(sizeof(Triangle)*corners*4);

	for (int i=0; i<corners; i++)
	{
		//Cyllinder slice cutout: uaA abA bBA bdB
		// * u *
		// a * A
		// b * B
		// * d *

		Vec3f a = {radius, h, 0};
		Vec3f A = {radius, h, 0};
		Vec3f b = {radius, -h, 0};
		Vec3f B = {radius, -h, 0};

		RotateY(&a, u, i*(M_PI*2/corners));
		RotateY(&A, u, (i+1)*(M_PI*2/corners));
		RotateY(&b, d, i*(M_PI*2/corners));
		RotateY(&B, d, (i+1)*(M_PI*2/corners));

		obj->_tris[4*i]   = (Triangle){.a = u, .b = a, .c = A, .col = color};
		obj->_tris[4*i+1] = (Triangle){.a = a, .b = b, .c = A, .col = color};
		obj->_tris[4*i+2] = (Triangle){.a = b, .b = B, .c = A, .col = color};
		obj->_tris[4*i+3] = (Triangle){.a = b, .b = d, .c = B, .col = color};
	}
}

void CreateSphere(Object* obj, float radius, int h_subdivs, int v_subdivs, uint32_t color)
{
	if (h_subdivs < 3) h_subdivs = 3;
	if (v_subdivs < 2) v_subdivs = 2;

	int max_tris = h_subdivs * (v_subdivs - 1) * 2;
	obj->_tris = malloc(sizeof(Triangle) * max_tris);

	float dtheta = (2.0f * M_PI) / h_subdivs;
	float dphi   = M_PI / v_subdivs;

	int t = 0;

	for (int y = 0; y < v_subdivs; y++)
	{
		float phi0 = y * dphi;
		float phi1 = (y + 1) * dphi;

		for (int x = 0; x < h_subdivs; x++)
		{
			float th0 = x * dtheta;
			float th1 = (x + 1) * dtheta;

			Vec3f a = {
				radius * sinf(phi0) * cosf(th0),
				radius * cosf(phi0),
				radius * sinf(phi0) * sinf(th0)
			};

			Vec3f A = {
				radius * sinf(phi0) * cosf(th1),
				radius * cosf(phi0),
				radius * sinf(phi0) * sinf(th1)
			};

			Vec3f b = {
				radius * sinf(phi1) * cosf(th0),
				radius * cosf(phi1),
				radius * sinf(phi1) * sinf(th0)
			};

			Vec3f B = {
				radius * sinf(phi1) * cosf(th1),
				radius * cosf(phi1),
				radius * sinf(phi1) * sinf(th1)
			};

			if (y != 0)
				obj->_tris[t++] = (Triangle){.a = a, .b = A, .c = b, .col = color};

			if (y != v_subdivs - 1)
				obj->_tris[t++] = (Triangle){.a = b, .b = A, .c = B, .col = color};
		}
	}

	obj->_tcount = t;
}

void CreateFrame(Frame* frm, uint8_t width, uint8_t height, uint32_t bcolor, uint32_t fcolor, sFONT* font, char* label)
{
	frm->Width = width;
	frm->Height = height;
	frm->BackColor = bcolor;
	frm->TextColor = fcolor;
	frm->Font = font;
	for (int i=0; i<4; i++)
		frm->Label[i] = label[i];
	frm->Label[4] = 0;
}

void DrawFrame(Frame* frm, Vec3f position)
{
	int16_t x, y;
	uint16_t w = frm->Width/2;
	uint16_t h = frm->Height/2;

	ProjectFrustum(position, &x, &y);

	uint16_t fh = frm->Font->Height;
	uint16_t fw = frm->Font->Width;

	w = (frm->Width < fw*4) ? fw*2 : w;
	h = w;

	BSP_LCD_SetFont(frm->Font);

	BSP_LCD_SetTextColor(frm->BackColor);
	BSP_LCD_DrawRect(x-w, y-h, w*2, h*2);
	BSP_LCD_FillRect(x-w, y-h-fh-1, w*2+1, fh+1);

	BSP_LCD_SetBackColor(frm->BackColor);
	BSP_LCD_SetTextColor(frm->TextColor);
	BSP_LCD_DisplayStringAt(x-w, y-h-fh, (uint8_t*)(frm->Label), LEFT_MODE);
}

void PushPrimitive(Object* obj, Vec3f parent_pos, Vec3f parent_rot)
{
	Vec3f world_rot = Vec3add(parent_rot, obj->Rotation);
	Vec3f world_pos = obj->Position;

	Rotate(&world_pos, parent_pos, parent_rot);
	world_pos = Vec3add(world_pos, parent_pos);

	obj->GlobalPosition = world_pos;

	for (int i = 0; i < obj->_tcount; i++)
	{
		Triangle tri = obj->_tris[i];

		Rotate(&tri.a, Vec3f_ZERO, world_rot);
		Rotate(&tri.b, Vec3f_ZERO, world_rot);
		Rotate(&tri.c, Vec3f_ZERO, world_rot);

		tri.a = Vec3add(tri.a, world_pos);
		tri.b = Vec3add(tri.b, world_pos);
		tri.c = Vec3add(tri.c, world_pos);

		PushTriangle(tri);
	}

	for (int i = 0; i < obj->_ccount; i++)
		PushPrimitive(obj->Children[i], world_pos, world_rot);
}

void AddChild(Object* parent, Object* child)
{
	if (!parent || !child) return;
	if (child->Parent) return;
	if (child == parent) return;

	if (parent->_ccount < OBJECT_MAX_CHILDREN)
	{
		parent->Children[parent->_ccount++] = child;
		child->Parent = parent;
	}
}

