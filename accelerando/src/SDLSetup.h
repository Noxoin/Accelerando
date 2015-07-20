#ifndef SDLSETUP_H
#define	SDLSETUP_H

//Using standard IO, strings, SDL, and SDL image
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <dirent.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>



//Screen dimension constants
const int SCREEN_WIDTH = 1067;
const int SCREEN_HEIGHT = 600;


//Maximum axis velocity of the dot
const double DOT_VEL = 90;
//Initial status of the dot
const double INITIAL_POS_X = 950;
const double INITIAL_POS_Y = 10;
const double INITIAL_VEL = 0;
//Final status of the dot
const double FINAL_POS_Y = 460;
//Max distance in one press
const double MAX_DISTANCE = 90;
//Dot dimension constants
//const int DOT_WIDTH = 72;
//const int DOT_HEIGHT = 97;

const int SONGS_PER_PAGE = 6;


//The window we'll be rendering to
extern SDL_Window* gWindow;// = NULL;

//The window renderer
extern SDL_Renderer* gRenderer;// = NULL;

//Globally used font
extern TTF_Font *gFont;

#endif