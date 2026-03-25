#include "primitives.h"
#include "scene_root.h"
#include "stm32f4xx_hal.h"

LightParams light1;
LightParams light2;

Object mrcr;

Object vnus;

Object trra;
Object trra_s;
Object luna;
Object luna_s;

Object mars;
Object mars_s;
Object phob;
Object phob_s;
Object deim;
Object deim_s;

Object strn;
Object strn_r1;
Object strn_r2;
Object strn_r3;
Object strn_r4;

Object jptr;
Object jptr_s;
Object io;
Object io_s;
Object euro;
Object euro_s;
Object gany;
Object gany_s;

Object uran;

Object nptn;

Object plha_bary;
Object plut;
Object plut_s;
Object harn;
Object harn_s;

Frame mrcr_f;
Frame vnus_f;
Frame trra_f;
Frame luna_f;
Frame mars_f;
Frame phob_f;
Frame deim_f;
Frame strn_f;
Frame jptr_f;
Frame io_f;
Frame euro_f;
Frame gany_f;
Frame uran_f;
Frame nptn_f;
Frame plut_f;
Frame harn_f;

uint8_t scene = 0;

char line1[50];
char line2[50];

void OnInit(void)
{
	sprintf(line2, "Scene: %d", scene);

	EnableLighting(0.2f);

	light1 = (LightParams){.dir = {0.5,0.5,0.5}, .col = LCD_COLOR_WHITE, .a = 0.9f};

	EnableLight(LIGHT_0);
	SetLight(LIGHT_0, light1);

	AddChild(&trra, &trra_s);
	AddChild(&trra, &luna);
	AddChild(&luna, &luna_s);

	AddChild(&mars, &mars_s);
	AddChild(&mars, &phob);
	AddChild(&phob, &phob_s);
	AddChild(&mars, &deim);
	AddChild(&deim, &deim_s);

	AddChild(&strn, &strn_r1);
	AddChild(&strn_r1, &strn_r3);
	AddChild(&strn, &strn_r2);
	AddChild(&strn_r2, &strn_r4);

	AddChild(&jptr, &jptr_s);
	AddChild(&jptr, &io);
	AddChild(&io, &io_s);
	AddChild(&jptr, &euro);
	AddChild(&euro, &euro_s);
	AddChild(&jptr, &gany);
	AddChild(&gany, &gany_s);

	AddChild(&plha_bary, &plut);
	AddChild(&plha_bary, &harn);
	AddChild(&plut, &plut_s);
	AddChild(&harn, &harn_s);


	CreateSphere(&mrcr, 10, 12,8, 0xFF8B8B83);
	CreateSphere(&vnus, 24, 12,8, 0xFFFFE5B4);
	CreateSphere(&trra_s, 32, 12,8, 0xFF3399FF);
	CreateSphere(&luna_s, 8, 8,6, 0xFFCCCCCC);
	CreateSphere(&mars_s, 30, 12,8, 0xFFFF6633);
	CreateCube(&phob_s, 3,3,3, 0xFF999999);
	CreateCube(&deim_s, 1,1,1, 0xFFAAAAAA);
	CreateSphere(&strn, 48, 12,8, 0xFFD9B599);
	CreateRing(&strn_r1, 98, 70, 12, 0xFFD0A888);
	CreateRing(&strn_r2, 110, 104, 12, 0xFF966F4F);
	CreateRing(&strn_r3, 98, 70, 12, 0xFFD0A888);
	CreateRing(&strn_r4, 110, 104, 12, 0xFF966F4F);
	CreateSphere(&jptr_s, 52, 12,8, 0xFFD9B37A);
	CreateCube(&io_s, 1,1,1, 0xFFFFCC99);
	CreateCube(&euro_s, 1,1,1, 0xFFD9FFFF);
	CreateCube(&gany_s, 1,1,1, 0xFFCCCCCC);
	CreateSphere(&uran, 40, 12,8, 0xFF7FD9FF);
	CreateSphere(&nptn, 38, 12,8, 0xFFB0B0B0);
	CreateSphere(&plut_s, 24, 12,8, 0xFFC8B4A0);
	CreateSphere(&harn_s, 12, 12,8, 0xFF9E9E9E);


	CreateFrame(&mrcr_f, 20, 20, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "MRCR");
	CreateFrame(&vnus_f, 50, 50, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "VNUS");
	CreateFrame(&trra_f, 70, 70, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "TRRA");
	CreateFrame(&luna_f, 40, 40, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "LUNA");
	CreateFrame(&mars_f, 60, 60, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "MARS");
	CreateFrame(&phob_f, 10, 10, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "PHOB");
	CreateFrame(&deim_f, 10, 10, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "DEIM");
	CreateFrame(&strn_f, 100, 100, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "STRN");
	CreateFrame(&jptr_f, 100, 100, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "JPTR");
	CreateFrame(&io_f, 10, 10, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "IO  ");
	CreateFrame(&euro_f, 10, 10, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "EURO");
	CreateFrame(&gany_f, 10, 10, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font8, "GANY");
	CreateFrame(&uran_f, 100, 100, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "URAN");
	CreateFrame(&nptn_f, 100, 100, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "NPTN");
	CreateFrame(&plut_f, 50, 50, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "PLUT");
	CreateFrame(&harn_f, 30, 30, LCD_COLOR_GREEN, LCD_COLOR_BLACK, &Font12, "HARN");

	luna_s.Position.X = 100;
	luna.Rotation.X = 0.127*M_PI;

	phob.Rotation.Y = M_PI*1.7f;
	phob_s.Position.X = 60;
	deim_s.Position.X = 95;
	deim.Rotation.X = M_PI*0.7f;

	strn_r3.Rotation.X = M_PI;
	strn_r4.Rotation.X = M_PI;

	io_s.Position.X = 70;
	euro_s.Position.X = 85;
	gany_s.Position.X = 103;

	plut_s.Position.X = -20;
	harn_s.Position.X = 40;
}

