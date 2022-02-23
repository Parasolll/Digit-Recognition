#include <stdlib.h>
#include <math.h>
#include <string.h>


#include "neuralnetwork.h"
#include "neuralnetworkbasicmath.h"
#include "Array.h"
#include "assert.h"


#include "memoryAllocation.h"

/*
  IMPORTANT TO KNOW :

	There can be still bugs in the code !! So don't be suprised if you find any.

  ALLOCATION ARE HIGHLY INEFFICIENT

	The goal of the project was to learn about Back Propagation by writing a tiny Neural Network

	There are rooms for improvment so feel free to make it better and sharing it

*/ 

NeuralNetwork *NeuralNetwork_create_new_with_random_parameters(uint32_t size_of_input,
																																uint32_t number_of_layers,
																																uint32_t *size_of_layers,
																																float init_weight_min,
																																float init_weight_max)
{

	NeuralNetwork * NN = NN_malloc(sizeof(NeuralNetwork));
	if (NN==NULL) return NULL;

	NN->input_size = size_of_input;
	NN->layers_count = number_of_layers;

	NN->layers_sizes = NN_malloc(sizeof(uint32_t)*number_of_layers);
	if(NN->layers_sizes==NULL)
	{
		NN_free(NN);
		return NULL;
	}

	for(uint32_t i=0;i<number_of_layers;i++)
	{

		NN->layers_sizes[i]=size_of_layers[i];

	}



	NN->neurons_value_in_layers = NN_malloc(sizeof(float*)*number_of_layers);
	if(NN->neurons_value_in_layers==NULL)
	{
		NN_free(NN->layers_sizes);
		NN_free(NN);
		return NULL;
	}

	for(uint32_t i=0;i<number_of_layers;i++)
	{

		NN->neurons_value_in_layers[i] = NN_malloc(sizeof(float) * size_of_layers[i]);
		if(NN->neurons_value_in_layers[i]==NULL)
		{
			for (int32_t j = i-1; j >= 0; j--)
			{
				NN_free(NN->neurons_value_in_layers[j]);
			}
			NN_free(NN->neurons_value_in_layers);
			NN_free(NN->layers_sizes);
			NN_free(NN);
			return NULL;
		}
		Array_float_init_with_zero(NN->neurons_value_in_layers[i],size_of_layers[i]);

	}

	NN->bias_in_layers = NN_malloc(sizeof(float*)*number_of_layers);
	if(NN->bias_in_layers==NULL)
	{
		for (uint32_t i = 0; i < NN->layers_count; ++i)
		{
			NN_free(NN->neurons_value_in_layers[i]);
		}
		NN_free(NN->neurons_value_in_layers);
		NN_free(NN->layers_sizes);
		NN_free(NN);

		return NULL;
	}

	for(uint32_t i=0;i<number_of_layers;i++)
	{
		NN->bias_in_layers[i] = NN_malloc(sizeof(float) * size_of_layers[i]);
		if(NN->bias_in_layers[i]==NULL)
		{
			for (int32_t j = i-1; j >= 0; j--)
			{
				NN_free(NN->bias_in_layers[j]);
			}
			for (uint32_t k = 0; k < NN->layers_count; ++k)
			{
				NN_free(NN->neurons_value_in_layers[k]);
			}	
			NN_free(NN->bias_in_layers);
			NN_free(NN->neurons_value_in_layers);
			NN_free(NN->layers_sizes);
			NN_free(NN);
			return NULL;
		}
		
		Array_float_init_with_zero(NN->bias_in_layers[i],size_of_layers[i]);
	}


	NN->weights_in_layers = NN_malloc(sizeof(float**)*number_of_layers);
	if(NN->weights_in_layers==NULL)
	{
		NN_free(NN->layers_sizes);

		for (uint32_t i = 0; i < NN->layers_count; ++i)
		{
			NN_free(NN->neurons_value_in_layers[i]);
			NN_free(NN->bias_in_layers[i]);
		}

		NN_free(NN->neurons_value_in_layers);
		NN_free(NN->bias_in_layers);
		NN_free(NN);

		return NULL;
	}

	for (uint32_t i = 0; i < number_of_layers; ++i)
	{	
		uint32_t rows = size_of_layers[i];
		uint32_t columns = (i==0) ? size_of_input : size_of_layers[i-1];

		NN->weights_in_layers[i] = Allocate_Matrix_float(rows,columns);

		if(NN->weights_in_layers[i]==NULL)
		{
			for (int32_t j = i-1; j >= 0; j--)
			{
				uint32_t frows = size_of_layers[j];
				Free_Allocated_Matrix_float(NN->weights_in_layers[j], frows);
			}

			NN_free(NN->weights_in_layers);
			NN_free(NN->layers_sizes);

			for (uint32_t i = 0; i < NN->layers_count; ++i)
			{
				NN_free(NN->neurons_value_in_layers[i]);
				NN_free(NN->bias_in_layers[i]);
			}

			NN_free(NN->neurons_value_in_layers);
			NN_free(NN->bias_in_layers);
			NN_free(NN);
		}
		
		Matrix_float_randomize(NN->weights_in_layers[i], rows, columns, init_weight_min, init_weight_max);

	}


	return NN;
}



