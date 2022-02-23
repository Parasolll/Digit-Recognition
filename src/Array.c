#include "Array.h" 
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "memoryAllocation.h"


/*
	This file is poorly named, it contains functions used to allocate and
		initialize arrays and matrices
*/


void Array_float_randomize(float *array, const uint32_t size, const float max , const float min)
{

	for(uint32_t i=0;i<size;i++)
	{
		array[i] = ((float) rand()/ (float)RAND_MAX)*(max-min) + min;
	}

}



void Matrix_float_randomize(float **matrix, const uint32_t rows,  const uint32_t columns,  const float max , const float min)
{

	for(uint32_t i=0;i<rows;i++)
	{
		Array_float_randomize(matrix[i],columns,max,min);
	}

}


float ** Allocate_Matrix_float(const uint32_t rows, const uint32_t columns)
{

	float **matrix = NN_malloc(sizeof(float*)*rows);
	if(matrix==NULL)
	{
		
		return NULL;
	}

	for(uint32_t i=0;i<rows;i++)
	{
		matrix[i] = NN_malloc(sizeof(float)*columns);
		if(matrix[i]==NULL)
		{
			for (int32_t j = i-1; j >= 0; j--)
			{
				
				NN_free(matrix[j]);
			}

			NN_free(matrix);
			return NULL;
		}
	}

	return matrix;

}

void Free_Allocated_Matrix_float(float **matrix, const uint32_t rows)
{

	for(uint32_t i=0;i<rows;i++)
	{
		NN_free(matrix[i]);
	}

	NN_free(matrix);

}