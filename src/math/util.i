#include <math.h>

#define INLINE inline __attribute__((__always_inline__))

INLINE void mat4f_setProjection(mat4f_t* o, float aspectRatio, float fov, float near, float far)
{
	float scale = atan(fov * 0.5f);
	o->m00 = 1.0f / (scale * aspectRatio);
	o->m11 = 1.0f / (scale);
	o->m22 = -(far + near) / (far - near);
	o->m23 = -1.0f;
	o->m32 = -(2.0f * far * near) / (far - near);
	o->m33 = 0;
}

INLINE void mat4f_setTransform3f(mat4f_t* o, float x, float y, float z)
{
	o->m03 = x;
	o->m13 = y;
	o->m23 = z;
}

INLINE void mat4f_setTransform3v(mat4f_t* o, vec3f_t* a)
{
	o->m03 = a->x;
	o->m13 = a->y;
	o->m23 = a->z;
}

INLINE void mat4f_setScalef(mat4f_t* o, float a)
{
	o->m00 = a;
	o->m11 = a;
	o->m22 = a;
}

INLINE void mat4f_setScale3f(mat4f_t* o, float x, float y, float z)
{
	o->m00 = x;
	o->m11 = y;
	o->m22 = z;
}

INLINE void mat4f_setScale3v(mat4f_t* o, vec3f_t* a)
{
	o->m00 = a->x;
	o->m11 = a->y;
	o->m22 = a->z;
}

#undef INLINE