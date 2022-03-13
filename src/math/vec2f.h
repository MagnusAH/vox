#pragma once

#define VEC2F_FMT "{%f, %f}\n"
#define VEC2F_ARG(v) (v).x, (v).y

union Vec2f
{
	struct {
		float x, y;
	};
	struct {
		float r, g;
	};
	struct {
		float s, t;
	};
	float f[2];
};

typedef union Vec2f vec2f_t;

VEC_FUNC void vec2f_addf(vec2f_t* o, vec2f_t* a, float b);
VEC_FUNC void vec2f_subf(vec2f_t* o, vec2f_t* a, float b);
VEC_FUNC void vec2f_mulf(vec2f_t* o, vec2f_t* a, float b);
VEC_FUNC void vec2f_divf(vec2f_t* o, vec2f_t* a, float b);

VEC_FUNC void vec2f_add(vec2f_t* o, vec2f_t* a, vec2f_t* b);
VEC_FUNC void vec2f_sub(vec2f_t* o, vec2f_t* a, vec2f_t* b);
VEC_FUNC void vec2f_mul(vec2f_t* o, vec2f_t* a, vec2f_t* b);
VEC_FUNC void vec2f_div(vec2f_t* o, vec2f_t* a, vec2f_t* b);
VEC_FUNC void vec2f_dot(float* o, vec2f_t* a, vec2f_t* b);
VEC_FUNC void vec2f_cross(float* o, vec2f_t* a, vec2f_t* b);

#ifdef VEC_EXT
	void vec2f_norm(vec2f_t* o, vec2f_t* a);
#endif

#if defined(VECTOR_INLINE) || defined(VECTOR_ALWAYS_INLINE)
	#include "vec2f.i"
#endif