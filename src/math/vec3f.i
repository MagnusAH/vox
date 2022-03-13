#include <immintrin.h>

VEC_FUNC void vec3f_addf(vec3f_t* o, vec3f_t* a, float b)
{
	o->x = a->x + b;
	o->y = a->y + b;
	o->z = a->z + b;
}

VEC_FUNC void vec3f_subf(vec3f_t* o, vec3f_t* a, float b)
{
	o->x = a->x - b;
	o->y = a->y - b;
	o->z = a->z - b;
}

VEC_FUNC void vec3f_mulf(vec3f_t* o, vec3f_t* a, float b)
{
	o->x = a->x * b;
	o->y = a->y * b;
	o->z = a->z * b;
}

VEC_FUNC void vec3f_divf(vec3f_t* o, vec3f_t* a, float b)
{
	o->x = a->x / b;
	o->y = a->y / b;
	o->z = a->z / b;
}

VEC_FUNC void vec3f_add(vec3f_t* o, vec3f_t* a, vec3f_t* b)
{
	o->x = a->x + b->x;
	o->y = a->y + b->y;
	o->z = a->z + b->z;
}

VEC_FUNC void vec3f_sub(vec3f_t* o, vec3f_t* a, vec3f_t* b)
{
	o->x = a->x - b->x;
	o->y = a->y - b->y;
	o->z = a->z - b->z;
}

VEC_FUNC void vec3f_mul(vec3f_t* o, vec3f_t* a, vec3f_t* b)
{
	o->x = a->x * b->x;
	o->y = a->y * b->y;
	o->z = a->z * b->z;
}

VEC_FUNC void vec3f_div(vec3f_t* o, vec3f_t* a, vec3f_t* b)
{
	o->x = a->x / b->x;
	o->y = a->y / b->y;
	o->z = a->z / b->z;
}

VEC_FUNC void vec3f_dot(float* o, vec3f_t* a, vec3f_t* b)
{
	*o = a->x * b->x + a->y * b->y + a->z * b->z;
}

VEC_FUNC void vec3f_cross(vec3f_t* o, vec3f_t* a, vec3f_t* b)
{
	o->x = a->y * b->z - a->z * b->y;
	o->y = a->z * b->x - a->x * b->z;
	o->z = a->x * b->y - a->y * b->x;
}

#ifdef VEC_EXT
#include <math.h>

VEC_FUNC void vec3f_norm(vec3f_t* o, vec3f_t* a)
{
	float scale = 1 / sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	o->x = a->x * scale;
	o->y = a->y * scale;
	o->z = a->z * scale;
}

#endif