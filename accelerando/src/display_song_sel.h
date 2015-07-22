#ifndef DISPLAY_SONG_SEL_H
#define	DISPLAY_SONG_SEL_H

#include "LTexture.h"
#include "Dot.h"
#include <SDL2/SDL_ttf.h>

using namespace std;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_ENTER,
	KEY_PRESS_SURFACE_TOTAL,
};

enum DotTexture
{
	DOT_TEXTURE_NOTE,
	DOT_TEXTURE_TOTAL,
};

int readFileNames();

void loadSelectScreenMedia(SDL_Renderer *renderer, TTF_Font *font, LTexture *gKeyPressSurfaces, LTexture *gDotTexture, LTexture *gSongName);

vector<string> getFileNames();

#endif

