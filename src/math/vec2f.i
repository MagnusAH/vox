#include <immintrin.h>

VEC_FUNC void vec2f_addf(vec2f_t* o, vec2f_t* a, float b)
{
	o->x = a->x + b;
	o->y = a->y + b;
}

VEC_FUNC void vec2f_subf(vec2f_t* o, vec2f_t* a, float b)
{
	o->x = a->x - b;
	o->y = a->y - b;
}

VEC_FUNC void vec2f_mulf(vec2f_t* o, vec2f_t* a, float b)
{
	o->x = a->x * b;
	o->y = a->y * b;
}

VEC_FUNC void vec2f_divf(vec2f_t* o, vec2f_t* a, float b)
{
	o->x = a->x / b;
	o->y = a->y / b;
}

VEC_FUNC void vec2f_add(vec2f_t* o, vec2f_t* a, vec2f_t* b)
{
	o->x = a->x + b->x;
	o->y = a->y + b->y;
}

VEC_FUNC void vec2f_sub(vec2f_t* o, vec2f_t* a, vec2f_t* b)
{
	o->x = a->x - b->x;
	o->y = a->y - b->y;
}

VEC_FUNC void vec2f_mul(vec2f_t* o, vec2f_t* a, vec2f_t* b)
{
	o->x = a->x * b->x;
	o->y = a->y * b->y;
}

VEC_FUNC void vec2f_div(vec2f_t* o, vec2f_t* a, vec2f_t* b)
{
	o->x = a->x / b->x;
	o->y = a->y / b->y;
}

VEC_FUNC void vec2f_dot(float* o, vec2f_t* a, vec2f_t* b)
{
	*o = a->x * b->x + a->y * b->y;
}

VEC_FUNC void vec2f_cross(float* o, vec2f_t* a, vec2f_t* b)
{
	*o = a->x * b->y - a->y * b->x;
}

#ifdef VEC_EXT
#include <math.h>

VEC_FUNC void vec2f_norm(vec2f_t* o, vec2f_t* a)
{
	float scale = 1 / sqrt(a->x * a->x + a->y * a->y);
	o->x = a->x * scale;
	o->y = a->y * scale;
}

#endif