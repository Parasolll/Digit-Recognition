#include "SDLButton.h"
#include <SDL2/SDL.h>
#include <stdlib.h>


#define MY_FLOAT_CLAMP(x) (((x)>255.0f)? 255.0f : (x))

bool SDL_immediate_button(SDL_Application * app, bool clicking, int32_t mouseX, int32_t mouseY, SDL_Rect rect, SDL_Rect color)
{

	bool mouse_on = mouseX > rect.x &&  mouseX < (rect.x + rect.w) && mouseY > rect.y && mouseY < (rect.y + rect.h);
	float r,g,b,a;

	r = (float)color.x;
	g = (float)color.y;
	b = (float)color.w;
	a = (float)color.h;

	if(mouse_on)
	{

		r = MY_FLOAT_CLAMP((255.0f-r)*0.50f + r);
		g = MY_FLOAT_CLAMP((255.0f-g)*0.50f + g);
		b = MY_FLOAT_CLAMP((255.0f-b)*0.50f + b);
		if(clicking)
		{
			r = 250.0f;
			g = 250.0f;
			b = 250.0f;
		}
	}

	SDL_SetRenderDrawColor(app->pRenderer, (uint8_t)r, (uint8_t) g, (uint8_t) b, (uint8_t) a);

	SDL_RenderFillRect(app->pRenderer, &rect);

	return mouse_on && clicking;

}