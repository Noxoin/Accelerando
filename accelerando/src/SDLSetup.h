#ifndef SDLSETUP_H
#define	SDLSETUP_H

//Using SDL, SDL image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



//Screen dimension constants
const int SCREEN_WIDTH = 1067;
const int SCREEN_HEIGHT = 600;

//Dot dimension constants
const int DOT_WIDTH = 72;
const int DOT_HEIGHT = 97;
//Maximum axis velocity of the dot
const int DOT_VEL = 3;//10;
//Initial status of the dot
const int INITIAL_POS_X = 600;
const int INITIAL_POS_Y = 0;
const int INITIAL_VEL = 0;
//Max distance in one press
const int MAX_DISTANCE = 100;

//The window we'll be rendering to
extern SDL_Window* gWindow;// = NULL;

//The window renderer
extern SDL_Renderer* gRenderer;// = NULL;

#endif