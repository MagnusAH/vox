#include "string.h"

// TODO, memory allocator replacements
#include <stdlib.h>

#include "mem.h"

// returns 1 when input is 0 or over 2_147_483_648, otherwise ok
inline uint32_t ceil_pow2(uint32_t n)
{
	return (uint32_t)1 << (32 - __builtin_clz(n - 1));
}

// inline for resizeing to ceil rounded power of two of size
inline void _resize1b(string1b_t* string, uint32_t size)
{
	string->size = ceil_pow2(size);
	string->c_str = realloc(string->c_str, string->size * sizeof(uint8_t));
}

inline void _resize2b(string2b_t* string, uint32_t size)
{
	string->size = ceil_pow2(size);
	string->c_str = realloc(string->c_str, string->size * sizeof(uint16_t));
}

inline void _resize4b(string4b_t* string, uint32_t size)
{
	string->size = ceil_pow2(size);
	string->c_str = realloc(string->c_str, string->size * sizeof(uint32_t));
}

/*
	String1b
*/

// initializes string1b_t with size 'size' in elements (defaults to 64 if 0 is passed)
void str1b_init(string1b_t* string, uint32_t size)
{
	string->size = size ? size : 64;
	string->length = 0;
	string->c_str = malloc(string->size * sizeof(uint8_t));
}

// appends a single element to the end of the string
void str1b_push(string1b_t* string, uint8_t element)
{
	if (__builtin_expect( string->length + 1 == string->size, 0) ) {
		_resize1b(string, string->size + 1);
	}
	string->c_str[string->length++] = element;
	string->c_str[string->length] = 0;
}

// appends an array of elements of length 'length' to the end of the string 
void str1b_pushc(string1b_t* string, uint8_t* src, uint32_t length)
{
	if (__builtin_expect( string->length + length >= string->size, 0) ) {
		_resize1b(string, string->size + length);
	}
	memcpy(&string->c_str[string->length], src, length * sizeof(uint8_t));
	string->length += length;
	string->c_str[string->length] = 0;
}

// appends the null terminated string 'src' to the end of the string
void str1b_pushs(string1b_t* string, uint8_t* src)
{
	uint32_t i;
	for (i = 0; src[i]; i++)
	{
		if (__builtin_expect( string->length + 1 == string->size, 0) ) {
			_resize1b(string, string->size + 1);
		}
		string->c_str[string->length + i] = src[i];
	}
	string->length += i;
	string->c_str[string->length] = 0;
}

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str1b_topush(string1b_t* string, uint32_t count)
{
	if (__builtin_expect( string->length + count >= string->size, 0) ) {
		_resize1b(string, string->size + count);
	}
}

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str1b_pushed(string1b_t* string, uint32_t pushed)
{
	string->length += pushed;
	string->c_str[string->length] = 0;
}

// indicates that string has been externally modified and should recalculate the length of the string
void str1b_sync(string1b_t* string)
{
	uint32_t i = 0;
	for (i = 0; string->c_str[i]; i++);
	string->length = i;
}

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str1b_resize(string1b_t* string, uint32_t size)
{
	_resize1b(string, size);
	if (__builtin_expect( string->length >= size, 0) ) {
		string->length = size - 1;
		string->c_str[string->length] = 0;
	}
}

// trims string size to ciel power of two of string length
void str1b_trim(string1b_t* string)
{
	_resize1b(string, string->length + 1);
}

// destroy the string (free the memory)
void str1b_destroy(string1b_t* string)
{
	free(string->c_str);
}

// -----------------------------------------------------------------------------------------

/*
	String2b
*/

// initializes string2b_t with size 'size' in elements (defaults to 32 if 0 is passed)
void str2b_init(string2b_t* string, uint32_t size)
{
	string->size = size ? size : 32;
	string->length = 0;
	string->c_str = malloc(string->size * sizeof(uint32_t));
}

// appends a single element to the end of the string
void str2b_push(string2b_t* string, uint16_t element)
{
	if (__builtin_expect( string->length + 1 == string->size, 0) ) {
		_resize2b(string, string->size + 1);
	}
	string->c_str[string->length++] = element;
	string->c_str[string->length] = 0;
}

