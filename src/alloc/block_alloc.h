#pragma once

#include <stdint.h>

// NOT thread safe, but should be relatively easy to implement

// ~40x faster vs windows stdlib for bulk allocations without having to allocate new block lists
// ~20x faster vs windows stdlib for bulk allocations with having to allocate new block lists

// ~35x faster vs windows stdlib for bulk deallocations without having to deallocate free block lists
// ~10x faster vs windows stdlib for bulk deallocations with having to deallocate free block lists

struct BlockAlloc
{
	uint64_t block_size;
	uint64_t align;
	void* list_free;
	void* list_full;

	uint64_t free_count;
	uint64_t total_count;
	uint64_t free_list_count;
	uint64_t total_list_count;
};

typedef struct BlockAlloc block_alloc_t;

void block_alloc_init(block_alloc_t* alloc, uint64_t block_size);

void* block_malloc(block_alloc_t* alloc);
void block_free(block_alloc_t* alloc, void* addr);