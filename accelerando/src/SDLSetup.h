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
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;


//Maximum axis velocity of the dot
const double DOT_VEL = 70;
//Initial status of the dot
const double INITIAL_POS_X = 580;
const double INITIAL_POS_Y = 5;
const double INITIAL_VEL = 0;
//Final status of the dot
const double FINAL_POS_Y = 355;
//Max distance in one press
const double MAX_DISTANCE = 70;
//Dot dimension constants
//const int DOT_WIDTH = 72;
//const int DOT_HEIGHT = 97;

const int SONGS_PER_PAGE = 6;

#endif