uint32_t NeuralNetwork_get_max_layer_size(NeuralNetwork *network)
{

	uint32_t max =0;

	for (uint32_t i = 0; i < network->layers_count; ++i)
	{
		if(network->layers_sizes[i]>max) max = network->layers_sizes[i];
	}

	return max;
}


void NeuralNetwork_free(NeuralNetwork* neural_network)
{
	
	
	for (uint32_t i = 0; i < neural_network->layers_count; ++i)
	{
		NN_free(neural_network->neurons_value_in_layers[i]);
		
		NN_free(neural_network->bias_in_layers[i]);
		
		Free_Allocated_Matrix_float(neural_network->weights_in_layers[i],neural_network->layers_sizes[i]);
		
	}

	NN_free(neural_network->layers_sizes);

	NN_free(neural_network->neurons_value_in_layers);
	
	NN_free(neural_network->bias_in_layers);
	
	NN_free(neural_network->weights_in_layers);
	
	NN_free(neural_network);
	
}



void NeuralNetwork_input_from_array_uint8(uint8_t *input, const uint32_t size, float *output)
{

	for(uint32_t i=0;i<size;i++)
	{
		output[i] = NeuralNetwork_input_from_uint8(input[i]);
	}

}


// This function name can(strongly) be missleading
void NeuralNetwork_compute_z_for_layer(NeuralNetwork *neural_network,const uint32_t layer, float *input, float *output)
{
	if(layer==0)
	{

		Matrix_float_product_vector_float(neural_network->weights_in_layers[0],
										input,
										neural_network->layers_sizes[0],
										neural_network->input_size,
										neural_network->input_size,
										output);
	
		Vector_float_sum(output,
												neural_network->bias_in_layers[0],
												neural_network->layers_sizes[0]);
		
	}
	else
	{

		Matrix_float_product_vector_float(neural_network->weights_in_layers[layer],
												neural_network->neurons_value_in_layers[layer-1],
												neural_network->layers_sizes[layer],
												neural_network->layers_sizes[layer-1],
												neural_network->layers_sizes[layer-1],
												output);
		

		Vector_float_sum(output,
												neural_network->bias_in_layers[layer],
												neural_network->layers_sizes[layer]);
	}
}

void NeuralNetwork_feed_with_input(NeuralNetwork *neural_network, float *input)
{

	uint32_t first_layer_size = neural_network->layers_sizes[0];

	NeuralNetwork_compute_z_for_layer(neural_network, 0, input, neural_network->neurons_value_in_layers[0]);
	sigmoid_vector_float(neural_network->neurons_value_in_layers[0], first_layer_size);

	for (uint32_t i = 1; i < neural_network->layers_count;i++)
	{
		uint32_t layer_size = neural_network->layers_sizes[i];

		NeuralNetwork_compute_z_for_layer(neural_network, i, NULL, neural_network->neurons_value_in_layers[i]);
		sigmoid_vector_float(neural_network->neurons_value_in_layers[i], layer_size);
	}
}




