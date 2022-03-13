#pragma once

#include <stdint.h>

// this is a bad layer of abstraction and should be replaced with platform specific macro allocator systems
void* native_alloc(uint64_t size);
void native_free(void* addr);