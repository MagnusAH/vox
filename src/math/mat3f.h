#pragma once

#include "config.h"
#include "vec3f.h"

union Mat3f
{
	struct {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};
	vec3f_t r[3];
	float f[9];
};

typedef union Mat3f mat3f_t;

MAT_FUNC void mat3f_addf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_subf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_mulf(mat3f_t* o, mat3f_t* a, float b);
MAT_FUNC void mat3f_divf(mat3f_t* o, mat3f_t* a, float b);

MAT_FUNC void mat3f_add(mat3f_t* o, mat3f_t* a, mat3f_t* b);
MAT_FUNC void mat3f_sub(mat3f_t* o, mat3f_t* a, mat3f_t* b);
MAT_FUNC void mat3f_mul(mat3f_t* o, mat3f_t* a, mat3f_t* b); // no impl
MAT_FUNC void mat3f_div(mat3f_t* o, mat3f_t* a, mat3f_t* b); // no impl

MAT_FUNC void mat3f_iden(mat3f_t* o);
MAT_FUNC void mat3f_inv(mat3f_t* o, mat3f_t* a); // no impl
MAT_FUNC void mat3f_trans(mat3f_t* o, mat3f_t* a); // no impl

MAT_FUNC void mat3f_mulv3f(vec3f_t* o, mat3f_t* a, vec3f_t* b); // no impl

#if defined(MATRIX_INLINE) || defined(MATRIX_ALWAYS_INLINE)
	#include"mat3f.i"
#endif