void NeuralNetwork_expected_output_neurons_from_uint8_output(const uint8_t single_output, const uint32_t size_output, float *output)
{

	for (uint32_t neuron = 0; neuron < size_output; ++neuron)
	{
		output[neuron] = (single_output==neuron)? 1.f : 0.f;
	}
}




void NeuralNetwork_get_output_cost(NeuralNetwork *neural_network,const  uint8_t input_value, float *bufferHelper1, float* output)
{

	uint32_t output_size = neural_network->layers_sizes[neural_network->layers_count-1];

	NeuralNetwork_expected_output_neurons_from_uint8_output(input_value, output_size, bufferHelper1);


	for (uint32_t neuron = 0; neuron < output_size; ++neuron)
	{
		output[neuron] = 2 *(neural_network->neurons_value_in_layers[neural_network->layers_count-1][neuron] - bufferHelper1[neuron]);
	}
	
	Matrix_float_product_vector_float(neural_network->weights_in_layers[neural_network->layers_count-1],
											neural_network->neurons_value_in_layers[neural_network->layers_count-2],
											neural_network->layers_count-1,
											neural_network->layers_count-2,
											neural_network->layers_count-2,
											bufferHelper1);
	Vector_float_sum(bufferHelper1, neural_network->bias_in_layers[neural_network->layers_count-1], output_size);
	
	sigmoid_derivative_vector_float(bufferHelper1, output_size);


	Vector_float_product(output, bufferHelper1, output_size);

}


void NeuralNetwork_backpropagate_gradient(NeuralNetwork *neural_network, float *input, float *cost_per_neuron, float *cost_per_neuron_next,const float learning_rate, float *bufferHelper)
{

	// Note that cost_per_neuron contain the cost of the output layer

	const uint32_t maxLayerSize = NeuralNetwork_get_max_layer_size(neural_network);

	NeuralNetwork_compute_z_for_layer(neural_network, neural_network->layers_count-2, input, bufferHelper);
	sigmoid_derivative_vector_float(bufferHelper, neural_network->layers_sizes[neural_network->layers_count-2]);


	Array_float_init_with_zero(cost_per_neuron_next, maxLayerSize);

	for (uint32_t i = 0; i < neural_network->layers_sizes[neural_network->layers_count-1]; ++i)
	{
		neural_network->bias_in_layers[neural_network->layers_count-1][i] -= learning_rate * cost_per_neuron[i];
		
		for (uint32_t j = 0; j < neural_network->layers_sizes[neural_network->layers_count-2]; ++j)
		{

			cost_per_neuron_next[j] += neural_network->weights_in_layers[neural_network->layers_count-1][i][j] * cost_per_neuron[i];

			neural_network->weights_in_layers[neural_network->layers_count-1][i][j] -= learning_rate * cost_per_neuron[i] * neural_network->neurons_value_in_layers[neural_network->layers_count-2][j];

		}

	}

	Vector_float_product(cost_per_neuron_next, bufferHelper, neural_network->layers_sizes[neural_network->layers_count-2]);



	for (int32_t layer = neural_network->layers_count-2; layer >= 0; --layer)
	{
		uint32_t layer_minus_one_size = neural_network->layers_sizes[layer-1];
		Array_copy_bytes(cost_per_neuron_next, cost_per_neuron, neural_network->layers_sizes[layer]*sizeof(float));
		
		if(layer>0)
		{
			Array_float_init_with_zero(cost_per_neuron_next, maxLayerSize);
			NeuralNetwork_compute_z_for_layer(neural_network, layer-1, input, bufferHelper);
			sigmoid_derivative_vector_float(bufferHelper, neural_network->layers_sizes[layer-1]);
		}
		else
		{
			layer_minus_one_size = neural_network->input_size;
		}

		for (uint32_t i = 0; i < neural_network->layers_sizes[layer]; ++i)
		{

			neural_network->bias_in_layers[layer][i] -= learning_rate * cost_per_neuron[i];
			
			for (uint32_t j = 0; j < layer_minus_one_size; ++j)
			{
				if(layer>0)
				{
					cost_per_neuron_next[j] += neural_network->weights_in_layers[layer][i][j] * cost_per_neuron[i];

					neural_network->weights_in_layers[layer][i][j] -= learning_rate * cost_per_neuron[i] * neural_network->neurons_value_in_layers[layer-1][j];
				}
				else
				{

					neural_network->weights_in_layers[layer][i][j] -= learning_rate * cost_per_neuron[i] * input[j];

				}
			}

		}

		if(layer>0)
		{
			Vector_float_product(cost_per_neuron_next, bufferHelper, neural_network->layers_sizes[layer-1]);
		}

	}

}


