#include "native_alloc.h"

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <memoryapi.h>
#else
	TODO
#endif

void* native_alloc(uint64_t size)
{
	#ifdef _WIN32
		return VirtualAllocEx(GetCurrentProcess(), NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	#else
		TODO
	#endif
}

void native_free(void* addr)
{
	#ifdef _WIN32
		VirtualFree(addr, 0, MEM_RELEASE);
	#else
		TODO
	#endif
}