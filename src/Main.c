#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>



// Must define this macro so MSVC doesn't explode
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "memoryAllocation.h"
#include "SDLApp.h"
#include "SDLButton.h"
#include "SDLDrawingArea.h"

#include "idxreader.h"
#include "neuralnetwork.h"


#define WEIGHTS_INIT_MIN -1.0
#define WEIGHTS_INIT_MAX 1.0
#define LEARNING_RATE 0.2
#define NEURAL_NETWORK_NUMBER_OF_LAYERS 2
#define NEURAL_NETWORK_LAYERS {30,10}
#define INPUT_SIZE 784


#define TRAIN_PATH_IMAGES "../data/train-images.idx3-ubyte"
#define TRAIN_PATH_LABELS "../data/train-labels.idx1-ubyte"
#define TEST_PATH_IMAGES "../data/t10k-images.idx3-ubyte"
#define TEST_PATH_LABELS "../data/t10k-labels.idx1-ubyte"





#define TITLE "Neural Network"
#define WINDOW_WIDTH 860
#define WINDOW_HEIGHT 640

#define BACKGROUND_COLOR 46,46,46,255

#define DRAWING_AREA_ZOOM 10
#define DRAWING_AREA_DIMENSION 28
#define DRAWING_AREA_SIDE_IN_PIXEL (DRAWING_AREA_ZOOM*DRAWING_AREA_DIMENSION)


