#ifndef SDLApp_h_included 
#define SDLApp_h_included 
#include <SDL2/SDL.h>
#include <stdint.h>

// SOME HARDCODED GARBAGES FOR THE INTERFACE
#define DIGITS_PATH "../ressources/barre.bmp"
#define DIGITS_X 544
#define BAR_STARTING_POINT_X DIGITS_X + 15
#define BAR_STARTING_POINT_Y 38
#define BAR_HEIGHT 10
#define BAR_GAP 32
#define LOADING_BAR_COLOR 0,0,255,255
#define LOADING_BAR_Y 550

typedef struct SDL_Application
{
	
	SDL_Window *pWindow;
	SDL_Renderer *pRenderer;

} SDL_Application;


SDL_Application *SDL_Application_create_new(const char *title,
								const uint32_t window_width,
								const uint32_t window_height);

void SDL_Application_Free(SDL_Application *app);

void SDL_Application_clear(SDL_Application *app);



SDL_Texture *LoadImage(SDL_Application *app, char * path, SDL_Rect *rect);

void SDL_Draw_List_probability_bar(SDL_Application *app, int32_t size, float *probabilty);

void SDL_Draw_probability_bar(SDL_Application *app, int32_t index, float probabilty);



void SDL_Application_update(SDL_Application *app);



#endif 
