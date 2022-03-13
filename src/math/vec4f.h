#pragma once

#include "config.h"

#define VEC4F_FMT "{%f, %f, %f, %f}\n"
#define VEC4F_ARG(v) (v).x, (v).y, (v).z, (v).w

union Vec4f
{
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
	struct {
		float s, t, u, v;
	};
	float f[4];
} __attribute__((aligned (16)));

typedef union Vec4f vec4f_t;

VEC_FUNC void vec4f_addf(vec4f_t* o, vec4f_t* a, float b);
VEC_FUNC void vec4f_subf(vec4f_t* o, vec4f_t* a, float b);
VEC_FUNC void vec4f_mulf(vec4f_t* o, vec4f_t* a, float b);
VEC_FUNC void vec4f_divf(vec4f_t* o, vec4f_t* a, float b);

#define vec4f_add2f(o, a, b) vec2f_add((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec4f_sub2f(o, a, b) vec2f_sub((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec4f_mul2f(o, a, b) vec2f_mul((vec2f_t*)(o), (vec2f_t*)(a), b)
#define vec4f_div2f(o, a, b) vec2f_div((vec2f_t*)(o), (vec2f_t*)(a), b)

#define vec4f_add3f(o, a, b) vec3f_add((vec3f_t*)(o), (vec3f_t*)(a), b)
#define vec4f_sub3f(o, a, b) vec3f_sub((vec3f_t*)(o), (vec3f_t*)(a), b)
#define vec4f_mul3f(o, a, b) vec3f_mul((vec3f_t*)(o), (vec3f_t*)(a), b)
#define vec4f_div3f(o, a, b) vec3f_div((vec3f_t*)(o), (vec3f_t*)(a), b)

VEC_FUNC void vec4f_add(vec4f_t* o, vec4f_t* a, vec4f_t* b);
VEC_FUNC void vec4f_sub(vec4f_t* o, vec4f_t* a, vec4f_t* b);
VEC_FUNC void vec4f_mul(vec4f_t* o, vec4f_t* a, vec4f_t* b);
VEC_FUNC void vec4f_div(vec4f_t* o, vec4f_t* a, vec4f_t* b);
VEC_FUNC void vec4f_dot(float* o, vec4f_t* a, vec4f_t* b);
VEC_FUNC void vec4f_cross(vec4f_t* restrict o, vec4f_t* a, vec4f_t* b);

#ifdef VEC_EXT
	void vec4f_norm(vec4f_t* o, vec4f_t* a);
#endif

#if defined(VECTOR_INLINE) || defined(VECTOR_ALWAYS_INLINE)
	#include "vec4f.i"
#endif