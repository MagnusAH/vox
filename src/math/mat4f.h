#pragma once

#include <stdint.h>
#include "config.h"
#include "vec4f.h"

// macros for debugging, ex "printf(MAT4F_FMT, MAT4F_ARG(mat4f_t))"
#define MAT4F_FMT "{\n {%f, %f, %f, %f},\n {%f, %f, %f, %f},\n {%f, %f, %f, %f},\n {%f, %f, %f, %f}\n}\n"
#define MAT4F_ARG(m) (m).m00, (m).m01, (m).m02, (m).m03, (m).m10, (m).m11, (m).m12, (m).m13, (m).m20, (m).m21, (m).m22, (m).m23, (m).m30, (m).m31, (m).m32, (m).m33

union Mat4f
{
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
	vec4f_t r[4];
	float f[16];
} __attribute__((aligned (16)));

typedef union Mat4f mat4f_t;

MAT_FUNC void mat4f_addf(mat4f_t* o, mat4f_t* a, float b);
MAT_FUNC void mat4f_subf(mat4f_t* o, mat4f_t* a, float b);
MAT_FUNC void mat4f_mulf(mat4f_t* o, mat4f_t* a, float b);
MAT_FUNC void mat4f_divf(mat4f_t* o, mat4f_t* a, float b);

MAT_FUNC void mat4f_add(mat4f_t* o, mat4f_t* a, mat4f_t* b);
MAT_FUNC void mat4f_sub(mat4f_t* o, mat4f_t* a, mat4f_t* b);
MAT_FUNC void mat4f_mul(mat4f_t* o, mat4f_t* a, mat4f_t* b);
MAT_FUNC void mat4f_div(mat4f_t* o, mat4f_t* a, mat4f_t* b);

MAT_FUNC void mat4f_iden(mat4f_t* o);
MAT_FUNC void mat4f_inv(mat4f_t* o, mat4f_t* a);
MAT_FUNC void mat4f_trans(mat4f_t* o, mat4f_t* a);

MAT_FUNC void mat4f_mulv4f(vec4f_t* o, mat4f_t* a, vec4f_t* b); // column vector => matrix * vector

#if defined(MATRIX_INLINE) || defined(MATRIX_ALWAYS_INLINE)
	#include "mat4f.i"
#endif