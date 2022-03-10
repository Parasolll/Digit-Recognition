#include "SDLApp.h" 
#include <stdlib.h>
#include <math.h>


#include "memoryAllocation.h"



SDL_Application *SDL_Application_create_new(const char *title,
								const uint32_t window_width,
								const uint32_t window_height)
{
	SDL_Application *app_created = NN_malloc(sizeof(SDL_Application));

	if(app_created==NULL) return NULL;

	if(0!=SDL_Init(SDL_INIT_VIDEO))
	{
		NN_free(app_created);
		return NULL;
	}

	app_created->pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
										window_width, window_height, SDL_WINDOW_SHOWN);

	if(app_created->pWindow== NULL)
	{
		NN_free(app_created);
		SDL_Quit();
		return NULL;
	}


	app_created->pRenderer = SDL_CreateRenderer(app_created->pWindow,
													-1,
													SDL_RENDERER_SOFTWARE);

	if(app_created->pRenderer== NULL)
	{
		SDL_DestroyWindow(app_created->pWindow);
		NN_free(app_created);
		SDL_Quit();
		return NULL;
	}

	return app_created;
}

void SDL_Application_Free(SDL_Application *app)
{
	SDL_DestroyRenderer(app->pRenderer);
	SDL_DestroyWindow(app->pWindow);
	NN_free(app);
	SDL_Quit();
}

void SDL_Application_clear(SDL_Application *app)
{
	SDL_RenderClear(app->pRenderer);
}



SDL_Texture *LoadImage(SDL_Application *app, char * path, SDL_Rect *rect)
{

	SDL_Surface *temporary_surface = SDL_LoadBMP(path);
	if(temporary_surface==NULL)
	{
		return NULL;
	}

	rect->w = temporary_surface->clip_rect.w;
	rect->h = temporary_surface->clip_rect.h;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(app->pRenderer,temporary_surface);
	if(texture==NULL)
	{
		SDL_FreeSurface(temporary_surface);
		return NULL;
	}
	SDL_FreeSurface(temporary_surface);
	return texture;
}


void SDL_Draw_List_probability_bar(SDL_Application *app, int32_t size, float *probabilty)
{

	for (int32_t i = 0; i < size; ++i)
	{
		SDL_Draw_probability_bar(app, i, probabilty[i]);
	}

}

void SDL_Draw_probability_bar(SDL_Application *app, int32_t index, float probabilty)
{
	uint8_t value = (uint8_t)floor(probabilty*255.f);

	for (int i = 0; i < value; ++i)
    {
		SDL_SetRenderDrawColor(app->pRenderer, i, 0, 255-i, 255);
		for (int j = 0; j < BAR_HEIGHT; ++j)
		{
			SDL_RenderDrawPoint(app->pRenderer, BAR_STARTING_POINT_X + i, index*BAR_GAP + BAR_STARTING_POINT_Y +j);
		}
    }

}

void SDL_Application_update(SDL_Application *app)
{
	SDL_RenderPresent(app->pRenderer);
}

