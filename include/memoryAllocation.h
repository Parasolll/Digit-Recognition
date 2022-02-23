#ifndef NN_memory_allocation_included
#define NN_memory_allocation_included
#include <stdlib.h>
#include <stdint.h>


/* 
	You must define NN_WEAK_CHECK_MEMORY_LEAK_MALLOC to check if all the memory allocated has been freed
		by checking if the total malloc count (malloc call that hasn't been followed by a free) is equal to 0
		This is a very weak check but i'm ok with that
*/
//#define NN_WEAK_CHECK_MEMORY_LEAK_MALLOC 1

#ifdef NN_WEAK_CHECK_MEMORY_LEAK_MALLOC
#define NN_malloc(x)    _NN_custom_malloc(x)
#define NN_free(x)      _NN_custom_free(x)
#else
#define NN_malloc(x)    malloc(x)
#define NN_free(x)      free(x)
#endif


void *_NN_custom_malloc(size_t x);


void  _NN_custom_free(void *x);


uint32_t NN_get_total_malloc_count(void);


#endif