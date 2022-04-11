#pragma once

#include <stdint.h>

// null terminated dynamically sized arrays of fixed element size

// 16 bytes
struct String1b
{
	uint8_t* c_str;
	uint32_t length; // max 2_147_483_647
	uint32_t size;   // max 2_147_483_648
};
typedef struct String1b string1b_t;

// 16 bytes
struct String2b
{
	uint16_t* c_str;
	uint32_t length; // max 2_147_483_647
	uint32_t size;   // max 2_147_483_648
};
typedef struct String2b string2b_t;

// 16 bytes
struct String4b
{
	uint32_t* c_str;
	uint32_t length; // max 2_147_483_647
	uint32_t size;   // max 2_147_483_648
};
typedef struct String4b string4b_t;

/*
	c_str can be set to a const pointer, but use of functions aside from 'pushed' and 'sync' should be avoided
	length does not include the null terminator, so the string is full at "length == size - 1"
*/

// -----------------------------------------------------------------------------------------

/*
	String1b
*/

// initializes string1b_t with size 'size' in elements (defaults to 64 if 0 is passed)
void str1b_init(string1b_t* string, uint32_t size);

// appends a single element to the end of the string
void str1b_push(string1b_t* string, uint8_t element);

// appends an array of elements of length 'length' to the end of the string 
void str1b_pushc(string1b_t* string, uint8_t* src, uint32_t length);

// appends the null terminated string 'src' to the end of the string
void str1b_pushs(string1b_t* string, uint8_t* src);

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str1b_topush(string1b_t* string, uint32_t count);

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str1b_pushed(string1b_t* string, uint32_t pushed);

// indicates that string has been externally modified and should recalculate the length of the string
void str1b_sync(string1b_t* string);

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str1b_resize(string1b_t* string, uint32_t size);

// trims string size to ciel power of two of string length
void str1b_trim(string1b_t* string);

// destroy the string (free the memory)
void str1b_destroy(string1b_t* string);

// -----------------------------------------------------------------------------------------

/*
	String2b
*/

// initializes string2b_t with size 'size' in elements (defaults to 32 if 0 is passed)
void str2b_init(string2b_t* string, uint32_t size);

// appends a single element to the end of the string
void str2b_push(string2b_t* string, uint16_t element);

// appends an array of elements of length 'length' to the end of the string 
void str2b_pushc(string2b_t* string, uint16_t* src, uint32_t length);

// appends the null terminated string 'src' to the end of the string
void str2b_pushs(string2b_t* string, uint16_t* src);

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str2b_topush(string2b_t* string, uint32_t count);

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str2b_pushed(string2b_t* string, uint32_t pushed);

// indicates that string has been externally modified and should recalculate the length of the string
void str2b_sync(string2b_t* string);

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str2b_resize(string2b_t* string, uint32_t size);

// trims string size to ciel power of two of string length
void str2b_trim(string2b_t* string);

// destroy the string (free the memory)
void str2b_destroy(string2b_t* string);

// -----------------------------------------------------------------------------------------

/*
	String4b
*/

// initializes string4b_t with size 'size' in elements (defaults to 16 if 0 is passed)
void str4b_init(string4b_t* string, uint32_t size);

// appends a single element to the end of the string
void str4b_push(string4b_t* string, uint32_t element);

// appends an array of elements of length 'length' to the end of the string 
void str4b_pushc(string4b_t* string, uint32_t* src, uint32_t length);

// appends the null terminated string 'src' to the end of the string
void str4b_pushs(string4b_t* string, uint32_t* src);

// indicates that there needs to be space for minimum of 'count' elements to be appended to the string
void str4b_topush(string4b_t* string, uint32_t count);

// indicates that 'pushed' elements were appended to the string and the length should be modified, and null terminator placed correctly
void str4b_pushed(string4b_t* string, uint32_t pushed);

// indicates that string has been externally modified and should recalculate the length of the string
void str4b_sync(string4b_t* string);

// sets string size to ceil power of two of 'size', null terminator is placed at index size (if original length was >= size)
void str4b_resize(string4b_t* string, uint32_t size);

// trims string size to ciel power of two of string length
void str4b_trim(string4b_t* string);

// destroy the string (free the memory)
void str4b_destroy(string4b_t* string);

// -----------------------------------------------------------------------------------------