void OnButtonPress()
{
	scene = (scene < 12) ? scene+1 : 0;
	sprintf(line2, "Scene: %d", scene);
}

void OnJoystickInput(uint32_t* _buffer, float _delta)
{
	uint32_t _x = _buffer[1];
	uint32_t _y = _buffer[0];

	float dx = (float)((int)_x - 2048) / 2048.0f;
	float dy = (float)((int)_y - 2048) / 2048.0f;

	sprintf(line1, "Joystick X: %.2f Y: %.2f", dx, dy);

	float vel = M_PI * 2;
	Vec3f rot = {-dy*vel*_delta, -dx*vel*_delta, 0};

	mrcr.Rotation = Vec3add(mrcr.Rotation, rot);
	vnus.Rotation = Vec3add(vnus.Rotation, rot);
	trra.Rotation = Vec3add(trra.Rotation, rot);
	mars.Rotation = Vec3add(mars.Rotation, rot);
	strn.Rotation = Vec3add(strn.Rotation, rot);
	jptr.Rotation = Vec3add(jptr.Rotation, rot);
	uran.Rotation = Vec3add(uran.Rotation, rot);
	nptn.Rotation = Vec3add(nptn.Rotation, rot);
	plha_bary.Rotation = Vec3add(plha_bary.Rotation, rot);
}

void OnRender(void)
{
	switch(scene)
	{
	case 0:
		RenderBegin();
		PushPrimitive(&mrcr, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&mrcr_f, mrcr.Position);
		break;
	case 1:
		RenderBegin();
		PushPrimitive(&vnus, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&vnus_f, vnus.Position);
		break;
	case 2:
		RenderBegin();
		PushPrimitive(&trra, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&luna_f, luna_s.GlobalPosition);
		DrawFrame(&trra_f, trra.Position);
		break;
	case 3:
		RenderBegin();
		PushPrimitive(&mars, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&phob_f, phob_s.GlobalPosition);
		DrawFrame(&deim_f, deim_s.GlobalPosition);
		DrawFrame(&mars_f, mars.Position);
		break;
	case 5:
		RenderBegin();
		PushPrimitive(&strn, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&strn_f, strn.Position);
		break;
	case 4:
		RenderBegin();
		PushPrimitive(&jptr, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&io_f, io_s.GlobalPosition);
		DrawFrame(&euro_f, euro_s.GlobalPosition);
		DrawFrame(&gany_f, gany_s.GlobalPosition);
		DrawFrame(&jptr_f, jptr.Position);
		break;
	case 6:
		RenderBegin();
		PushPrimitive(&uran, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&uran_f, uran.Position);
		break;
	case 7:
		RenderBegin();
		PushPrimitive(&nptn, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&nptn_f, nptn.Position);
		break;
	case 8:
		RenderBegin();
		PushPrimitive(&plha_bary, Vec3f_ZERO, Vec3f_ZERO);
		RenderEnd();

		DrawFrame(&harn_f, harn_s.GlobalPosition);
		DrawFrame(&plut_f, plut_s.GlobalPosition);
		break;
	default:
		break;
	}


	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)line1);
	BSP_LCD_DisplayStringAtLine(2, (uint8_t*)line2);
}

void OnTick(float _delta)
{
	mrcr.Rotation.Y -= 0.02f*M_PI*_delta;
	vnus.Rotation.Y += 0.005f*M_PI*_delta;

	trra_s.Rotation.Y -= 0.04f*M_PI*_delta;
	luna.Rotation.Y -= 0.25f*M_PI*_delta;
	luna_s.Rotation.Y += 0.08f*M_PI*_delta;

	mars_s.Rotation.Y -= 0.03f*M_PI*_delta;
	phob_s.Rotation.Y += 0.06f*M_PI * _delta;
	deim_s.Rotation.Y -= 0.03f*M_PI * _delta;
	phob.Rotation.Y -= 0.2f*M_PI * _delta;
	deim.Rotation.Y -= 0.1f*M_PI * _delta;

	strn.Rotation.Y -= 0.2f*M_PI*_delta;

	jptr_s.Rotation.Y -= 0.05f*M_PI*_delta;
	io.Rotation.Y -= 0.3f*M_PI*_delta;
	euro.Rotation.Y -= 0.2f*M_PI*_delta;
	gany.Rotation.Y -= 0.15f*M_PI*_delta;

	uran.Rotation.Y += 0.02f*M_PI*_delta;

	nptn.Rotation.Y -= 0.03f*M_PI*_delta;

	plha_bary.Rotation.Y -= 0.2*M_PI*_delta;
}
