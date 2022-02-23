#ifndef IDX_READER_H
#define IDX_READER_H
#include <stdint.h>
#include <stdbool.h>


/*
	Just enough code to read training set and test set into a "Matrix3d_uint8" struct
		wich is just a flatten 3D Tensor (all naming you'll find in this project sucks)
*/

// RETURN VALUE
#define IDX_READER_SUCCESS                   0
#define IDX_READER_FILE_NOT_FOUND            1
#define IDX_READER_FILE_READING_UNSUCCESSFUL 2
#define IDX_READER_ALLOCATION_FAILED         3


/*
	Requirements
*/
bool byte_order_is_little_endian(void); // it is needed to check if we need to swap the bytes when reading files


void int32_swap_bytes(int32_t *n);

typedef struct Matrix3d_uint8
{
	uint32_t dimension[3];

	uint8_t *data;

} Matrix3d_uint8;


Matrix3d_uint8 * Matrix3d_uint8_create_new(uint32_t dim1,uint32_t dim2,uint32_t dim3);


void Matrix3d_uint8_free(Matrix3d_uint8 *element);

#ifndef Matrix3d_uint8_get_value
#define Matrix3d_uint8_get_value(matrix, index1, index2, index3) (*(matrix->data + index1*matrix->dimension[1]*matrix->dimension[2]+ index2*matrix->dimension[2] +index3))
#endif

#ifndef Matrix3d_uint8_get_array2D_to_array
#define Matrix3d_uint8_get_array2D_to_array(matrix, index) (matrix->data + index*matrix->dimension[1]*matrix->dimension[2])
#endif

/*
	File reading
*/

int32_t IDX_read_image_set_to_matrix3d_uint8(Matrix3d_uint8 **matrix,char *filepath);


int32_t IDX_read_label_set_to_array_uint8(uint8_t **array_ptr,char *filepath);


#endif