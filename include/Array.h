#ifndef Array_h_included
#define Array_h_included
#include <stdint.h>
#include <stddef.h>

/*
	This file is poorly named, it contains functions used to allocate and
		initialize arrays and matrices

	Functions here DO NOT CALL SRAND() 
	srand() is called once in main function inside Main.c
*/


#ifndef Array_float_init_with_zero
#define Array_float_init_with_zero(array, size) memset(array, 0, size*sizeof(float))
#endif

#ifndef Array_uint32_init_with_zero
#define Array_uint32_init_with_zero(array, size) memset(array, 0, size*sizeof(uint32_t));
#endif

#ifndef Array_copy_bytes
#define Array_copy_bytes(src, dest, size_in_byte) memcpy(dest, src, size_in_byte);
#endif

void Array_float_randomize(float *array, const uint32_t size, const float max , const float min);

void Matrix_float_randomize(float **matrix, const uint32_t rows, const uint32_t columns, const float max , const float min);


float ** Allocate_Matrix_float(const uint32_t rows, const uint32_t columns);
void Free_Allocated_Matrix_float(float **matrix, const uint32_t rows);

#endif 
