#ifndef INC_RENDERER_H_
#define INC_RENDERER_H_
#endif /* INC_RENDERER_H_ */

#include "stm32f429i_discovery_lcd.h"
#include "math.h"
#include "vector_math.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	Vec3f a,b,c;
	uint32_t col;
	float avg_z;
} Triangle;

typedef struct {
	Vec3f dir;
	uint32_t col;
	float a;
} LightParams;

typedef enum {
	RENDER_MODE_WIREFRAME,
	RENDER_MODE_FILLED
} RENDER_MODE;

typedef enum {
	LIGHT_0,
	LIGHT_1,
	LIGHT_2,
	LIGHT_3,
	LIGHT_4,
	LIGHT_5,
	LIGHT_6,
	LIGHT_7
} LIGHT;

typedef enum {
	_128 = 0b10000000,
	_64  = 0b01000000,
	_32  = 0b00100000,
	RENDER_FLAG_LIGHTING  = 0b00010000,
	_8   = 0b00001000,
	_4   = 0b00000100,
	_2   = 0b00000010,
	RENDER_FLAG_CULLING   = 0b00000001
} RENDER_FLAG;

void RenderBegin(void);

void EnableLighting(float ambient);
void DisableLighting(void);

void EnableLight(LIGHT index);
void DisableLight(LIGHT index);

void SetLight(LIGHT index, LightParams params);
LightParams GetLight(LIGHT index);

void SetCameraPos(Vec3f newpos);
Vec3f GetCameraPos(void);

void SetRenderMode(RENDER_MODE mode);

void RenderEnable(RENDER_FLAG flags);
void RenderDisable(RENDER_FLAG flags);
uint8_t RenderIsEnabled(RENDER_FLAG flag);

void PushTriangle(Triangle tri);

void RenderEnd(void);