void NeuralNetwork_train_with_uint8_array(NeuralNetwork *neural_network, uint8_t *input, uint8_t *input_label,const uint32_t number_of_training, const float learning_rate)
{
	float         *inputAsFloat     = NN_malloc(sizeof(float)*(neural_network->input_size));
	assert(inputAsFloat!=NULL);

	const uint32_t maxLayerSize     = NeuralNetwork_get_max_layer_size(neural_network);
	const uint32_t bufferHelperSize = maxLayerSize * 3;

	float         *baseAllocation   = NN_malloc(sizeof(float)*bufferHelperSize);
	assert(baseAllocation!=NULL);

	float         *helperBuffer1    = baseAllocation;
	float         *helperBuffer2    = baseAllocation +    maxLayerSize ;
	float         *helperBuffer3    = baseAllocation + (2*maxLayerSize);



	for (uint32_t i = 0; i < number_of_training; ++i)
	{

		NeuralNetwork_input_from_array_uint8(input,neural_network->input_size,inputAsFloat);
		
		NeuralNetwork_feed_with_input(neural_network, inputAsFloat);

		// note that helperBuffer 1 is used to pass the output layer's cost

		NeuralNetwork_get_output_cost(neural_network, *input_label, helperBuffer2, helperBuffer1);

		NeuralNetwork_backpropagate_gradient(neural_network, inputAsFloat, helperBuffer1, helperBuffer2, learning_rate, helperBuffer3);

		input+=neural_network->input_size;
		input_label++;
	}

	NN_free(baseAllocation);
	NN_free(inputAsFloat);
}


uint32_t NeuralNetwork_index_neuron_max_between_output(NeuralNetwork *neural_network)
{
	uint32_t index_max =0;

	for (uint32_t i = 0; i < neural_network->layers_sizes[neural_network->layers_count-1]; ++i)
	{
		if(neural_network->neurons_value_in_layers[neural_network->layers_count-1][i]>neural_network->neurons_value_in_layers[neural_network->layers_count-1][index_max])
		{
			index_max=i;
		}
	}
	return index_max;
}



float NeuralNetwork_is_output_correct(NeuralNetwork *neural_network, const uint8_t expected_output)
{

	uint32_t index = NeuralNetwork_index_neuron_max_between_output(neural_network);

	if(index == (uint32_t)expected_output)
	{
			return 1.0f;
	}

	return 0.0f;
}


float NeuralNetwork_get_accuracy_on_test_set_uint8(NeuralNetwork *neural_network, uint8_t *input, uint8_t *input_value, const uint32_t number_of_test)
{

	float *inputAsFloat = NN_malloc(sizeof(float)*(neural_network->input_size));
	assert(inputAsFloat!=NULL);
	float accuracy = 0;

	for (uint32_t i = 0; i < number_of_test; ++i)
	{

		NeuralNetwork_input_from_array_uint8(input,neural_network->input_size,inputAsFloat);

		NeuralNetwork_feed_with_input(neural_network, inputAsFloat);

		accuracy+= (float)NeuralNetwork_is_output_correct(neural_network, *input_value);

		input+= neural_network->input_size;
		input_value++;
	}

	accuracy /= (float)number_of_test;

	NN_free(inputAsFloat);

	return accuracy;
}