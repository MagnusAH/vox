#pragma once

#include <stdint.h>

// should always inline with current function length
#ifndef INLINE
	#define INLINE inline
#endif

#define SS_MAGIC_8 1 // sizeof(char)
#define SS_MAGIC_16 2 // sizeof(uint16_t)
#define SS_MAGIC_32 3 // sizeof(uint32_t)

#define SS_MAGIC_UTF8 SS_MAGIC_8
#define SS_MAGIC_UTF16 SS_MAGIC_16
#define SS_MAGIC_UCS4 SS_MAGIC_32

#define SS_MEMORY_ALLOC_FAILED 0x1
#define SS_INVALID_STRING_PASSED 0x2

// ratio of dest : src buffer for conversion
#define SS_C8_C16_WORST_RATIO 3
#define SS_C8_C32_WORST_RATIO 4

// ratio of dest : src buffer for conversion
#define SS_C16_C8_WORST_RATIO 1
#define SS_C16_C32_WORST_RATIO 2

// ratio of dest : src buffer for conversion
#define SS_C32_C8_WORST_RATIO 1
#define SS_C32_C16_WORST_RATIO 1

// generic type (for functions params that accept any type of string)
struct SimpleString
{
	void* c_str;
	uint32_t length;
	uint32_t size;
	uint32_t magic;	// for identifying string type, (might remove if it doesn't add value)
};

// UTF-8 string
struct SimpleString8
{
	char* c_str;
	uint32_t length;
	uint32_t size;
	uint32_t magic;
};

// UTF-16 string
struct SimpleString16
{
	uint16_t* c_str;
	uint32_t length;
	uint32_t size;
	uint32_t magic;
};

// UCS-4 string (codepoints)
struct SimpleString32
{
	uint32_t* c_str;
	uint32_t length;
	uint32_t size;
	uint32_t magic;
};

typedef struct SimpleString string_t;
typedef struct SimpleString8 string8_t;
typedef struct SimpleString16 string16_t;
typedef struct SimpleString32 string32_t;

// -------------------------------------------------------------------------------------------

/*
	->c_str<-
*/

// to UTF-8
uint32_t ss_c8_c16(char* dest, const uint16_t* src, uint32_t length);
uint32_t ss_c8_c32(char* dest, const uint32_t* src, uint32_t length);

// to UTF-16
uint32_t ss_c16_c8(uint16_t* dest, const char* src, uint32_t length);
uint32_t ss_c16_c32(uint16_t* dest, const uint32_t* src, uint32_t length);

// to UCS-4
uint32_t ss_c32_c8(uint32_t* dest, const char* src, uint32_t length);
uint32_t ss_c32_c16(uint32_t* dest, const uint16_t* src, uint32_t length);

// -------------------------------------------------------------------------------------------

/*
	->string8<-
*/

// init