int main(int argc, char **args)
{	


	// --------- INIT
//  once and done
	srand(time(NULL));
// ------
	SDL_Application *windowInfo;
	SDL_DrawingArea *drawingAreaInfo;

	SDL_Rect         digits_probability_rect;
	SDL_Texture     *digits_probability;


	uint32_t layersSizes[NEURAL_NETWORK_NUMBER_OF_LAYERS] = NEURAL_NETWORK_LAYERS;

	NeuralNetwork *network = NeuralNetwork_create_new_with_random_parameters(INPUT_SIZE,
																			 NEURAL_NETWORK_NUMBER_OF_LAYERS,
																			 layersSizes,
																			 WEIGHTS_INIT_MIN,
																			 WEIGHTS_INIT_MAX);
	if(network==NULL) return 1;

	
	Matrix3d_uint8 *learning_set_matrix;
	uint8_t        *learning_labels;
	
	Matrix3d_uint8 *testing_set_matrix;
	uint8_t        *testing_labels;


	// Used to convert temporary input from unsigned byte ranging from 0 to 255 to float ranging from 0.0 to 1.0
	float          inputAsFloat[INPUT_SIZE];


	// Getting the Learning and Training Data
	{
		int32_t fileReadingResult = 0;

		fileReadingResult = IDX_read_image_set_to_matrix3d_uint8(&learning_set_matrix,TRAIN_PATH_IMAGES);
		if(fileReadingResult!=IDX_READER_SUCCESS)
		{
			printf("Failed to open learning set\n");
			return 1;
		}


		fileReadingResult = IDX_read_label_set_to_array_uint8(&learning_labels,TRAIN_PATH_LABELS);
		if(fileReadingResult!=IDX_READER_SUCCESS)
		{
			Matrix3d_uint8_free(learning_set_matrix);
			printf("Failed to open learning set labels\n");
			return 1;
		}
		

		fileReadingResult = IDX_read_image_set_to_matrix3d_uint8(&testing_set_matrix,TEST_PATH_IMAGES);
		if(fileReadingResult!=IDX_READER_SUCCESS)
		{
			Matrix3d_uint8_free(learning_set_matrix);
			NN_free(learning_labels);
			printf("Failed to open testing set\n");
			return 1;
		}
		

		fileReadingResult = IDX_read_label_set_to_array_uint8(&testing_labels,TEST_PATH_LABELS);
		if(fileReadingResult!=IDX_READER_SUCCESS)
		{
			Matrix3d_uint8_free(learning_set_matrix);
			NN_free(learning_labels);
			Matrix3d_uint8_free(testing_set_matrix);
			printf("Failed to open testing set labels\n");
			return 1;
		}
	}
	
	// Initializing SDL stuffs
	{
		windowInfo = SDL_Application_create_new(TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		if(windowInfo==NULL) return 1;


		drawingAreaInfo = SDL_DrawingArea_create_new(0,
													 0,
													 DRAWING_AREA_DIMENSION,
													 DRAWING_AREA_ZOOM);
		if(drawingAreaInfo==NULL) return 1;

		
		digits_probability_rect.x = DIGITS_X;
		digits_probability_rect.y = 0;
		digits_probability        = LoadImage(windowInfo, DIGITS_PATH, &digits_probability_rect);
		if(digits_probability==NULL)
		{
			SDL_Application_Free(windowInfo);
			return 1;
		}
	}




	// Variables used in the loop for the interface
	SDL_Event event;
	int32_t   previous_mouse_x     = 0;
    int32_t   previous_mouse_y     = 0;
    int32_t   mouse_x              = 0;
    int32_t   mouse_y              = 0;
    bool      mouse_pressed        = false;
    bool      mouse_click          = false;


    bool      running              = true;


    float    *neuralNetwork_output = NULL;


// ------------------- MAINLOOP

	while (running)
	{
		mouse_click = false;

		uint64_t start = SDL_GetPerformanceCounter();

    	SDL_GetMouseState( &mouse_x, &mouse_y);

    	while (SDL_PollEvent(&event)) 
    	{

    		switch(event.type)
    		{
    			case SDL_KEYDOWN:
    			{
    				if(event.key.keysym.sym==SDLK_ESCAPE)
    				{
						running = false;
    				}
    			}
		        break;
    			case SDL_QUIT:
    			{
    				running = false;

    			}break;
    			case SDL_MOUSEBUTTONDOWN:
    			{
    				if(event.button.button == SDL_BUTTON_LEFT)
    				{
    					mouse_pressed  = true;
    					mouse_click    = true;
    				}
    			}break;
    			case SDL_MOUSEBUTTONUP:
    			{
    				if(event.button.button == SDL_BUTTON_LEFT)
    				{
    					mouse_pressed  = false;
    				}
    			}break;
    		}        
    	}


    	// Rendering
    	SDL_SetRenderDrawColor(windowInfo->pRenderer, BACKGROUND_COLOR);
    	SDL_Application_clear(windowInfo);
    	SDL_DrawingArea_draw(drawingAreaInfo, windowInfo->pRenderer);
    	SDL_RenderCopy(windowInfo->pRenderer, digits_probability, NULL, &digits_probability_rect);
    	if(neuralNetwork_output!=NULL)
    	{
    		SDL_Draw_List_probability_bar(windowInfo,
    			network->layers_sizes[network->layers_count -1], neuralNetwork_output);
    	}


    	// User Interface logic + Rendering  

		bool train  = SDL_immediate_button( windowInfo,
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){                50, 350, 150,  50},     // POSITION
											 (SDL_Rect){               224, 206, 191, 255});  // COLOR
		bool submit = SDL_immediate_button( windowInfo, 
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){               350, 200, 150,  50},      // POSITION
											 (SDL_Rect){               100, 100, 255, 255});   // COLOR
		bool clear  = SDL_immediate_button( windowInfo, 
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){ DRAWING_AREA_SIDE_IN_PIXEL,   0,  50, 50},     // POSITION
											 (SDL_Rect){                230, 99, 200, 255});   // COLOR
		bool pencil = SDL_immediate_button( windowInfo, 
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){ DRAWING_AREA_SIDE_IN_PIXEL,  50,  50, 50},     // POSITION
											 (SDL_Rect){                99, 99, 99, 255});     // COLOR
		bool eraser = SDL_immediate_button( windowInfo, 
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){ DRAWING_AREA_SIDE_IN_PIXEL, 100,  50, 50},      // POSITION
											 (SDL_Rect){                198, 198, 198, 255});   // COLOR
		bool smooth = SDL_immediate_button( windowInfo,
											 mouse_click, mouse_x, mouse_y, 
											 (SDL_Rect){ DRAWING_AREA_SIDE_IN_PIXEL, 150,  50, 50},      // POSITION
											 (SDL_Rect){                 100, 210,  120,255});  // COLOR

		if(SDL_DrawingArea_is_point_inside(drawingAreaInfo, mouse_x, mouse_y))
		{
			if(mouse_pressed)
			{
				if((previous_mouse_x!=mouse_x ||previous_mouse_y!=mouse_y))
				{
					int32_t x_area = mouse_x/drawingAreaInfo->multiplier;
					int32_t y_area= mouse_y/drawingAreaInfo->multiplier;
					SDL_DrawingArea_change_pixel(drawingAreaInfo, x_area, y_area);
				}

			}
		}
		else
		{
			if(train)
			{

				// --------------------------------------------------------  TRAINING AND TESTTING
				printf("\nStart of learning : -------- \n");

				uint64_t startTimer = SDL_GetPerformanceCounter();
				NeuralNetwork_train_with_uint8_array(network,
	    			Matrix3d_uint8_get_array2D_to_array(learning_set_matrix, 0),
	    			learning_labels,
	    			learning_set_matrix->dimension[0],
	    			LEARNING_RATE);

				uint64_t endTimer = SDL_GetPerformanceCounter();

				float elapsedTime = (endTimer - startTimer) / (float)SDL_GetPerformanceFrequency();

				printf("~~~~~~\nEnd of learning   : Learning duration was %f seconds\n\n", elapsedTime);

				printf("Start of testing  : -------- \n");

				float accuracy = NeuralNetwork_get_accuracy_on_test_set_uint8(network,
	    			Matrix3d_uint8_get_array2D_to_array(testing_set_matrix, 0),
	    			testing_labels,
	    			testing_set_matrix->dimension[0]);

				printf("End of testing    : Accuracy on testing set is %f \n\n", accuracy);

				// ------------------------------------------------------------------------------------
			}
			else if(submit)
			{
				NeuralNetwork_input_from_array_uint8(drawingAreaInfo->data,
													network->input_size,
													inputAsFloat);

				NeuralNetwork_feed_with_input(network,
												inputAsFloat);

	    		neuralNetwork_output = NeuralNetwork_get_output_value(network);
			}
			else if(clear)
			{
				SDL_DrawingArea_clear(drawingAreaInfo);
			}
			else if(pencil)
			{
				SDL_DrawingArea_pencil_mode(drawingAreaInfo);
			}
			else if(eraser)
			{
				SDL_DrawingArea_eraser_mode(drawingAreaInfo);
			}
			else if(smooth)
			{
				SDL_DrawingArea_average_value(drawingAreaInfo);
			}
		}


		
		previous_mouse_x = mouse_x;
		previous_mouse_y = mouse_y;

		SDL_Application_update(windowInfo);

		uint64_t end = SDL_GetPerformanceCounter();

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		if(elapsedMS < 16.666f)
		{
			// Cap to 60 FPS
			SDL_Delay((uint32_t)(floor(16.666f - elapsedMS)));
		}
	}



// ------------------- END

	printf("Closing ...\n");

	SDL_DestroyTexture(digits_probability);
	SDL_DrawingArea_free(drawingAreaInfo);
	SDL_Application_Free(windowInfo);

	NN_free(learning_labels);
	NN_free(testing_labels);
	Matrix3d_uint8_free(learning_set_matrix);
	Matrix3d_uint8_free(testing_set_matrix);
	NeuralNetwork_free(network);

	printf("Number of malloc not freed = %u\n", NN_get_total_malloc_count());

	return 0;


}
