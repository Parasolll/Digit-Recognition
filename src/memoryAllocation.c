#include "memoryAllocation.h"
#include <assert.h>


/*
	This file contain a little trick to make sure 
		the number of malloc and free are equals
*/

static uint32_t _NN_malloc_count = 0;

void *_NN_custom_malloc(size_t x)
{
	_NN_malloc_count++;
	return malloc(x);
}


void  _NN_custom_free(void *x)
{
	assert(_NN_malloc_count>0);
	_NN_malloc_count--;
	free(x);
}


uint32_t NN_get_total_malloc_count(void)
{
	return _NN_malloc_count;
}
