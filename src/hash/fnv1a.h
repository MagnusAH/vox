#pragma once

#include <stdint.h>

#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

inline uint64_t fnv1a_hash(void* bytes, uint64_t count)
{
	uint64_t hash = FNV_OFFSET_BASIS;

	while (count--) {
		hash ^= *(uint8_t*)bytes++;
		hash *= FNV_PRIME;
	}

	return hash;
}

inline uint64_t fnv1a_hash32(uint32_t n)
{
	uint64_t hash = FNV_OFFSET_BASIS;

	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n;
	hash *= FNV_PRIME;

	return hash;
}

inline uint64_t fnv1a_hash64(uint64_t n)
{
	uint64_t hash = FNV_OFFSET_BASIS;

	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n & 0xff;
	hash *= FNV_PRIME;
	n >>= 8;
	hash ^= n;
	hash *= FNV_PRIME;

	return hash;
}