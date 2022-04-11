#include "dynamic_array.h"

#include <stdlib.h> // need custom replacement for malloc, malloc dies with anything over 16k on windows

#include "mem.h"

// returns 1 when input is 0 or over 2_147_483_648, otherwise ok
inline uint32_t ceil_pow2(uint32_t n)
{
	return (uint32_t)1 << (32 - __builtin_clz(n - 1));
}

// inline for resizeing to ceil rounded power of two of size
inline void _resize(dynamic_array_t* array, uint32_t size)
{
	array->size = ceil_pow2(size);
	array->ptr = realloc(array->ptr, array->size);
}

/*
	Array management
*/

// init dynamic array with size 'size' (in bytes)
void dynar_init(dynamic_array_t* array, uint32_t size)
{
	array->size = size ? size : 64;
	array->length = 0;
	array->ptr = malloc(array->size);
}

// destroy dynamic array (frees memory)
void dynar_destroy(dynamic_array_t* array)
{
	free(array->ptr);
}

/*
	Integer pushing
*/

// push byte (1 byte)
void dynar_pushb(dynamic_array_t* array, uint8_t b)
{
	if (__builtin_expect( array->length == array->size, 0) ) {
		_resize(array, array->length + 1);
	}
	*(uint8_t*)(&array->ptr[array->length]) = b;
	array->length++;
}

// push short (2 bytes)
void dynar_pushs(dynamic_array_t* array, uint16_t s)
{
	if (__builtin_expect( array->length + 2 > array->size, 0) ) {
		_resize(array, array->length + 2);
	}
	*(uint16_t*)(&array->ptr[array->length]) = s;
	array->length += 2;
}

// push int (4 bytes)
void dynar_pushi(dynamic_array_t* array, uint32_t i)
{
	if (__builtin_expect( array->length + 4 > array->size, 0) ) {
		_resize(array, array->length + 4);
	}
	*(uint32_t*)(&array->ptr[array->length]) = i;
	array->length += 4;
}

// push long (8 bytes)
void dynar_pushl(dynamic_array_t* array, uint64_t l)
{
	if (__builtin_expect( array->length + 8 > array->size, 0) ) {
		_resize(array, array->length + 8);
	}
	*(uint64_t*)(&array->ptr[array->length]) = l;
	array->length += 8;
}

/*
	Float pushing
*/

// push float (4 bytes)
void dynar_pushf(dynamic_array_t* array, float f)
{
	if (__builtin_expect( array->length + 4 > array->size, 0) ) {
		_resize(array, array->length + 4);
	}
	*(float*)(&array->ptr[array->length]) = f;
	array->length += 4;
}

// push double (8 bytes)
void dynar_pushd(dynamic_array_t* array, double d)
{
	if (__builtin_expect( array->length + 8 > array->size, 0) ) {
		_resize(array, array->length + 8);
	}
	*(double*)(&array->ptr[array->length]) = d;
	array->length += 8;
}

/*
	Pointer pushing
*/

// push pointer (8 bytes)
void dynar_pushp(dynamic_array_t* array, void* p)
{
	if (__builtin_expect( array->length + 8 > array->size, 0) ) {
		_resize(array, array->length + 8);
	}
	*(void**)(&array->ptr[array->length]) = p;
	array->length += 8;
}

// push copy ('size' bytes)
void dynar_pushc(dynamic_array_t* array, void* p, uint32_t size)
{
	if (__builtin_expect( array->length + size > array->size, 0) ) {
		_resize(array, array->length + size);
	}
	memcpy(&array->ptr[array->length], p, size);
	array->length += size;
}

/*
	Size modification
*/

// resizes array to size 'size'
void dynar_resize(dynamic_array_t* array, uint32_t size)
{
	_resize(array, size);
}

// trims array to nearest power of two to length
void dynar_trim(dynamic_array_t* array)
{
	_resize(array, array->length);
}

// prepares array for addition of 'size' bytes
void dynar_topush(dynamic_array_t* array, uint32_t size)
{
	if (__builtin_expect( array->length + size > array->size, 0) ) {
		_resize(array, array->length + size);
	}
}