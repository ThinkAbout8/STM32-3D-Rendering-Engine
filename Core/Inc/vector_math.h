#ifndef INC_VECTOR_MATH_H_
#define INC_VECTOR_MATH_H_

typedef struct {
    float X, Y, Z;
} Vec3f;

#define Vec3f_ZERO (Vec3f){0.0f,0.0f,0.0f}

inline static Vec3f Vec3add(Vec3f a, Vec3f b)
{
	return (Vec3f){a.X+b.X, a.Y+b.Y, a.Z+b.Z};
}

inline static Vec3f Vec3sub(Vec3f a, Vec3f b)
{
	return (Vec3f){a.X-b.X, a.Y-b.Y, a.Z-b.Z};
}

inline static Vec3f Vec3mul(Vec3f a, Vec3f b)
{
	return (Vec3f){a.X*b.X, a.Y*b.Y, a.Z*b.Z};
}

inline static float Vec3dot(Vec3f a, Vec3f b)
{
	return a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}

static inline Vec3f Vec3cross(Vec3f a, Vec3f b) {
    return (Vec3f){
        a.Y*b.Z - a.Z*b.Y,
        a.Z*b.X - a.X*b.Z,
        a.X*b.Y - a.Y*b.X
    };
}

static inline Vec3f Vec3scale(Vec3f a, float b)
{
	return (Vec3f){a.X*b, a.Y*b, a.Z*b};
}

static inline double Vec3len(Vec3f a) {
    return sqrtf(a.X*a.X + a.Y*a.Y + a.Z*a.Z);
}

static inline Vec3f Vec3norm(Vec3f a) {
    float len = Vec3len(a);
    return len > 0.0 ? Vec3scale(a, 1.0f / len) : a;
}

void RotateX(Vec3f* vec, Vec3f o, float a);
void RotateY(Vec3f* vec, Vec3f o, float a);
void RotateZ(Vec3f* vec, Vec3f o, float a);
void Rotate(Vec3f* vec, Vec3f o, Vec3f a);

#endif /* INC_VECTOR_MATH_H_ */
