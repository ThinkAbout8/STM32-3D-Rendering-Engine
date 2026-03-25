#ifndef INC_PRIMITIVES_H_
#define INC_PRIMITIVES_H_

#include "renderer.h"

#define OBJECT_MAX_CHILDREN 32

typedef struct Object{
	Vec3f Position;
	Vec3f GlobalPosition;
	Vec3f Rotation;
	Vec3f Scale;
	Triangle* _tris;
	int _tcount;
	struct Object* Parent;
	struct Object* Children[OBJECT_MAX_CHILDREN];
	int _ccount;
} Object;

typedef struct {
	uint32_t BackColor;
	uint32_t TextColor;
	sFONT* Font;
	char Label[5];
	uint8_t Width;
	uint8_t Height;
} Frame;

void CreatePlane(Object* obj, float width, float length, uint32_t color);
void CreateCircle(Object* obj, float radius, int corners, uint32_t color);
void CreateRing(Object* obj, float outer_rad, float inner_rad, int corners, uint32_t color);
void CreateCube(Object* obj, float width, float length, float height, uint32_t color);
void CreateCyllinder(Object* obj, float radius, float height, int corners, uint32_t color);
void CreateCone(Object* obj, float radius, float height, int corners, uint32_t color);
void CreateSphere(Object* obj, float radius, int h_subdivs, int v_subdivs, uint32_t color);
void CreateIcoSphere(Object* obj, float radius, int subdivisions, uint32_t color);

void CreateFrame(Frame* frm, uint8_t width, uint8_t height, uint32_t bcolor, uint32_t fcolor, sFONT* font, char* label);

void DrawFrame(Frame* frm, Vec3f position);
void PushPrimitive(Object* obj, Vec3f parent_pos, Vec3f parent_rot);
void AddChild(Object* parent, Object* child);

#endif /* INC_PRIMITIVES_H_ */
