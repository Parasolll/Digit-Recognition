#ifndef NEURAL_NETWORK_H_INCLUDED
#define NEURAL_NETWORK_H_INCLUDED
#include <stdint.h>

/*
    Neurals networks works with uint8_t inputs
*/


/*
	Requirements
*/
void NeuralNetwork_input_from_array_uint8(uint8_t *input, const uint32_t size, float *output);

/*

	Neural Network

*/

typedef struct NeuralNetwork
{
	uint32_t input_size; // number of "neurons" of the "input layer"
	uint32_t layers_count; // number of layer, input is not included and the last one is the output

	// An array containning `layers_count` element 
	//  element i correspond to the number of neurons in the layer i
	//  note that input is not included and the last one is the output
	uint32_t *layers_sizes;

	float **neurons_value_in_layers; /* value always between 0 and 1 because of the sigmoid */
	float **bias_in_layers; /* can be every floating point value */

	// An array of matrices
	//  matrix i correspond to the weight matrix of the layer i
	//  note that input is not included and the last one is the output
	float ***weights_in_layers;

} NeuralNetwork;


 /* Converting to a floating point value between 0 and 1*/
#define NeuralNetwork_input_from_uint8(value) ((float)(value) / 255.f)
/* Get the output layer as floats array*/
#define NeuralNetwork_get_output_value(neural_network) (neural_network->neurons_value_in_layers[neural_network->layers_count-1])

NeuralNetwork *NeuralNetwork_create_new_with_random_parameters(uint32_t size_of_input,
																	uint32_t number_of_layers,
																	uint32_t *size_of_layers,
																	float init_weight_min,
																	float init_weight_max);
void NeuralNetwork_free(NeuralNetwork* neural_network);



uint32_t NeuralNetwork_get_max_layer_size(NeuralNetwork *network);

void NeuralNetwork_expected_output_neurons_from_uint8_output(const uint8_t single_output, const uint32_t size_output,float *output);

void NeuralNetwork_compute_z_for_layer(NeuralNetwork *neural_network,const uint32_t layer, float *input, float * output);


void NeuralNetwork_feed_with_input(NeuralNetwork *neural_network, float *input);

void NeuralNetwork_get_output_cost(NeuralNetwork *neural_network,const  uint8_t input_value, float *bufferHelper1, float* output);

void NeuralNetwork_backpropagate_gradient(NeuralNetwork *neural_network, float *input, float *cost_per_neuron, float *cost_per_neuron_next,const float learning_rate, float *bufferHelper);


void NeuralNetwork_train_with_uint8_array(NeuralNetwork *neural_network, uint8_t *input, uint8_t *input_label,const uint32_t number_of_training, const float learning_rate);

uint32_t NeuralNetwork_index_neuron_max_between_output(NeuralNetwork *neural_network);

float NeuralNetwork_is_output_correct(NeuralNetwork *neural_network, const uint8_t expected_output);

float NeuralNetwork_get_accuracy_on_test_set_uint8(NeuralNetwork *neural_network, uint8_t *input, uint8_t *input_value,const uint32_t number_of_test);

#endif