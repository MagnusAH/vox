VEC_FUNC void vec2i_addi(vec2i_t* o, vec2i_t* a, int32_t b)
{
	o->x = a->x + b;
	o->y = a->y + b;
}

VEC_FUNC void vec2i_subi(vec2i_t* o, vec2i_t* a, int32_t b)
{
	o->x = a->x - b;
	o->y = a->y - b;
}

VEC_FUNC void vec2i_muli(vec2i_t* o, vec2i_t* a, int32_t b)
{
	o->x = a->x * b;
	o->y = a->y * b;
}

VEC_FUNC void vec2i_divi(vec2i_t* o, vec2i_t* a, int32_t b)
{
	o->x = a->x / b;
	o->y = a->y / b;
}

VEC_FUNC void vec2i_add(vec2i_t* o, vec2i_t* a, vec2i_t* b)
{
	o->x = a->x + b->x;
	o->y = a->y + b->y;
}

VEC_FUNC void vec2i_sub(vec2i_t* o, vec2i_t* a, vec2i_t* b)
{
	o->x = a->x - b->x;
	o->y = a->y - b->y;
}

VEC_FUNC void vec2i_mul(vec2i_t* o, vec2i_t* a, vec2i_t* b)
{
	o->x = a->x * b->x;
	o->y = a->y * b->y;
}

VEC_FUNC void vec2i_div(vec2i_t* o, vec2i_t* a, vec2i_t* b)
{
	o->x = a->x / b->x;
	o->y = a->y / b->y;
}