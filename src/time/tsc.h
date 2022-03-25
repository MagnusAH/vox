#pragma once

#include <stdint.h>

extern uint64_t tsc_frequency;
extern double tsc_frequencyHz;
extern double tsc_frequencyKHz;
extern double tsc_frequencyMHz;
extern double tsc_frequencyGHz;

// use default parameters defined in tsc.c
void tsc_calibrate();

// real deviation is 1 / (deviation * 4 + 2)
void _tsc_calibrate(uint64_t warmup_count, uint64_t sample_count, uint64_t sample_duration, uint64_t deviation);

inline uint64_t rdtsc() {
	uint64_t rax, rdx;
	__asm__ volatile("rdtsc" : "=a" (rax), "=d" (rdx) :: );
	return (rdx << 32) | rax;
}

inline void lfence() {
	__asm__ volatile("lfence" ::: );
}

inline uint64_t tsc_toNano(uint64_t ticks) {
	return ticks * 1000000000 / tsc_frequency;
}

inline uint64_t tsc_toMicro(uint64_t ticks) {
	return ticks * 1000000 / tsc_frequency;
}

inline uint64_t tsc_toMilli(uint64_t ticks) {
	return ticks * 1000 / tsc_frequency;
}

inline uint64_t tsc_toSec(uint64_t ticks) {
	return ticks / tsc_frequency;
}

inline double tsc_toNanoF(double ticks) {
	return ticks / tsc_frequencyGHz;
}

inline double tsc_toMicroF(double ticks) {
	return ticks / tsc_frequencyMHz;
}

inline double tsc_toMilliF(double ticks) {
	return ticks / tsc_frequencyKHz;
}

inline double tsc_toSecF(double ticks) {
	return ticks / tsc_frequencyHz;
}