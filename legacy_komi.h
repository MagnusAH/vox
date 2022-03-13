#pragma once

#include <stdint.h>

#ifndef KOMI_M128
#define KOMI_M128
inline void komi_m128(uint64_t i0, uint64_t i1, uint64_t* ol, uint64_t* oh)
{
	const __uint128_t n = (__uint128_t) i0 * i1;
	*ol = (uint64_t) n;
	*oh = (uint64_t) (n >> 64);
}
#endif

// compiler auto optimizes komi_m128 into almost the same assembly, but compiler can't optimize with inline assembly
#ifndef KOMI_M128
#define komi_m128( i0, i1, ol, oh ) __asm__("mul %[in1]" \
											  : [low] "=a" (ol), [high] "=d" (oh) \
											  : [in0] "a" (i0), [in1] "r" (i1) \
											  : )
#endif

// komihash adapted specifically for 32 bit inputs, seedless
inline uint64_t komi_hash32(uint64_t n)
{
	// init
	uint64_t seed1 = 0x243f6a8885a308d3;
	uint64_t seed5 = 0x452821e638d01377;

	uint64_t r1l, r1h, r2l, r2h;

	// seems to work fine without this block, but further testing for hash quality is required
	komi_m128(seed1, seed5, &r2l, &r2h);
	seed5 += r2h;
	seed1 = seed5 ^ r2l;
	//

	r2l = seed1;
	r2h = seed5;

	// 4 byte case
	r2l ^= (uint64_t)(1 << (n >> 31)) << 32 | n;

	// finish
	komi_m128(r2l, r2h, &r1l, &r1h);
	seed5 += r1h;
	seed1 = seed5 ^ r1l;
	komi_m128(seed1, seed5, &r2l, &r2h);

	return (seed5 + r2h) ^ r2l;
}

// komihash adapted specifically for 64 bit inputs, seedless, for coding reference
inline uint64_t komi_hash64(uint64_t n)
{
	// init
	uint64_t seed1 = 0x243f6a8885a308d3;
	uint64_t seed5 = 0x452821e638d01377;

	uint64_t r1l, r1h, r2l, r2h;

	// seems to work fine without this block, but further testing for hash quality is required
	komi_m128(seed1, seed5, &r2l, &r2h);
	seed5 += r2h;
	seed1 = seed5 ^ r2l;
	//

	r2l = seed1;
	r2h = seed5;

	// 8 byte case
	r2h ^= 1 << (n >> 63);
	r2l ^= n;

	// finish
	komi_m128(r2l, r2h, &r1l, &r1h);
	seed5 += r1h;
	seed1 = seed5 ^ r1l;
	komi_m128(seed1, seed5, &r2l, &r2h);

	return (seed5 + r2h) ^ r2l;
}