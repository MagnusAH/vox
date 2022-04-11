#include "unicode.h"

// convert utf-16 string with length 'length' to a null terminated utf-8 string returning the length
uint32_t unicode_utf16_to_utf8(uint8_t* dest, const uint16_t* src, uint32_t length)
{
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
}

// convert utf-32 string with length 'length' to a null terminated utf-8 string returning the length
uint32_t unicode_utf32_to_utf8(uint8_t* dest, const uint32_t* src, uint32_t length)
{
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
}

// convert utf-8 string with length 'length' to a null terminated utf-16 string returning the length
uint32_t unicode_utf8_to_utf16(uint16_t* dest, const uint8_t* src, uint32_t length)
{
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
}

// convert utf-32 string with length 'length' to a null terminated utf-16 string returning the length
uint32_t unicode_utf32_to_utf16(uint16_t* dest, const uint32_t* src, uint32_t length)
{
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
}

// convert utf-8 string with length 'length' to a null terminated utf-32 string returning the length
uint32_t unicode_utf8_to_utf32(uint32_t* dest, const uint8_t* src, uint32_t length)
{
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
}

// convert utf-16 string with length 'length' to a null terminated utf-32 string returning the length
uint32_t unicode_utf16_to_utf32(uint32_t* dest, const uint8_t* src, uint32_t length)
{
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
}