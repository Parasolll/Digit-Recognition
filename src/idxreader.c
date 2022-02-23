#include "idxreader.h"
#include <stdio.h>
#include <stdlib.h>


#include "memoryAllocation.h"

/*

	This file contains functions that read data used to trainand test the neural network

*/

bool byte_order_is_little_endian(void)
{
	int16_t one = 1; // writing 0x0001 in the memory
	// 00000000 00000001 = Big Endian, notice that the first byte is equal to 0
	// 00000001 00000000 = Little Endian, notice that the first byte is equal to 1
	int8_t *byte = (int8_t *)&one; // getting the adress of the first byte

	return *byte==1; // if the first byte is equal to 1, the less significant byte is the first one therefore we are facing little endian byte order
}

void int32_swap_bytes(int32_t *n)
{
	int32_t value = *n;
	*n = (value<<24 & 0xff000000) |
		 (value<<8  & 0x00ff0000) |
		 (value>>8  & 0x0000ff00) |
		 (value>>24 & 0x000000ff) ;
}

Matrix3d_uint8 * Matrix3d_uint8_create_new(uint32_t dim1,uint32_t dim2,uint32_t dim3)
{
	Matrix3d_uint8 * result = NN_malloc(sizeof(Matrix3d_uint8) + (dim1*dim2*dim3));

	if(result)
	{
		result->dimension[0] = dim1;
		result->dimension[1] = dim2;
		result->dimension[2] = dim3;
		result->data         = (((char *)result) + sizeof(Matrix3d_uint8));
	}

	return result;
}

void Matrix3d_uint8_free(Matrix3d_uint8 *element)
{
	NN_free(element);
}



int32_t IDX_read_image_set_to_matrix3d_uint8(Matrix3d_uint8 **matrix_ptr,char *filepath)
{

	FILE *pFile = fopen(filepath,"rb");
	if(pFile==NULL)
	{
		return IDX_READER_FILE_NOT_FOUND;
	}

	

	bool is_little_endian = byte_order_is_little_endian();
	uint32_t dimensions[3] = {0,0,0};
	size_t  result;

	fseek(pFile,4,SEEK_SET);

	result = fread(dimensions,sizeof(uint32_t),3,pFile);
	if(result!=3) 
	{
		fclose(pFile);
		return IDX_READER_FILE_READING_UNSUCCESSFUL;
	}
		


	if (is_little_endian)
	{
		for(uint32_t i=0;i<3;i++)
		{	
			
			int32_swap_bytes(dimensions+i);
			
		}
	}


	Matrix3d_uint8 *output = Matrix3d_uint8_create_new(dimensions[0], dimensions[1], dimensions[2]);
	if(!output)
	{
		fclose(pFile);
		return IDX_READER_ALLOCATION_FAILED;
	}
	
	result = fread(output->data,1,
							      output->dimension[0]*
							      output->dimension[1]*
							      output->dimension[2],
								  pFile);

	if(result!=output->dimension[0]*output->dimension[1]*output->dimension[2]) 
	{
		Matrix3d_uint8_free(output);
		fclose(pFile);
		return IDX_READER_FILE_READING_UNSUCCESSFUL;
	}

	
	
	fclose(pFile);

	*matrix_ptr = output;

	return IDX_READER_SUCCESS;
}


int32_t IDX_read_label_set_to_array_uint8(uint8_t **array_ptr,
											char *filepath)
{

	uint8_t *array;
	

	FILE *pFile = fopen(filepath,"rb");
	if(pFile==NULL) return IDX_READER_FILE_NOT_FOUND;


	bool is_little_endian = byte_order_is_little_endian();
	size_t result;
	int32_t dimension[1];

	fseek(pFile,4,SEEK_SET);

	result = fread(dimension,sizeof(int32_t),1,pFile);
	if(result!=1) 
	{
		fclose(pFile);
		return IDX_READER_FILE_READING_UNSUCCESSFUL;
	}
		
	

	if (is_little_endian)
	{
		int32_swap_bytes(dimension);
	}

	array = NN_malloc(sizeof(uint8_t)*dimension[0]);
	if(!array)
	{
		fclose(pFile);
		return IDX_READER_ALLOCATION_FAILED;
	}


	result = fread(array,sizeof(uint8_t),dimension[0],pFile);
	if(result!=dimension[0]) 
	{
		fclose(pFile);
		NN_free(array);
		return IDX_READER_FILE_READING_UNSUCCESSFUL;
	}
	
	*array_ptr = array;

	return IDX_READER_SUCCESS;
}

