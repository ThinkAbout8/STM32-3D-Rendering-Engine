#include "math.h"
#include "vector_math.h"


void RotateX(Vec3f* vec, Vec3f o, float a)
{
	float _y = vec->Y - o.Y;
	float _z = vec->Z - o.Z;

	vec->Y = _y * cosf(a) - _z * sinf(a) + o.Y;
	vec->Z = _y * sinf(a) + _z * cosf(a) + o.Z;
}

void RotateY(Vec3f* vec, Vec3f o, float a)
{
	float _x = vec->X - o.X;
	float _z = vec->Z - o.Z;

	vec->X =  _x * cosf(a) + _z * sinf(a) + o.X;
	vec->Z = -_x * sinf(a) + _z * cosf(a) + o.Z;
}

void RotateZ(Vec3f* vec, Vec3f o, float a)
{
	float _x = vec->X - o.X;
	float _y = vec->Y - o.Y;

	vec->X = _x * cosf(a) - _y * sinf(a) + o.X;
	vec->Y = _x * sinf(a) + _y * cosf(a) + o.Y;
}

void Rotate(Vec3f* vec, Vec3f o, Vec3f a)
{
	RotateZ(vec, o, a.Z);
	RotateY(vec, o, a.Y);
	RotateX(vec, o, a.X);
}
