#pragma once

#include <stdint.h>
#include "config.h"

#define VEC2I_FMT "{%i, %i}\n"
#define VEC2I_ARG(v) (v).x, (v).y

union Vec2i
{
	struct {
		int32_t x, y;
	};
	struct {
		int32_t r, g;
	};
	struct {
		int32_t s, t;
	};
	int32_t i[2];
};

typedef union Vec2i vec2i_t;

VEC_FUNC void vec2i_addi(vec2i_t* o, vec2i_t* a, int32_t b);
VEC_FUNC void vec2i_subi(vec2i_t* o, vec2i_t* a, int32_t b);
VEC_FUNC void vec2i_muli(vec2i_t* o, vec2i_t* a, int32_t b);
VEC_FUNC void vec2i_divi(vec2i_t* o, vec2i_t* a, int32_t b);

VEC_FUNC void vec2i_add(vec2i_t* o, vec2i_t* a, vec2i_t* b);
VEC_FUNC void vec2i_sub(vec2i_t* o, vec2i_t* a, vec2i_t* b);
VEC_FUNC void vec2i_mul(vec2i_t* o, vec2i_t* a, vec2i_t* b);
VEC_FUNC void vec2i_div(vec2i_t* o, vec2i_t* a, vec2i_t* b);

#if defined(VECTOR_INLINE) || defined(VECTOR_ALWAYS_INLINE)
	#include "vec2i.i"
#endif