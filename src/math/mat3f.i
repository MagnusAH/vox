// TODO :: vectorize scalar if possible if mat3f recieves heavy use (lol no)
#include <immintrin.h>

extern const float mat3f_identity[9];

MAT_FUNC void mat3f_addf(mat3f_t* o, mat3f_t* a, float b)
{
	_mm256_store_ps(&o->f[0], _mm256_add_ps(_mm256_load_ps(&a->f[0]), _mm256_broadcast_ss(&b)));
	o->f[8] = a->f[8] + b;
}

MAT_FUNC void mat3f_subf(mat3f_t* o, mat3f_t* a, float b)
{
	_mm256_store_ps(&o->f[0], _mm256_sub_ps(_mm256_load_ps(&a->f[0]), _mm256_broadcast_ss(&b)));
	o->f[8] = a->f[8] - b;
}

MAT_FUNC void mat3f_mulf(mat3f_t* o, mat3f_t* a, float b)
{
	_mm256_store_ps(&o->f[0], _mm256_mul_ps(_mm256_load_ps(&a->f[0]), _mm256_broadcast_ss(&b)));
	o->f[8] = a->f[8] * b;
}

MAT_FUNC void mat3f_divf(mat3f_t* o, mat3f_t* a, float b)
{
	_mm256_store_ps(&o->f[0], _mm256_div_ps(_mm256_load_ps(&a->f[0]), _mm256_broadcast_ss(&b)));
	o->f[8] = a->f[8] / b;
}

MAT_FUNC void mat3f_iden(mat3f_t* o)
{
	_mm256_store_ps(&o->f[0], _mm256_load_ps(&mat3f_identity[0]));
	o->f[8] = mat3f_identity[8];
}

MAT_FUNC void mat3f_inv(mat3f_t* o, mat3f_t* a)
{

}

MAT_FUNC void mat3f_trans(mat3f_t* o, mat3f_t* a)
{

}

MAT_FUNC void mat3f_add(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	_mm256_store_ps(&o->f[0], _mm256_add_ps(_mm256_load_ps(&a->f[0]), _mm256_load_ps(&b->f[0])));
	o->f[8] = a->f[8] + b->f[8];
}

MAT_FUNC void mat3f_sub(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	_mm256_store_ps(&o->f[0], _mm256_sub_ps(_mm256_load_ps(&a->f[0]), _mm256_load_ps(&b->f[0])));
	o->f[8] = a->f[8] - b->f[8];
}

MAT_FUNC void mat3f_mul(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{

}

MAT_FUNC void mat3f_div(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{

}

MAT_FUNC void mat3f_mulv3f(vec3f_t* o, mat3f_t* a, vec3f_t* b)
{

}