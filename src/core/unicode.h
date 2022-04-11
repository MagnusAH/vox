#pragma once

#include <stdint.h>

// worst case length ratios for ensuring destination buffer is properly sized
#define WORST_CASE_UTF8_TO_UTF16 1
#define WORST_CASE_UTF8_TO_UTF32 1

#define WORST_CASE_UTF16_TO_UTF8 2
#define WORST_CASE_UTF16_TO_UTF32 1

#define WORST_CASE_UTF32_TO_UTF8 4
#define WORST_CASE_UTF32_TO_UTF16 2

/*
	dest -> destination string, must already be allocated
	src -> source string, does not have to be null terminated
	length -> the length of the input string (excluding null terminator)
	return -> length of output string (excluding null terminator)
*/

// convert utf-16 string with length 'length' to a null terminated utf-8 string returning the length
uint32_t unicode_utf16_to_utf8(uint8_t* dest, const uint16_t* src, uint32_t length);

// convert utf-32 string with length 'length' to a null terminated utf-8 string returning the length
uint32_t unicode_utf32_to_utf8(uint8_t* dest, const uint32_t* src, uint32_t length);

// convert utf-8 string with length 'length' to a null terminated utf-16 string returning the length
uint32_t unicode_utf8_to_utf16(uint16_t* dest, const uint8_t* src, uint32_t length);

// convert utf-32 string with length 'length' to a null terminated utf-16 string returning the length
uint32_t unicode_utf32_to_utf16(uint16_t* dest, const uint32_t* src, uint32_t length);

// convert utf-8 string with length 'length' to a null terminated utf-32 string returning the length
uint32_t unicode_utf8_to_utf32(uint32_t* dest, const uint8_t* src, uint32_t length);

// convert utf-16 string with length 'length' to a null terminated utf-32 string returning the length
uint32_t unicode_utf16_to_utf32(uint32_t* dest, const uint8_t* src, uint32_t length);