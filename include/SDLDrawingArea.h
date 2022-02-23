#ifndef SDLDrawingArea_h_included
#define SDLDrawingArea_h_included
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>


typedef enum
{
	SDL_DRAWING_AREA_STATE_ERASER,
	SDL_DRAWING_AREA_STATE_PENCIL	

} SDL_DRAWING_AREA_STATE;

typedef struct SDL_DrawingArea
{
	int32_t x;
	int32_t y;
	int32_t dimension;
	int32_t multiplier;
	SDL_DRAWING_AREA_STATE state;
	uint8_t *data;
} SDL_DrawingArea;


SDL_DrawingArea *SDL_DrawingArea_create_new(int32_t x, int32_t y,
											int32_t dimension, int32_t multiplier);


void SDL_DrawingArea_free(SDL_DrawingArea *area);


bool SDL_DrawingArea_is_point_inside(SDL_DrawingArea *area, int32_t x, int32_t y);

void SDL_DrawingArea_clear(SDL_DrawingArea *area);

void SDL_DrawingArea_average_value(SDL_DrawingArea *area);

void SDL_DrawingArea_eraser_mode(SDL_DrawingArea *area);

void SDL_DrawingArea_pencil_mode(SDL_DrawingArea *area);

void SDL_DrawingArea_draw(SDL_DrawingArea *area, SDL_Renderer *pRenderer);

void SDL_DrawingArea_change_pixel(SDL_DrawingArea *area, int32_t x, int32_t y);


#endif
