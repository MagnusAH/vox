#include "simple_string.h"

#include <stdlib.h>
#include <string.h>

void* (*SS_MALLOC_FUNC)(uint64_t) = malloc;
void* (*SS_REALLOC_FUNC)(void*, uint64_t) = realloc;
void (*SS_FREE_FUNC)(void*) = free;

#define SS_MALLOC (*SS_MALLOC_FUNC)
#define SS_REALLOC (*SS_REALLOC_FUNC)
#define SS_FREE (*SS_FREE_FUNC)

// critical not under user control
#define SAFETY_CHECKS_L0
// not under user control, but not necesarrily vital to check for (ex: memory allocatioin failure)
#define SAFETY_CHECKS_L1
// checks for improper usage of functions (ex: inserting after last character when append should be used instead)
// #define SAFETY_cHECKS_L2

// no vectorized code currently
#ifdef SIMPLE_STRING_VECTORIZE
	#ifdef __AVX__
		#include <immintrin.h>
		#define SIMPLE_STRING_VECTORIZE_AMD64
	#endif
#endif

/*
	->c_str<-
*/

// UTF-8

uint32_t ss_c8_c16(char* dest, const uint16_t* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp = 0;
	for (uint32_t sp = 0; sp < length;)
	{
		if (sp < 0xD800) {
			if (src[sp] < 0x80) {
				dest[dp++] = src[sp];
			} else if (src[sp] < 0x800) {
				dest[dp++] = 0xC0 | (src[sp] >> 6);
				dest[dp++] = 0x80 | (src[sp] & 0x3F);
			} else {
				dest[dp++] = 0xE0 | (src[sp] >> 12);
				dest[dp++] = 0x80 | ((src[sp] >> 6) & 0x3F);
				dest[dp++] = 0x80 | (src[sp] & 0x3F);
			}
			sp++;
		} else {
			// uint32_t code = ((src[sp] - 0xD800) << 10) + (src[sp + 1] - 0xDC00) + 0x10000;
			uint32_t code = ((src[sp] & 0x3FF) << 10) + src[sp + 1] + 0x2400;
			if (code < 0x80) {
				dest[dp++] = code;
			} else if (code < 0x800) {
				dest[dp++] = 0xC0 | (code >> 6);
				dest[dp++] = 0x80 | (code & 0x3F);
			} else if (code < 0x8000) {
				dest[dp++] = 0xE0 | (code >> 12);
				dest[dp++] = 0xE0 | ((code >> 6) & 0x3F);
				dest[dp++] = 0x80 | (code & 0x3F);
			} else {
				dest[dp++] = 0xE0 | (code >> 18);
				dest[dp++] = 0x80 | ((code >> 12) & 0x3F);
				dest[dp++] = 0x80 | ((code >> 6) & 0x3F);
				dest[dp++] = 0x80 | (code & 0x3F);
			}
			sp+=2;
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

uint32_t ss_c8_c32(char* dest, const uint32_t* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp = 0;
	for (uint32_t sp = 0; sp < length; sp++) {
		if (src[sp] < 0x80) {
			dest[dp++] = src[sp];
		} else if (src[sp] < 0x800) {
			dest[dp++] = 0xC0 | (src[sp] >> 6);
			dest[dp++] = 0x80 | (src[sp] & 0x3F);
		} else if (src[sp] < 0x8000) {
			dest[dp++] = 0xE0 | (src[sp] >> 12);
			dest[dp++] = 0x80 | ((src[sp] >> 6) & 0x3F);
			dest[dp++] = 0x80 | (src[sp] & 0x3F);
		} else {
			dest[dp++] = 0xF0 | (src[sp] >> 18);
			dest[dp++] = 0x80 | ((src[sp] >> 12) & 0x3F);
			dest[dp++] = 0x80 | ((src[sp] >> 6) & 0x3F);
			dest[dp++] = 0x80 | (src[sp] & 0x3F);
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

// UTF-16

uint32_t ss_c16_c8(uint16_t* dest, const char* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp;
	for (uint32_t sp = 0; sp < length; sp++) {
		if (~(src[sp] & 0x80)) {
			dest[dp++] = src[sp];
			sp++;
		} else if (~(src[sp] & 0x20)) {
			dest[dp++] = ((src[sp] & 0x1F) << 6) | (src[sp + 1] & 0x3F);
			sp+=2;
		} else if (~(src[sp] & 0x10)) {
			dest[dp++] = ((src[sp] & 0x0F) << 12) | ((src[sp + 1] & 0x3F) << 6) | (src[sp + 2] & 0x3F);
			sp+=3;
		} else {
			uint32_t code = ( ((src[sp] & 0x0F) << 18) | ((src[sp + 1] & 0x3F) << 12) | ((src[sp + 2] & 0x3F) << 6) | (src[sp + 3] & 0x3F) ) - 0x10000;
			dest[dp++] = 0xD800 | (code >> 10);
			dest[dp++] = 0xDC00 | (code & 0x3FF);
			sp+=4;
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

uint32_t ss_c16_c32(uint16_t* dest, const uint32_t* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp = 0;
	for (uint32_t sp = 0; sp < length; sp++) {
		if (src[sp] < 0x10000) {
			dest[dp++] = src[sp];
		} else {
			dest[dp++] = 0xD800 | (src[sp] >> 10);
			dest[dp++] = 0xDC00 | (src[sp] & 0x3FF);
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

// UCS-4

uint32_t ss_c32_c8(uint32_t* dest, const char* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp = 0;
	for (uint32_t sp = 0; sp < length;) {
		if (~(src[sp] & 0x80)) {
			dest[dp++] = src[sp];
			sp++;
		} else if (~(src[sp] & 0x20)) {
			dest[dp++] = ((src[sp] & 0x1F) << 6) | (src[sp + 1] & 0x3F);
			sp+=2;
		} else if (~(src[sp] & 0x10)) {
			dest[dp++] = ((src[sp] & 0x0F) << 12) | ((src[sp + 1] & 0x3F) << 6) | (src[sp + 2] & 0x3F);
			sp+=3;
		} else {
			dest[dp++] = ((src[sp] & 0x0F) << 18) | ((src[sp + 1] & 0x3F) << 12) | ((src[sp + 2] & 0x3F) << 6) | (src[sp + 3] & 0x3F);
			sp+=4;
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

uint32_t ss_c32_c16(uint32_t* dest, const uint16_t* src, uint32_t length)
{
#ifdef SIMPLE_STRING_VECTORIZE_AMD64

#else
	uint32_t dp = 0;
	for (uint32_t sp = 0; sp < length;) {
		if (sp < 0xD800) {
			dest[dp++] = src[sp];
			sp++;
		} else {
			dest[dp++] = ((src[sp] & 0x3FF) << 10) + src[sp + 1] + 0x2400;
			sp+=2;
		}
	}
	dest[dp] = 0;
	return dp;
#endif
}

// -------------------------------------------------------------------------------------------

/*
	->string8<-
*/

// INIT

uint64_t ss_init8(struct SimpleString8* str)
{
	return ss_init8_s(str, 16);
}

uint64_t ss_init8_s(struct SimpleString8* str, uint32_t size)
{
	str->magic = SS_MAGIC_8;
	str->size = size;
	str->c_str = SS_MALLOC(size * sizeof(char));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = 0;
	str->c_str[0] = 0;
	return 0;
}

uint64_t ss_init8_c8(struct SimpleString8* str, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init8_c8l(str, src, length);
}

uint64_t ss_init8_c8l(struct SimpleString8* str, const char* src, uint32_t length)
{
	str->magic = SS_MAGIC_8;
	str->size = length + 1;
	str->c_str = SS_MALLOC(str->size * sizeof(char));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = length;
	str->c_str[length] = 0;
	memcpy(str->c_str, src, length);
	return 0;
}

uint64_t ss_init8_c16(struct SimpleString8* str, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init8_c16l(str, src, length);
}

uint64_t ss_init8_c16l(struct SimpleString8* str, const uint16_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_8;
	str->size = length * 3 + 1; // worst case size ratio
	str->c_str = SS_MALLOC(str->size * sizeof(char));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c8_c16(str->c_str, src, length);
	ss_trim8(str);
	return 0;
}

uint64_t ss_init8_c32(struct SimpleString8* str, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init8_c32l(str, src, length);
}

uint64_t ss_init8_c32l(struct SimpleString8* str, const uint32_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_8;
	str->size = length * 4 + 1; // worst case size ratio
	str->c_str = SS_MALLOC(str->size * sizeof(char));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c8_c32(str->c_str, src, length);
	ss_trim8(str);
	return 0;
}

// APPEND

uint64_t ss_append8_c8(struct SimpleString8* dest, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append8_c8l(dest, src, length);
}

uint64_t ss_append8_c8l(struct SimpleString8* dest, const char* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			char* str = SS_REALLOC(dest->c_str, size * sizeof(char));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(char));
		#endif
		dest->size = size;
	}
	memcpy(dest->c_str + dest->length, src, length);
	dest->length += length;
	dest->c_str[length] = 0;
	return 0;
}

uint64_t ss_append8_c16(struct SimpleString8* dest, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append8_c16l(dest, src, length);
}

uint64_t ss_append8_c16l(struct SimpleString8* dest, const uint16_t* src, uint32_t length)
{
	if (dest->length + length * 3 + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length * 3 + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			char* str = SS_REALLOC(dest->c_str, size * sizeof(char));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(char));
		#endif
		dest->size = size;
		dest->length += ss_c8_c16(dest->c_str + dest->length, src, length);
		ss_trim8(dest);
	} else {
		dest->length += ss_c8_c16(dest->c_str + dest->length, src, length);
	}
	return 0;
}

uint64_t ss_append8_c32(struct SimpleString8* dest, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append8_c32l(dest, src, length);
}

uint64_t ss_append8_c32l(struct SimpleString8* dest, const uint32_t* src, uint32_t length)
{
	if (dest->length + length * 4 + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length * 4 + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			char* str = SS_REALLOC(dest->c_str, size * sizeof(char));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(char));
		#endif
		dest->size = size;
		dest->length += ss_c8_c32(dest->c_str + dest->length, src, length);
		ss_trim8(dest);
	} else {
		dest->length += ss_c8_c32(dest->c_str + dest->length, src, length);
	}
	return 0;
}

// INSERT

uint64_t ss_insert8_c8(struct SimpleString8* dest, const char* src, uint32_t index)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_insert8_c8l(dest, src, index, length);
}

uint64_t ss_insert8_c8l(struct SimpleString8* dest, const char* src, uint32_t index, uint32_t length)
{
	#ifdef SAFETY_cHECKS_L2
		if (index > dest->length - 1) index = dest->length - 1;
	#endif
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size >>= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			char* str = SS_REALLOC(dest->c_str, size);
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size);
		#endif
		dest->size = size;
	}
	memmove(dest->c_str + index, dest->c_str + index + length, (dest->length - index) * sizeof(char));
	memcpy(dest->c_str + index, src, length * sizeof(char));
	dest->length += length;
	dest->c_str[dest->length] = 0;
	return 0;
}

// EXTRA

void ss_trim8(struct SimpleString8* str)
{
	if (str->size >> 1 > 15 && str->size >> 1 > str->length + 1) {
		while (str->size >> 1 > 15 && str->size >> 1 > str->length + 1) {
			str->size >>= 1;
		}
		str->c_str = SS_REALLOC(str->c_str, str->size);
	}
}

void ss_sync8(struct SimpleString8* str)
{
	uint32_t length = 0;
	while (str->c_str[length]) { length++; }
	str->length = length;
}

void ss_clear8(struct SimpleString8* str)
{
	str->c_str[0] = 0;
	str->length = 0;
}

uint64_t ss_resize8(struct SimpleString8* str, uint32_t size)
{
	#ifdef SAFETY_CHECKS_L1
		char* cstr = SS_REALLOC(str->c_str, size * sizeof(char));
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
		str->c_str = cstr;
	#else
		str->c_str = SS_REALLOC(str->c_str, size * sizeof(char));
	#endif
	str->size = size;
	if (size - 1 < str->length) {
		str->length = size - 1;
		str->c_str[str->length] = 0;
	}
	return 0;
}

uint64_t ss_ensize8(struct SimpleString8* str, uint32_t size)
{
	if (str->size >= size) return 0;
	return ss_resize8(str, size);
}


// -------------------------------------------------------------------------------------------

/*
	->string16<-
*/

// INIT

uint64_t ss_init16(struct SimpleString16* str)
{
	return ss_init16_s(str, 8);
}

uint64_t ss_init16_s(struct SimpleString16* str, uint32_t size)
{
	str->magic = SS_MAGIC_16;
	str->size = size;
	str->c_str = SS_MALLOC(size * sizeof(uint16_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = 0;
	str->c_str[0] = 0;
	return 0;
}

uint64_t ss_init16_c8(struct SimpleString16* str, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init16_c8l(str, src, length);
}

uint64_t ss_init16_c8l(struct SimpleString16* str, const char* src, uint32_t length)
{
	str->magic = SS_MAGIC_16;
	str->size = length + 1; // worst case size ratio
	str->c_str = SS_MALLOC(str->size * sizeof(uint16_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c16_c8(str->c_str, src, length);
	ss_trim16(str);
	return 0;
}

uint64_t ss_init16_c16(struct SimpleString16* str, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init16_c16l(str, src, length);
}

uint64_t ss_init16_c16l(struct SimpleString16* str, const uint16_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_16;
	str->size = length + 1;
	str->c_str = SS_MALLOC(str->size * sizeof(uint16_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = length;
	str->c_str[length] = 0;
	memcpy(str->c_str, src, length * sizeof(uint16_t));
	return 0;
}

uint64_t ss_init16_c32(struct SimpleString16* str, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init16_c32l(str, src, length);
}

uint64_t ss_init16_c32l(struct SimpleString16* str, const uint32_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_16;
	str->size = length * 2 + 1; // worst case size ratio
	str->c_str = SS_MALLOC(str->size * sizeof(uint16_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c16_c32(str->c_str, src, length);
	ss_trim16(str);
	return 0;
}

// APPEND

uint64_t ss_append16_c8(struct SimpleString16* dest, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append16_c8l(dest, src, length);
}

uint64_t ss_append16_c8l(struct SimpleString16* dest, const char* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint16_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
		#endif
		dest->size = size;
		dest->length += ss_c16_c8(dest->c_str + dest->length, src, length);
		ss_trim16(dest);
	} else {
		dest->length += ss_c16_c8(dest->c_str + dest->length, src, length);
	}
	return 0;
}

uint64_t ss_append16_c16(struct SimpleString16* dest, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append16_c16l(dest, src, length);
}

uint64_t ss_append16_c16l(struct SimpleString16* dest, const uint16_t* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size;
		while (dest->length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint16_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
		#endif
		dest->size = size;
	}
	memcpy(dest->c_str + dest->length, src, length * sizeof(uint16_t));
	dest->length += length;
	dest->c_str[length] = 0;
	return 0;
}

uint64_t ss_append16_c32(struct SimpleString16* dest, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append16_c32l(dest, src, length);
}

uint64_t ss_append16_c32l(struct SimpleString16* dest, const uint32_t* src, uint32_t length)
{
	if (dest->length + length * 2 + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length * 2 + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint16_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
		#endif
		dest->size = size;
		dest->length += ss_c16_c32(dest->c_str + dest->length, src, length);
		ss_trim16(dest);
	} else {
		dest->length += ss_c16_c32(dest->c_str + dest->length, src, length);
	}
	return 0;
}

// INSERT

uint64_t ss_insert16_c16(struct SimpleString16* dest, const uint16_t* src, uint32_t index)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_insert16_c16l(dest, src, index, length);
}

uint64_t ss_insert16_c16l(struct SimpleString16* dest, const uint16_t* src, uint32_t index, uint32_t length)
{
	#ifdef SAFETY_cHECKS_L2
		if (index > dest->length - 1) index = dest->length - 1;
	#endif
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size >>= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint16_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint16_t));
		#endif
		dest->size = size;
	}
	memmove(dest->c_str + index, dest->c_str + index + length, (dest->length - index) * sizeof(uint16_t));
	memcpy(dest->c_str + index, src, length * sizeof(uint16_t));
	dest->length += length;
	dest->c_str[dest->length] = 0;
	return 0;
}

// EXTRA

void ss_trim16(struct SimpleString16* str)
{
	if (str->size >> 1 > 7 && str->size >> 1 > str->length + 1) {
		while (str->size >> 1 > 7 && str->size >> 1 > str->length + 1) {
			str->size >>= 1;
		}
		str->c_str = SS_REALLOC(str->c_str, str->size);
	}
}

void ss_sync16(struct SimpleString16* str)
{
	uint32_t length = 0;
	while (str->c_str[length]) { length++; }
	str->length = length;
}

void ss_clear16(struct SimpleString16* str)
{
	str->c_str[0] = 0;
	str->length = 0;
}

uint64_t ss_resize16(struct SimpleString16* str, uint32_t size)
{
	#ifdef SAFETY_CHECKS_L1
		uint16_t* cstr = SS_REALLOC(str->c_str, size * sizeof(uint16_t));
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
		str->c_str = cstr;
	#else
		str->c_str = SS_REALLOC(str->c_str, size * sizeof(uint16_t));
	#endif
	str->size = size;
	if (size - 1 < str->length) {
		str->length = size - 1;
		str->c_str[str->length] = 0;
	}
	return 0;
}

uint64_t ss_ensize16(struct SimpleString16* str, uint32_t size)
{
	if (str->size >= size) return 0;
	return ss_resize16(str, size);
}

// -------------------------------------------------------------------------------------------

/*
	->string32<-
*/

// INIT

uint64_t ss_init32(struct SimpleString32* str)
{
	return ss_init32_s(str, 4);
}

uint64_t ss_init32_s(struct SimpleString32* str, uint32_t size)
{
	str->magic = SS_MAGIC_32;
	str->size = size;
	str->c_str = SS_MALLOC(size * sizeof(uint32_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = 0;
	str->c_str[0] = 0;
	return 0;
}

uint64_t ss_init32_c8(struct SimpleString32* str, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init32_c8l(str, src, length);
}

uint64_t ss_init32_c8l(struct SimpleString32* str, const char* src, uint32_t length)
{
	str->magic = SS_MAGIC_32;
	str->size = length + 1;
	str->c_str = SS_MALLOC(str->size * sizeof(uint32_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c32_c8(str->c_str, src, length);
	ss_trim32(str);
	return 0;
}

uint64_t ss_init32_c16(struct SimpleString32* str, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init32_c16l(str, src, length);
}

uint64_t ss_init32_c16l(struct SimpleString32* str, const uint16_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_32;
	str->size = length + 1;
	str->c_str = SS_MALLOC(str->size * sizeof(uint32_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = ss_c32_c16(str->c_str, src, length);
	ss_trim32(str);
	return 0;
}

uint64_t ss_init32_c32(struct SimpleString32* str, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_init32_c32l(str, src, length);
}

uint64_t ss_init32_c32l(struct SimpleString32* str, const uint32_t* src, uint32_t length)
{
	str->magic = SS_MAGIC_32;
	str->size = length + 1;
	str->c_str = SS_MALLOC(str->size * sizeof(uint32_t));
	#ifdef SAFETY_CHECKS_L1
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
	#endif
	str->length = length;
	str->c_str[length] = 0;
	memcpy(str->c_str, src, length * sizeof(uint32_t));
	return 0;
}

// APPEND

uint64_t ss_append32_c8(struct SimpleString32* dest, const char* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append32_c8l(dest, src, length);
}

uint64_t ss_append32_c8l(struct SimpleString32* dest, const char* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint32_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
		#endif
		dest->size = size;
		dest->length += ss_c32_c8(dest->c_str + dest->length, src, length);
		ss_trim32(dest);
	} else {
		dest->length += ss_c32_c8(dest->c_str + dest->length, src, length);
	}
	return 0;
}

uint64_t ss_append32_c16(struct SimpleString32* dest, const uint16_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append32_c16l(dest, src, length);
}

uint64_t ss_append32_c16l(struct SimpleString32* dest, const uint16_t* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint32_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
		#endif
		dest->size = size;
		dest->length += ss_c32_c16(dest->c_str + dest->length, src, length);
		ss_trim32(dest);
	} else {
		dest->length += ss_c32_c16(dest->c_str + dest->length, src, length);
	}
	return 0;
}

uint64_t ss_append32_c32(struct SimpleString32* dest, const uint32_t* src)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_append32_c32l(dest, src, length);
}

uint64_t ss_append32_c32l(struct SimpleString32* dest, const uint32_t* src, uint32_t length)
{
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + 1 > size) {
			size <<= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint32_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
		#endif
		dest->size = size;
	}
	memcpy(dest->c_str + dest->length, src, length * sizeof(uint16_t));
	dest->length += length;
	dest->c_str[length] = 0;
	return 0;
}

// INSERT

uint64_t ss_insert32_c32(struct SimpleString32* dest, const uint32_t* src, uint32_t index)
{
	uint32_t length = 0;
	while (src[length]) { length++; }
	return ss_insert32_c32l(dest, src, index, length);
}

uint64_t ss_insert32_c32l(struct SimpleString32* dest, const uint32_t* src, uint32_t index, uint32_t length)
{
	#ifdef SAFETY_cHECKS_L2
		if (index > dest->length - 1) index = dest->length - 1;
	#endif
	if (dest->length + length + 1 > dest->size) {
		uint32_t size = dest->size;
		while (dest->length + length + 1 > size) {
			size >>= 1;
		}
		#ifdef SAFETY_CHECKS_L1
			uint32_t* str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
			if (str == NULL) return SS_MEMORY_ALLOC_FAILED;
			dest->c_str = str;
		#else
			dest->c_str = SS_REALLOC(dest->c_str, size * sizeof(uint32_t));
		#endif
		dest->size = size;
	}
	memmove(dest->c_str + index, dest->c_str + index + length, (dest->length - index) * sizeof(uint32_t));
	memcpy(dest->c_str + index, src, length * sizeof(uint32_t));
	dest->length += length;
	dest->c_str[dest->length] = 0;
	return 0;
}

// EXTRA

void ss_trim32(struct SimpleString32* str)
{
	if (str->size >> 1 > 3 && str->size >> 1 > str->length + 1) {
		while (str->size >> 1 > 3 && str->size >> 1 > str->length + 1) {
			str->size >>= 1;
		}
		str->c_str = SS_REALLOC(str->c_str, str->size);
	}
}

void ss_sync32(struct SimpleString32* str)
{
	uint32_t length = 0;
	while (str->c_str[length]) { length++; }
	str->length = length;
}

void ss_clear32(struct SimpleString32* str)
{
	str->c_str[0] = 0;
	str->length = 0;
}

uint64_t ss_resize32(struct SimpleString32* str, uint32_t size)
{
	#ifdef SAFETY_CHECKS_L1
		uint32_t* cstr = SS_REALLOC(str->c_str, size * sizeof(uint32_t));
		if (str->c_str == NULL) return SS_MEMORY_ALLOC_FAILED;
		str->c_str = cstr;
	#else
		str->c_str = SS_REALLOC(str->c_str, size * sizeof(uint32_t));
	#endif
	str->size = size;
	if (size - 1 < str->length) {
		str->length = size - 1;
		str->c_str[str->length] = 0;
	}
	return 0;
}

uint64_t ss_ensize32(struct SimpleString32* str, uint32_t size)
{
	if (str->size >= size) return 0;
	return ss_resize32(str, size);
}

// -------------------------------------------------------------------------------------------

/*
	->generic<-
*/

void _ssg_destroy(struct SimpleString* str)
{
	SS_FREE(str->c_str);
	memset(str, 0, sizeof(struct SimpleString));
}

uint64_t _ssg_init8(struct SimpleString8* str, struct SimpleString* src)
{
	switch (src->magic)
	{
		case SS_MAGIC_8:
			return ss_init8_c8l(str, (const char*)src->c_str, src->length);
			break;
		case SS_MAGIC_16:
			return ss_init8_c16l(str, (const uint16_t*)src->c_str, src->length);
			break;
		case SS_MAGIC_32:
			return ss_init8_c32l(str, (const uint32_t*)src->c_str, src->length);
			break;
		default:
			return SS_INVALID_STRING_PASSED;
	}
}

uint64_t _ssg_init16(struct SimpleString16* str, struct SimpleString* src)
{
	switch (src->magic)
	{
		case SS_MAGIC_8:
			return ss_init16_c8l(str, (const char*)src->c_str, src->length);
			break;
		case SS_MAGIC_16:
			return ss_init16_c16l(str, (const uint16_t*)src->c_str, src->length);
			break;
		case SS_MAGIC_32:
			return ss_init16_c32l(str, (const uint32_t*)src->c_str, src->length);
			break;
		default:
			return SS_INVALID_STRING_PASSED;
	}
}

uint64_t _ssg_init32(struct SimpleString32* str, struct SimpleString* src)
{
	switch (src->magic)
	{
		case SS_MAGIC_8:
			return ss_init32_c8l(str, (const char*)src->c_str, src->length);
			break;
		case SS_MAGIC_16:
			return ss_init32_c16l(str, (const uint16_t*)src->c_str, src->length);
			break;
		case SS_MAGIC_32:
			return ss_init32_c32l(str, (const uint32_t*)src->c_str, src->length);
			break;
		default:
			return SS_INVALID_STRING_PASSED;
	}
}
