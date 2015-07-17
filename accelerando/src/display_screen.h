#ifndef DISPLAY_SCREEN_H
#define	DISPLAY_SCREEN_H

#include "LTexture.h"
#include "Dot.h"

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_ENTER,
	KEY_PRESS_SURFACE_TOTAL,
};
//	KEY_PRESS_SURFACE_UP,
//	KEY_PRESS_SURFACE_DOWN,
//	KEY_PRESS_SURFACE_LEFT,
//	KEY_PRESS_SURFACE_RIGHT,

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Run the application
void run();

#endif