#ifndef NEURAL_NETWORK_BASIC_MATH_H_INCLUDED
#define NEURAL_NETWORK_BASIC_MATH_H_INCLUDED
#include <stdint.h>

/*
	Functions only use float
*/

void Matrix_float_product_vector_float(float ** matrix,
								float * vector,
								uint32_t matrix_rows,
								uint32_t matrix_columns,
								uint32_t vector_size,
								float * vector_output);

void Vector_float_sum(float *vector_dest,
							float *vector_two,
							uint32_t size);

void Vector_float_product(float * vector_one,
								float * vector_two,
								uint32_t size);

void sigmoid_vector_float(float *vector,uint32_t size);

void sigmoid_derivative_vector_float(float *vector,uint32_t size);


#endif