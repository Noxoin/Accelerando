#ifndef DISPLAY_SONG_SEL_H
#define	DISPLAY_SONG_SEL_H

#include "LTexture.h"
#include "Dot.h"

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

void loadMedia(LTexture *gKeyPressSurfaces, LTexture *gDotTexture, LTexture *gSongName);

#endif