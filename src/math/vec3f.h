#pragma once

#define VEC3F_FMT "{%f, %f, %f}\n"
#define VEC3F_ARG(v) (v).x, (v).y, (v).z

union Vec3f
{
	struct {
		float x, y, z;
	};
	struct {
		float r, g, b;
	};
	struct {
		float s, t, u;
	};
	float f[3];
};

typedef union Vec3f vec3f_t;

VEC_FUNC void vec3f_addf(vec3f_t* o, vec3f_t* a, float b);
VEC_FUNC void vec3f_subf(vec3f_t* o, vec3f_t* a, float b);
VEC_FUNC void vec3f_mulf(vec3f_t* o, vec3f_t* a, float b);
VEC_FUNC void vec3f_divf(vec3f_t* o, vec3f_t* a, float b);

#define vec3f_add2f(o, a, b) vec2f_add((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec3f_sub2f(o, a, b) vec2f_sub((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec3f_mul2f(o, a, b) vec2f_mul((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec3f_div2f(o, a, b) vec2f_div((vec2f_t*)(o), (vec2f_t*)(a), b)

VEC_FUNC void vec3f_add(vec3f_t* o, vec3f_t* a, vec3f_t* b);
VEC_FUNC void vec3f_sub(vec3f_t* o, vec3f_t* a, vec3f_t* b);
VEC_FUNC void vec3f_mul(vec3f_t* o, vec3f_t* a, vec3f_t* b);
VEC_FUNC void vec3f_div(vec3f_t* o, vec3f_t* a, vec3f_t* b);
VEC_FUNC void vec3f_dot(float* o, vec3f_t* a, vec3f_t* b);
VEC_FUNC void vec3f_cross(vec3f_t* restrict o, vec3f_t* a, vec3f_t* b);

#ifdef VEC_EXT
	void vec3f_norm(vec3f_t* o, vec3f_t* a);
#endif

#if defined(VECTOR_INLINE) || defined(VECTOR_ALWAYS_INLINE)
	#include "vec3f.i"
#endif