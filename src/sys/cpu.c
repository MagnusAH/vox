#include "cpu.h"

#include <cpuid.h>

uint8_t cpu_checkTSCSupport()
{
	uint32_t eax, ebx, ecx, edx;

	// extended processor info and feature bits
	// https://en.wikipedia.org/wiki/CPUID
	__cpuid(0x80000001, eax, ebx, ecx, edx);

	uint8_t tsc = (edx >> 4) & 0x1;
	return tsc;
}

uint8_t cpu_checkTSCInvariant()
{
	uint32_t eax, ebx, ecx, edx;

	// advanced power management information
	__cpuid(0x80000007, eax, ebx, ecx, edx);

	uint8_t itsc = (edx >> 8) & 0x1;
	return itsc;
}