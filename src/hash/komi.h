#pragma once

#include <stdint.h>

/*
	algorithm and generic code at
	https://github.com/avaneev/komihash
*/

// komihash for 32 bit inputs, seedless
// on clang 13.0.1 => 6x mov, 3x xor, 3x shift, 2x mul, 2x add, 1x or
// number of mov instructions varies slightly based on where it's inlined
inline uint64_t komi_hash32(uint32_t n)
{
	__uint128_t large;

    large = (__uint128_t) (0x01d2ee0ae40a48dc ^ ((uint64_t)(1 << (n >> 31)) << 32 | n)) * 0x4ef2e8526fea8bc9;
    uint64_t hash = 0x4ef2e8526fea8bc9 + (large >> 64);
    large = (__uint128_t) (hash ^ (uint64_t)large) * hash;

    return (hash + (large >> 64)) ^ (uint64_t)large;
}

// komihash for 64 bit inputs, seedless
// on clang 13.0.1 => 6x mov, 4x xor, 2x shift, 2x mul, 2x add
// number of mov instructions varies slightly based on where it's inlined
inline uint64_t komi_hash64(uint64_t n)
{
	__uint128_t large;

    large = (__uint128_t) (0x01d2ee0ae40a48dc ^ n) * (0x4ef2e8526fea8bc9 ^ (1 << (n >> 63)));
    uint64_t hash = 0x4ef2e8526fea8bc9 + (large >> 64);
    large = (__uint128_t) (hash ^ (uint64_t)large) * hash;

    return (hash + (large >> 64)) ^ (uint64_t)large;
}