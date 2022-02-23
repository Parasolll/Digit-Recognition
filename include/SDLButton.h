#ifndef SDLButton_h_included 
#define SDLButton_h_included
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDLApp.h"
#include <stdbool.h>



bool SDL_immediate_button(SDL_Application * app, bool clicking, int32_t mouseX, int32_t mouseY, SDL_Rect rect, SDL_Rect color);


#endif 
