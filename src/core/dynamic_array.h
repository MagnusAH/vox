#pragma once

#include <stdint.h>

// can be treated as regular pointer
// array can't be larger than 2G due to uint32_t (though this shouldn't ever be a problem)
// 16 bytes
struct DynamicArray
{
	void* ptr;
	uint32_t length;
	uint32_t size;
};

typedef struct DynamicArray dynamic_array_t;

// init dynamic array with size 'size' (in bytes)
void dynar_init(dynamic_array_t* array, uint32_t size);

// destroy dynamic array (frees memory)
void dynar_destroy(dynamic_array_t* array);

// push byte (1 byte)
void dynar_pushb(dynamic_array_t* array, uint8_t b);
// push short (2 bytes)
void dynar_pushs(dynamic_array_t* array, uint16_t s);
// push int (4 bytes)
void dynar_pushi(dynamic_array_t* array, uint32_t i);
// push long (8 bytes)
void dynar_pushl(dynamic_array_t* array, uint64_t l);

// push float (4 bytes)
void dynar_pushf(dynamic_array_t* array, float f);
// push double (8 bytes)
void dynar_pushd(dynamic_array_t* array, double d);

// push pointer (8 bytes)
void dynar_pushp(dynamic_array_t* array, void* p);

// push copy ('size' bytes)
void dynar_pushc(dynamic_array_t* array, void* p, uint32_t size);

// resizes array to size 'size'
void dynar_resize(dynamic_array_t* array, uint32_t size);

// trims array to nearest power of two to length
void dynar_trim(dynamic_array_t* array);

// prepares array for addition of 'size' bytes
void dynar_topush(dynamic_array_t* array, uint32_t size);

// tells array 'size' bytes were pushed through unknown means
inline void dynar_pushed(dynamic_array_t* array, uint32_t size)
{
	array->length += size;
}

// clears array
inline void dynar_clear(dynamic_array_t* array)
{
	array->length = 0;
}

// returns push address (good for passing as destination pointer for methods)
// most likely should be preceded by a 'dynar_topush' call
inline void* dynar_pushloc(dynamic_array_t* array)
{
	return &array->ptr[array->length];
}