uint64_t ss_init8(struct SimpleString8* str);
uint64_t ss_init8_s(struct SimpleString8* str, uint32_t size);
uint64_t ss_init8_c8(struct SimpleString8* str, const char* src);
uint64_t ss_init8_c8l(struct SimpleString8* str, const char* src, uint32_t length);
uint64_t ss_init8_c16(struct SimpleString8* str, const uint16_t* src);
uint64_t ss_init8_c16l(struct SimpleString8* str, const uint16_t* src, uint32_t length);
uint64_t ss_init8_c32(struct SimpleString8* str, const uint32_t* src);
uint64_t ss_init8_c32l(struct SimpleString8* str, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_init8_s8(struct SimpleString8* str, struct SimpleString8* src) { return ss_init8_c8l(str, src->c_str, src->length); }
INLINE uint64_t ss_init8_s16(struct SimpleString8* str, struct SimpleString16* src) { return ss_init8_c16l(str, src->c_str, src->length); }
INLINE uint64_t ss_init8_s32(struct SimpleString8* str, struct SimpleString32* src) { return ss_init8_c32l(str, src->c_str, src->length); }

// append

uint64_t ss_append8_c8(struct SimpleString8* dest, const char* src);
uint64_t ss_append8_c8l(struct SimpleString8* dest, const char* src, uint32_t length);
uint64_t ss_append8_c16(struct SimpleString8* dest, const uint16_t* src);
uint64_t ss_append8_c16l(struct SimpleString8* dest, const uint16_t* src, uint32_t length);
uint64_t ss_append8_c32(struct SimpleString8* dest, const uint32_t* src);
uint64_t ss_append8_c32l(struct SimpleString8* dest, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_append8_s8(struct SimpleString8* dest, struct SimpleString8* src) { return ss_append8_c8l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append8_s16(struct SimpleString8* dest, struct SimpleString16* src) { return ss_append8_c16l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append8_s32(struct SimpleString8* dest, struct SimpleString32* src) { return ss_append8_c32l(dest, src->c_str, src->length); }

// insert

uint64_t ss_insert8_c8(struct SimpleString8* dest, const char* src, uint32_t index);
uint64_t ss_insert8_c8l(struct SimpleString8* dest, const char* src, uint32_t index, uint32_t length);

INLINE uint64_t ss_insert8_s8(struct SimpleString8* dest, struct SimpleString8* src, uint32_t index) { return ss_insert8_c8l(dest, src->c_str, src->length, index); }

// extra

void ss_trim8(struct SimpleString8* str);
void ss_sync8(struct SimpleString8* str);
void ss_clear8(struct SimpleString8* str);
uint64_t ss_resize8(struct SimpleString8* str, uint32_t size);
uint64_t ss_ensize8(struct SimpleString8* str, uint32_t size);

// -------------------------------------------------------------------------------------------

/*
	->string16<-
*/

// create


// init

uint64_t ss_init16(struct SimpleString16* str);
uint64_t ss_init16_s(struct SimpleString16* str, uint32_t size);
uint64_t ss_init16_c8(struct SimpleString16* str, const char* src);
uint64_t ss_init16_c8l(struct SimpleString16* str, const char* src, uint32_t length);
uint64_t ss_init16_c16(struct SimpleString16* str, const uint16_t* src);
uint64_t ss_init16_c16l(struct SimpleString16* str, const uint16_t* src, uint32_t length);
uint64_t ss_init16_c32(struct SimpleString16* str, const uint32_t* src);
uint64_t ss_init16_c32l(struct SimpleString16* str, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_init16_s8(struct SimpleString16* str, struct SimpleString8* src) { return ss_init16_c8l(str, src->c_str, src->length); }
INLINE uint64_t ss_init16_s16(struct SimpleString16* str, struct SimpleString16* src) { return ss_init16_c16l(str, src->c_str, src->length); }
INLINE uint64_t ss_init16_s32(struct SimpleString16* str, struct SimpleString32* src) { return ss_init16_c32l(str, src->c_str, src->length); }

// append

uint64_t ss_append16_c8(struct SimpleString16* dest, const char* src);
uint64_t ss_append16_c8l(struct SimpleString16* dest, const char* src, uint32_t length);
uint64_t ss_append16_c16(struct SimpleString16* dest, const uint16_t* src);
uint64_t ss_append16_c16l(struct SimpleString16* dest, const uint16_t* src, uint32_t length);
uint64_t ss_append16_c32(struct SimpleString16* dest, const uint32_t* src);
uint64_t ss_append16_c32l(struct SimpleString16* dest, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_append16_s8(struct SimpleString16* dest, struct SimpleString8* src) { return ss_append16_c8l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append16_s16(struct SimpleString16* dest, struct SimpleString16* src) { return ss_append16_c16l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append16_s32(struct SimpleString16* dest, struct SimpleString32* src) { return ss_append16_c32l(dest, src->c_str, src->length); }

// insert

uint64_t ss_insert16_c16(struct SimpleString16* dest, const uint16_t* src, uint32_t index);
uint64_t ss_insert16_c16l(struct SimpleString16* dest, const uint16_t* src, uint32_t index, uint32_t length);

INLINE uint64_t ss_insert16_s16(struct SimpleString16* dest, struct SimpleString16* src, uint32_t index) { return ss_insert16_c16l(dest, src->c_str, src->length, index); }

// extra

void ss_trim16(struct SimpleString16* str);
void ss_sync16(struct SimpleString16* str);
void ss_clear16(struct SimpleString16* str);
uint64_t ss_resize16(struct SimpleString16* str, uint32_t size);
uint64_t ss_ensize16(struct SimpleString16* str, uint32_t size);

// -------------------------------------------------------------------------------------------

/*
	->string32<-
*/

// create


// init

uint64_t ss_init32(struct SimpleString32* str);
uint64_t ss_init32_s(struct SimpleString32* str, uint32_t size);
uint64_t ss_init32_c8(struct SimpleString32* str, const char* src);
uint64_t ss_init32_c8l(struct SimpleString32* str, const char* src, uint32_t length);
uint64_t ss_init32_c16(struct SimpleString32* str, const uint16_t* src);
uint64_t ss_init32_c16l(struct SimpleString32* str, const uint16_t* src, uint32_t length);
uint64_t ss_init32_c32(struct SimpleString32* str, const uint32_t* src);
uint64_t ss_init32_c32l(struct SimpleString32* str, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_init32_s8(struct SimpleString32* str, struct SimpleString8* src) { return ss_init32_c8l(str, src->c_str, src->length); }
INLINE uint64_t ss_init32_s16(struct SimpleString32* str, struct SimpleString16* src) { return ss_init32_c16l(str, src->c_str, src->length); }
INLINE uint64_t ss_init32_s32(struct SimpleString32* str, struct SimpleString32* src) { return ss_init32_c32l(str, src->c_str, src->length); }

// append

uint64_t ss_append32_c8(struct SimpleString32* dest, const char* src);
uint64_t ss_append32_c8l(struct SimpleString32* dest, const char* src, uint32_t length);
uint64_t ss_append32_c16(struct SimpleString32* dest, const uint16_t* src);
uint64_t ss_append32_c16l(struct SimpleString32* dest, const uint16_t* src, uint32_t length);
uint64_t ss_append32_c32(struct SimpleString32* dest, const uint32_t* src);
uint64_t ss_append32_c32l(struct SimpleString32* dest, const uint32_t* src, uint32_t length);

INLINE uint64_t ss_append32_s8(struct SimpleString32* dest, struct SimpleString8* src) { return ss_append32_c8l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append32_s16(struct SimpleString32* dest, struct SimpleString16* src) { return ss_append32_c16l(dest, src->c_str, src->length); }
INLINE uint64_t ss_append32_s32(struct SimpleString32* dest, struct SimpleString32* src) { return ss_append32_c32l(dest, src->c_str, src->length); }

// insert

uint64_t ss_insert32_c32(struct SimpleString32* dest, const uint32_t* src, uint32_t index);
uint64_t ss_insert32_c32l(struct SimpleString32* dest, const uint32_t* src, uint32_t index, uint32_t length);

INLINE uint64_t ss_insert32_s32(struct SimpleString32* dest, struct SimpleString32* src, uint32_t index) { return ss_insert32_c32l(dest, src->c_str, src->length, index); }

// extra

void ss_trim32(struct SimpleString32* str);
void ss_sync32(struct SimpleString32* str);
void ss_clear32(struct SimpleString32* str);
uint64_t ss_resize32(struct SimpleString32* str, uint32_t size);
uint64_t ss_ensize32(struct SimpleString32* str, uint32_t size);

// -------------------------------------------------------------------------------------------

/*
	->generic<-
*/

void _ssg_destroy(struct SimpleString* str);

uint64_t ssg_init8(struct SimpleString8* str, struct SimpleString* src);
uint64_t ssg_init16(struct SimpleString16* str, struct SimpleString* src);
uint64_t ssg_init32(struct SimpleString32* str, struct SimpleString* src);

INLINE uint16_t ssg_compType(struct SimpleString* a, struct SimpleString* b)
{
	return a->magic == b->magic;
}

INLINE uint16_t ssg_isUTF8(struct SimpleString* str)
{
	return str->magic = SS_MAGIC_8;
}

INLINE uint16_t ssg_isUTF16(struct SimpleString* str)
{
	return str->magic = SS_MAGIC_16;
}

INLINE uint16_t ssg_isUCS4(struct SimpleString* str)
{
	return str->magic == SS_MAGIC_32;
}

INLINE uint32_t ssg_wsize8(struct SimpleString* str)
{
	if (str->magic == SS_MAGIC_8) return 1 * str->length + 1;
	if (str->magic == SS_MAGIC_16) return SS_C8_C16_WORST_RATIO * str->length + 1;
	if (str->magic == SS_MAGIC_32) return SS_C8_C32_WORST_RATIO * str->length + 1;
	return 0;
}

INLINE uint32_t ssg_wsize16(struct SimpleString* str)
{
	if (str->magic == SS_MAGIC_8) return SS_C16_C8_WORST_RATIO * str->length + 1;
	if (str->magic == SS_MAGIC_16) return 1 * str->length + 1;
	if (str->magic == SS_MAGIC_32) return SS_C16_C32_WORST_RATIO * str->length + 1;
	return 0;
}

INLINE uint32_t ssg_wsize32(struct SimpleString* str)
{
	if (str->magic == SS_MAGIC_8) return SS_C32_C8_WORST_RATIO * str->length + 1;
	if (str->magic == SS_MAGIC_16) return SS_C32_C16_WORST_RATIO * str->length + 1;
	if (str->magic == SS_MAGIC_32) return 1 * str->length + 1;
	return 0;
}

#undef INLINE

// -------------------------------------------------------------------------------------------

/*
	spooky time
*/

// ss_destroy
#define ssg_destroy(x) (_ssg_destroy)((struct SimpleString*)(x))

// ssg_compType
#define ssg_compType(x, y) (_ssg_compType)((struct SimpleString*)(x), (struct SimpleString*)(y))

// ssg_is[type]
#define ssg_isUTF8(x) (_ssg_isUTF8)((struct SimpleString*)(x))
#define ssg_isUTF16(x) (_ssg_isUTF16)((struct SimpleString*)(x))
#define ssg_isUTF32(x) (_ssg_isUCS4)((struct SimpleString*)(x))
#define ssg_isUCS4(x) (_ssg_isUCS4)((struct SimpleString*)(x))

// ssg_init
#define ssg_init8(x, y) (_ssg_init8)((x), (struct SimpleString*)(y))
#define ssg_init16(x, y) (_ssg_init16)((x), (struct SimpleString*)(y))
#define ssg_init32(x, y) (_ssg_init32)((x), (struct SimpleString*)(y))

// ssg_wsizer
#define ssg_wsize8(x) (_ssg_wsize8)((struct SimpleString*)(x))
#define ssg_wsize16(x) (_ssg_wsize16)((struct SimpleString*)(x))
#define ssg_wsize32(x) (_ssg_wsize32)((struct SimpleString*)(x))
