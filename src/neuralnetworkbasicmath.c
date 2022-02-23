#include "neuralnetworkbasicmath.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>



/*
	NOTE :
 		IN THIS FILE LOOPS HAVE BEEN WRITTEN IN SUCH WAY SO THE COMPILER CAN VECTORIZE IT EASILY
 		THIS IS NOT OPTIMIZED BUT AS A LEARNING/GARBAGE PROJECT THIS IS WAY MORE THAN SUFFICIENT FOR ME.
*/

static float sigmoid(float x)
{
	return (float) 1/ (1.f + expf(-x));
}

static float sigmoid_derivative(float x)
{
	
	return sigmoid(x) * ( 1 - sigmoid(x));
}

void Matrix_float_product_vector_float(float **matrix, float *vector, uint32_t matrix_rows, uint32_t matrix_columns, uint32_t vector_size, float *vector_output)
{

	assert(matrix_columns==vector_size);
	
	for(uint32_t i=0;i<matrix_rows;i++)
	{
		float sum = 0;
		for(uint32_t j=0;j<matrix_columns;j++)
		{
			sum += matrix[i][j]*vector[j];
		}
		vector_output[i]=sum;
	}
}

void Vector_float_sum(float *vector_dest, float *vector_two, uint32_t size)
{

	uint32_t i=0;
	const int max  = size - (size%4);
	const int max2 = size%4;

	for(i = 0; i<max; i+=4)
	{
		vector_dest[i]   = vector_dest[i]   + vector_two[i];
		vector_dest[i+1] = vector_dest[i+1] + vector_two[i+1];
		vector_dest[i+2] = vector_dest[i+2] + vector_two[i+2];
		vector_dest[i+3] = vector_dest[i+3] + vector_two[i+3];
	}

	for(i = 0; i<(max2); i++)
	{
		vector_dest[i+max]   = vector_dest[i+max]   + vector_two[i+max];
	}

}


void Vector_float_product(float * vector_one, float * vector_two, uint32_t size)
{

	uint32_t i=0;
	const int max  = size - (size%4);
	const int max2 = size%4;

	for(i = 0; i<max; i+=4)
	{
		vector_one[i]   = vector_one[i]   * vector_two[i];
		vector_one[i+1] = vector_one[i+1] * vector_two[i+1];
		vector_one[i+2] = vector_one[i+2] * vector_two[i+2];
		vector_one[i+3] = vector_one[i+3] * vector_two[i+3];
	}

	for(i = 0; i<(max2); i++)
	{
		vector_one[i+max]   = vector_one[i+max]   * vector_two[i+max];
	}

}

void sigmoid_vector_float(float *vector,uint32_t size)
{

	uint32_t i=0;
	const int max  = size - (size%4);
	const int max2 = size%4;


	for(i = 0; i<max; i+=4)
	{
		vector[i]   = sigmoid(vector[i]);
		vector[i+1] = sigmoid(vector[i+1]);
		vector[i+2] = sigmoid(vector[i+2]);
		vector[i+3] = sigmoid(vector[i+3]);
	}

	for(i = 0; i<(max2); i++)
	{
		uint32_t index = i + max;
		vector[index] = sigmoid(vector[index]);
	}
}


void sigmoid_derivative_vector_float(float *vector,uint32_t size)
{

	uint32_t i=0;
	const int max  = size - (size%4);
	const int max2 = size%4;

	for(i = 0; i<max; i+=4)
	{
		vector[i]   = sigmoid_derivative(vector[i]);
		vector[i+1] = sigmoid_derivative(vector[i+1]);
		vector[i+2] = sigmoid_derivative(vector[i+2]);
		vector[i+3] = sigmoid_derivative(vector[i+3]);
	}

	for(i = 0; i<(max2); i++)
	{
		uint32_t index = i + max;
		vector[index] = sigmoid_derivative(vector[index]);
	}
}

