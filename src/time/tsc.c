#include "tsc.h"

// these give high precision results on windows, so they should also work well on linux

#define TSC_DEFAULT_WARMUP_COUNT 128
#define TSC_DEFAULT_SAMPLE_COUNT 1024
#define TSC_DEFAULT_SAMPLE_DURATION 1000000
#define TSC_DEFAULT_DEVIATION 6

uint64_t tsc_frequency = 0;
double tsc_frequencyHz = 0.0;
double tsc_frequencyKHz = 0.0;
double tsc_frequencyMHz = 0.0;
double tsc_frequencyGHz = 0.0;

#ifdef _WIN32
	#include <windows.h>
#else
	#include <time.h>
#endif

#include <immintrin.h>
#include <x86intrin.h>

static uint64_t sample(uint64_t span_ns);
// swap for garbage sort function
static void swap(uint64_t* a, uint64_t* b)
{
	uint64_t tmp = *a;
	*a = *b;
	*b = tmp;
}

void tsc_calibrate()
{
	_tsc_calibrate(TSC_DEFAULT_WARMUP_COUNT, TSC_DEFAULT_SAMPLE_COUNT, TSC_DEFAULT_SAMPLE_DURATION, TSC_DEFAULT_DEVIATION);
}

#include <stdio.h>

void _tsc_calibrate(uint64_t warmup_count, uint64_t sample_count, uint64_t sample_duration, uint64_t deviation)
{
	uint64_t samples[sample_count + warmup_count];

	// add a warmup round of samples
	uint64_t s = sample_count + warmup_count;
	do {
		samples[--s] = sample(sample_duration);
	} while (s);

	// basic sort (should probably replace with something better, but eh, good enough)
	uint64_t start = __rdtsc();
	for (uint64_t i = 0; i < sample_count - 1; i++) {
		uint64_t midx = i;
		for (uint64_t j = i + 1; j < sample_count; j++) {
			if (samples[j] < samples[midx]) {
				midx = j;
			}
		}
		swap(&samples[midx], &samples[i]);
	}
	uint64_t end = __rdtsc();

	// choose range based on deviation field, higher deviation field = less deviation tolerated
	uint64_t devn0 = deviation;
	uint64_t devn1 = deviation + 1;
	uint64_t devd = deviation * 2 + 1;

	uint64_t avg = 0;
	for (uint64_t i = sample_count * devn0 / devd; i < sample_count * devn1 / devd; i++) {
		avg += samples[i];
	}
	avg /= (sample_count * devn1 / devd) - (sample_count * devn0 / devd);

	tsc_frequency = avg * (1000000000 / sample_duration);
	tsc_frequencyHz = (double)tsc_frequency;
	tsc_frequencyKHz = tsc_frequencyHz / 1000;
	tsc_frequencyMHz = tsc_frequencyKHz / 1000;
	tsc_frequencyGHz = tsc_frequencyMHz / 1000;

	// printf("took %lu ns to sort\n", (end-start) * 1000000000 / tsc_frequency);
}

static uint64_t sample(uint64_t span_ns)
{
	uint64_t tscStart, tscEnd;
	uint64_t tStart, tEnd;
	#ifdef _WIN32
		uint64_t pcFreq;
		QueryPerformanceFrequency((LARGE_INTEGER*)&pcFreq);
		span_ns = span_ns * pcFreq / (uint64_t)1000000000;
	#else
		struct timespec ts;
	#endif
	_mm_lfence();
	#ifdef _WIN32
		QueryPerformanceCounter((LARGE_INTEGER*)&tStart);
	#else
		clock_gettime(CLOCK_MONOTONIC, &ts);
		tStart = ts.tv_sec * 1000000000 + ts.tv_nsec;
	#endif
	tscStart = __rdtsc();
	do {
		#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&tEnd);
		#else
			clock_gettime(CLOCK_MONOTONIC, &ts);
			tEnd = ts.tv_sec * 1000000000 + ts.tv_nsec;
		#endif
	} while (tStart + span_ns > tEnd);
	tscEnd = __rdtsc();
	return tscEnd - tscStart;
}