// appends an array of elements of length 'length' to the end of the string 
void str2b_pushc(string2b_t* string, uint16_t* src, uint32_t length)
{
	if (__builtin_expect( string->length + length >= string->size, 0) ) {
		_resize2b(string, string->size + length);
	}
	memcpy(&string->c_str[string->length], src, length * sizeof(uint16_t));
	string->length += length;
	string->c_str[string->length] = 0;
}

// appends the null terminated string 'src' to the end of the string
void str2b_pushs(string2b_t* string, uint16_t* src)
{
	uint32_t i;
	for (i = 0; src[i]; i++)
	{
		if (__builtin_expect( string->length + 1 == string->size, 0) ) {
			_resize2b(string, string->size + 1);
		}
		string->c_str[string->length + i] = src[i];
	}
	string->length += i;
	string->c_str[string->length] = 0;
}

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str2b_topush(string2b_t* string, uint32_t count)
{
	if (__builtin_expect( string->length + count >= string->size, 0) ) {
		_resize2b(string, string->size + count);
	}
}

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str2b_pushed(string2b_t* string, uint32_t pushed)
{
	string->length += pushed;
	string->c_str[string->length] = 0;
}

// indicates that string has been externally modified and should recalculate the length of the string
void str2b_sync(string2b_t* string)
{
	uint32_t i = 0;
	for (i = 0; string->c_str[i]; i++);
	string->length = i;
}

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str2b_resize(string2b_t* string, uint32_t size)
{
	_resize2b(string, size);
	if (__builtin_expect( string->length >= size, 0) ) {
		string->length = size - 1;
		string->c_str[string->length] = 0;
	}
}

// trims string size to ciel power of two of string length
void str2b_trim(string2b_t* string)
{
	_resize2b(string, string->length + 1);
}

// destroy the string (free the memory)
void str2b_destroy(string2b_t* string)
{
	free(string->c_str);
}

// -----------------------------------------------------------------------------------------

/*
	String4b
*/

// initializes string4b_t with size 'size' in elements (defaults to 16 if 0 is passed)
void str4b_init(string4b_t* string, uint32_t size)
{
	string->size = size ? size : 16;
	string->length = 0;
	string->c_str = malloc(string->size * sizeof(uint32_t));
}

// appends a single element to the end of the string
void str4b_push(string4b_t* string, uint32_t element)
{
	if (__builtin_expect( string->length + 1 == string->size, 0) ) {
		_resize4b(string, string->size + 1);
	}
	string->c_str[string->length++] = element;
	string->c_str[string->length] = 0;
}

// appends an array of elements of length 'length' to the end of the string 
void str4b_pushc(string4b_t* string, uint32_t* src, uint32_t length)
{
	if (__builtin_expect( string->length + length >= string->size, 0) ) {
		_resize4b(string, string->size + length);
	}
	memcpy(&string->c_str[string->length], src, length * sizeof(uint32_t));
	string->length += length;
	string->c_str[string->length] = 0;
}

// appends the null terminated string 'src' to the end of the string
void str4b_pushs(string4b_t* string, uint32_t* src)
{
	uint32_t i;
	for (i = 0; src[i]; i++)
	{
		if (__builtin_expect( string->length + 1 == string->size, 0) ) {
			_resize4b(string, string->size + 1);
		}
		string->c_str[string->length + i] = src[i];
	}
	string->length += i;
	string->c_str[string->length] = 0;
}

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str4b_topush(string4b_t* string, uint32_t count)
{
	if (__builtin_expect( string->length + count >= string->size, 0) ) {
		_resize4b(string, string->size + count);
	}
}

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str4b_pushed(string4b_t* string, uint32_t pushed)
{
	string->length += pushed;
	string->c_str[string->length] = 0;
}

// indicates that string has been externally modified and should recalculate the length of the string
void str4b_sync(string4b_t* string)
{
	uint32_t i = 0;
	for (i = 0; string->c_str[i]; i++);
	string->length = i;
}

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str4b_resize(string4b_t* string, uint32_t size)
{
	_resize4b(string, size);
	if (__builtin_expect( string->length >= size, 0) ) {
		string->length = size - 1;
		string->c_str[string->length] = 0;
	}
}

// trims string size to ciel power of two of string length
void str4b_trim(string4b_t* string)
{
	_resize4b(string, string->length + 1);
}

// destroy the string (free the memory)
void str4b_destroy(string4b_t* string)
{
	free(string->c_str);
}

// -----------------------------------------------------------------------------------------