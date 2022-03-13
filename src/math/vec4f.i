#include <immintrin.h>

VEC_FUNC void vec4f_addf(vec4f_t* o, vec4f_t* a, float b)
{
	_mm_store_ps((float*)o, _mm_add_ps(_mm_load_ps((float*)a), _mm_broadcast_ss(&b)));
}

VEC_FUNC void vec4f_subf(vec4f_t* o, vec4f_t* a, float b)
{
	_mm_store_ps((float*)o, _mm_sub_ps(_mm_load_ps((float*)a), _mm_broadcast_ss(&b)));
}

VEC_FUNC void vec4f_mulf(vec4f_t* o, vec4f_t* a, float b)
{
	_mm_store_ps((float*)o, _mm_mul_ps(_mm_load_ps((float*)a), _mm_broadcast_ss(&b)));
}

VEC_FUNC void vec4f_divf(vec4f_t* o, vec4f_t* a, float b)
{
	_mm_store_ps((float*)o, _mm_div_ps(_mm_load_ps((float*)a), _mm_broadcast_ss(&b)));
}

VEC_FUNC void vec4f_add(vec4f_t* o, vec4f_t* a, vec4f_t* b)
{
	_mm_store_ps((float*)o, _mm_add_ps(_mm_load_ps((float*)a), _mm_load_ps((float*)b)));
}

VEC_FUNC void vec4f_sub(vec4f_t* o, vec4f_t* a, vec4f_t* b)
{
	_mm_store_ps((float*)o, _mm_sub_ps(_mm_load_ps((float*)a), _mm_load_ps((float*)b)));
}

VEC_FUNC void vec4f_mul(vec4f_t* o, vec4f_t* a, vec4f_t* b)
{
	_mm_store_ps((float*)o, _mm_mul_ps(_mm_load_ps((float*)a), _mm_load_ps((float*)b)));
}

VEC_FUNC void vec4f_div(vec4f_t* o, vec4f_t* a, vec4f_t* b)
{
	_mm_store_ps((float*)o, _mm_div_ps(_mm_load_ps((float*)a), _mm_load_ps((float*)b)));
}

VEC_FUNC void vec4f_dot(float* o, vec4f_t* a, vec4f_t* b)
{
	*o = a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

VEC_FUNC void vec4f_cross(vec4f_t* restrict o, vec4f_t* a, vec4f_t* b)
{
	o->x = a->y * b->z - a->z * b->y;
	o->y = a->z * b->w - a->w * b->z;
	o->z = a->w * b->x - a->x * b->w;
	o->w = a->x * b->y - a->y * b->x;
}

#ifdef VEC_EXT
#include <math.h>

VEC_FUNC void vec4f_norm(vec4f_t* o, vec4f_t* a)
{
	float scale = 1 / sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	o->x = a->x * scale;
	o->y = a->y * scale;
	o->z = a->z * scale;
	o->w = a->w * scale;
}

#endif