#pragma once

#include <x86intrin.h>

inline void memcpy(void* dest, void* src, uint32_t size)
{
	uint32_t i;
	// 32 byte load store
	for (i = 0; i < size - 31; i+=32) {
		_mm256_storeu_ps(&dest[i], _mm256_loadu_ps(&src[i]));
	}
	// 8 byte load store
	for (; i < size - 7; i+=8) {
		((uint64_t*)dest)[i] = ((uint64_t*)src)[i];
	}
	// 2 byte load store
	for (; i < size - 1; i+=2) {
		((uint16_t*)dest)[i] = ((uint16_t*)src)[i];
	}
	// 1 byte load store
	if (i < size) {
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
}