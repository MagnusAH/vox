#pragma once

#include <stdint.h>
#include "config.h"
#include "vec3f.h"

// macros for debugging, ex "printf(MAT3F_FMT, MAT3F_ARG(mat3f_t))"
#define MAT3F_FMT "{\n {%f, %f, %f},\n {%f, %f, %f},\n {%f, %f, %f}\n}\n"
#define MAT3F_ARG(m) (m).m00, (m).m01, (m).m02, (m).m10, (m).m11, (m).m12, (m).m20, (m).m21, (m).m22

union Mat3f
{
	struct {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};
	vec3f_t r[3];
	float f[9];
}
#ifdef MAT3_ALIGN
	__attribute__((aligned (16)));
#else
	;
#endif

typedef union Mat3f mat3f_t;

MAT_FUNC void mat3f_addf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_subf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_mulf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_divf(mat3f_t* o, mat3f_t* a, float b);

MAT_FUNC void mat3f_add(mat3f_t* o, mat3f_t* a, mat3f_t* b);
MAT_FUNC void mat3f_sub(mat3f_t* o, mat3f_t* a, mat3f_t* b);
MAT_FUNC void mat3f_mul(mat3f_t* o, mat3f_t* a, mat3f_t* b);
MAT_FUNC void mat3f_div(mat3f_t* o, mat3f_t* a, mat3f_t* b);

MAT_FUNC void mat3f_iden(mat3f_t* o);
MAT_FUNC void mat3f_inv(mat3f_t* o, mat3f_t* a);
MAT_FUNC void mat3f_trans(mat3f_t* o, mat3f_t* a);

MAT_FUNC void mat3f_mulv3f(vec3f_t* o, mat3f_t* a, vec3f_t* b);

#if defined(MATRIX_INLINE) || defined(MATRIX_ALWAYS_INLINE)
	#include"mat3f.i"
#endif