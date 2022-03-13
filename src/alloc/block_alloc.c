#include "block_alloc.h"

#define BLOCK_COUNT 255
#define LIST_ADDR_OFFSET (sizeof(block_list_t) + BLOCK_COUNT)

#include <string.h>

#ifdef _WIN32
	#include <malloc.h>
	#define i_malloc(size) _aligned_malloc(size, size)
	#define i_free(ptr) _aligned_free(ptr)
#else
	#include <stdlib.h>
	#define i_malloc(size) aligned_alloc(size, size)
	#define i_free(ptr) free(ptr)
#endif

#define TRACKING

struct BlockList
{
	void* addr; // = &BlockList +
	void* next;
	void* prev;
	uint64_t free_count;
	uint8_t free[BLOCK_COUNT + 1];
};

typedef struct BlockList block_list_t;

inline uint64_t ceil_pow2(uint64_t n)
{
	return (uint64_t)1 << (64 - __builtin_clzll(n - 1));
}

inline void* alloc_block(uint64_t align)
{
	return _aligned_malloc(align, align);
}

void block_alloc_init(block_alloc_t* alloc, uint64_t block_size)
{
	alloc->block_size = block_size;
	alloc->align = ceil_pow2(BLOCK_COUNT * block_size + sizeof(block_list_t));
	alloc->list_free = NULL;
	alloc->list_full = NULL;

	alloc->free_count = 0;
	alloc->total_count = 0;
	alloc->free_list_count = 0;
	alloc->total_list_count = 0;
}

void* block_malloc(block_alloc_t* alloc)
{
	block_list_t* list = alloc->list_free;

	if (__builtin_expect(!list, 0))
	{
		list = i_malloc(alloc->align);

		if (__builtin_expect(!list, 0)) {
			return NULL;
		}

		list->addr = (void*)((uint64_t)list + LIST_ADDR_OFFSET);
		list->next = alloc->list_free;
		list->prev = NULL;
		list->free_count = BLOCK_COUNT;

		for (uint8_t i = 0; i < BLOCK_COUNT; i++) {
			list->free[i] = i;
		}

		alloc->list_free = list;
		#ifdef TRACKING
			alloc->free_count += BLOCK_COUNT;
			alloc->total_count += BLOCK_COUNT;
			alloc->free_list_count++;
			alloc->total_list_count++;
		#endif
	}

	list->free_count--;
	#ifdef TRACKING
		alloc->free_count--;
	#endif

	// now full, remove from free list
	if (!list->free_count)
	{
		alloc->list_free = list->next;
		#ifdef LIST_FULL
			list->next = alloc->list_full;
			alloc->list_full->prev = list;
			alloc->list_full = list;
		#endif
		#ifdef TRACKING
			alloc->free_list_count--;
		#endif
	}

	return (void*)((uint64_t)list->addr + (uint64_t)list->free[list->free_count] * alloc->block_size);
}

// won't bother adding handling for an invalid address
void block_free(block_alloc_t* alloc, void* addr)
{
	block_list_t* list = (void*)((uint64_t)addr & (~(alloc->align - 1)));

	#ifdef TRACKING
		alloc->free_count++;
	#endif

	// in full list, move to free list
	#ifdef LIST_FULL
		if (!list->free_count)
		{
			if (list->next) {
				((block_list_t*)list->next)->prev = list->prev;
			}
			if (list->prev) {
				((block_list_t*)list->prev)->next = list->next;
				list->prev = NULL;
			}

			alloc->free_list_count++;
		}
	#endif

	if (list->prev) {
		((block_list_t*)list->prev)->next = list->next;
		list->prev = NULL;
	}

	// keep hot block_lists at the front of the free list
	if (alloc->list_free != list) {
		list->next = alloc->list_free;
		((block_list_t*)alloc->list_free)->prev = list;
		alloc->list_free = list;
	}

	list->free[list->free_count++] = ((uint64_t)addr - (uint64_t)list) / alloc->block_size;

	// if (list->free_count == BLOCK_COUNT) {
	// 	alloc->list_free = list->next;
	// 	alloc->free_list_count--;
	// 	alloc->total_list_count--;
	// 	if (alloc->list_free) {
	// 		((block_list_t*)alloc->list_free)->prev = NULL;
	// 	}
	// 	i_free(list);
	// }
}