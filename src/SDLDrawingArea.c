#include "SDLDrawingArea.h" 
#include <stdlib.h>


#include "memoryAllocation.h"

SDL_DrawingArea *SDL_DrawingArea_create_new(int32_t x, int32_t y,
											int32_t dimension, int32_t multiplier)
{
	const uint32_t bytes = dimension * dimension;
	SDL_DrawingArea *new = NN_malloc(sizeof(SDL_DrawingArea) + bytes);
	if(new==NULL) return NULL;

	new->dimension = dimension;
	new->x = x;
	new->y = y;
	new->multiplier = multiplier;
	new->state = SDL_DRAWING_AREA_STATE_PENCIL;
	new->data  = (void *)(new + 1);


	memset(new->data,0,bytes);

	return new;
}



void SDL_DrawingArea_free(SDL_DrawingArea *area)
{
	NN_free(area);
}

bool SDL_DrawingArea_is_point_inside(SDL_DrawingArea *area, int32_t x, int32_t y)
{
	return x > area->x &&  x < (area->x + area->dimension*area->multiplier) && y > area->y &&  y < (area->y + area->dimension*area->multiplier);
}


void SDL_DrawingArea_clear(SDL_DrawingArea *area)
{
	const int32_t bytes = area->dimension * area->dimension;
	memset(area->data,0,bytes);
}



void SDL_DrawingArea_draw(SDL_DrawingArea *area, SDL_Renderer *pRenderer)
{
	const uint32_t bytes  = area->dimension * area->dimension;
	const uint32_t offset = area->dimension * area->multiplier;

	for (int32_t i = 0; i < bytes; ++i)
	{
		uint8_t value = area->data[i];
		SDL_SetRenderDrawColor(pRenderer, value, value, value, 255);


		// Draw the normal sized one
		{
			SDL_RenderDrawPoint(pRenderer, (i%(area->dimension)) + offset, (i/(area->dimension))+ offset);
		}

		// Draw the magnified one
		{
			SDL_Rect rect = {(i%(area->dimension))*area->multiplier,
							 (i/(area->dimension))*area->multiplier,
							 area->multiplier,
							 area->multiplier};

			SDL_RenderFillRect(pRenderer,&rect);
		}
	}

}

#define MY_INT_CLAMP(x) (((x)>255)? 255 : ((x)<0)? 0 : (x))


void SDL_DrawingArea_average_value(SDL_DrawingArea *area)
{
	uint8_t copy_result[784];


	for(int i =0; i<(area->dimension*area->dimension);i++)
	{
		int32_t sum   = area->data[i];

		const int32_t x     = i%(area->dimension);
		const int32_t y     = i/(area->dimension);	

		int32_t index = 0;	

		// UP
		if(y > 0)
		{
			index = (y-1) * area->dimension +  x;
			sum  += area->data[index];
		}
		//LEFT
		if(x > 0)
		{
			index = (y) * area->dimension   +  x-1;
			sum  += area->data[index];
		}

		// DOWN
		if(y < (area->dimension-1))
		{
			index = (y+1) * area->dimension +  x;
			sum  += area->data[index];
		}

		//RIGHT
		if(x < (area->dimension-1))
		{
			index = y    * area->dimension  + (x+1);
			sum  += area->data[index];

		}

		// UP_LEFT
		if((y > 0) && (x > 0))
		{
			index = (y-1) * area->dimension +  x-1;
			sum  += area->data[index];

		}
		// UP_RIGHT
		if((y > 0) && (x < (area->dimension-1)))
		{
			index = (y-1) * area->dimension +  x+1;
			sum  += area->data[index];

		}

		// DOWN_RIGHT
		if((y < (area->dimension-1)) && (x < (area->dimension-1)))
		{
			index = (y+1) * area->dimension +  x+1;
			sum  += area->data[index];

		}
		// DOWN_LEFT
		if((y < (area->dimension-1)) && (x > 0))
		{
			index = (y+1) * area->dimension +  x-1;
			sum  += area->data[index];

		}



		copy_result[i] = (uint8_t)MY_INT_CLAMP(sum/9);
	}

	memcpy( area->data, copy_result, (area->dimension)*(area->dimension) );

}



void SDL_DrawingArea_eraser_mode(SDL_DrawingArea *area)
{
	area->state = SDL_DRAWING_AREA_STATE_ERASER;
} 


void SDL_DrawingArea_pencil_mode(SDL_DrawingArea *area)
{
	area->state = SDL_DRAWING_AREA_STATE_PENCIL;
} 

void SDL_DrawingArea_change_pixel(SDL_DrawingArea *area, int32_t x, int32_t y)
{
	const int32_t cursor_index     =  y    * area->dimension +  x;
	const int32_t right_index      =  y    * area->dimension + (x+1);
	const int32_t down_index       = (y+1) * area->dimension +  x;
	const int32_t right_down_index = (y+1) * area->dimension + (x+1);
	

	switch(area->state)
	{

		case SDL_DRAWING_AREA_STATE_ERASER:
		{
			int32_t current = area->data[cursor_index];

			area->data[cursor_index]         = 0;

			if(y < area->dimension-1)
			{
				current = area->data[down_index]; 
				area->data[down_index]       = 0;
			}
			
			if(x < area->dimension-1)
			{
				current = area->data[right_index];
				area->data[right_index]      = 0;
			}

			if((x < area->dimension-1) && (y < area->dimension-1))
			{
				area->data[right_down_index] = 0;
			}
		}break;

		case SDL_DRAWING_AREA_STATE_PENCIL:
		{
			const int32_t light_grey       =         87; 
			const int32_t       grey       =        123; 
	      	      uint8_t      color       = light_grey; 
	      	

			area->data[cursor_index]    = (uint8_t)MY_INT_CLAMP(area->data[cursor_index] + light_grey);

			if(y < area->dimension-1)
			{
				area->data[down_index]  = (uint8_t)MY_INT_CLAMP(area->data[down_index]   +       grey);
			}
			
			if(x < area->dimension-1)
			{
				area->data[right_index] = (uint8_t)MY_INT_CLAMP( area->data[right_index] +       grey);
			}


			if((x < area->dimension-1) && (y < area->dimension-1))
			{
				area->data[right_down_index] = 255;
			}
		}break;

	}
}

