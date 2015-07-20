extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include "../src/midi_reader.h"
}

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "playscreen.h"


const int bar_Height = 80;
const int song_MaxWidth = 10000;

const int bitMapCellW = 75;
const int bitMapCellH = 125;

SDL_Rect copy;
SDL_Rect paste;
SDL_Rect draw;

//Screen dimension
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

//Symbols image
SDL_Surface* gSymbolsSurface = NULL;

//Trace... ON!
bool loadMedia(SDL_Surface* gScreenSurface)
{
	SCREEN_WIDTH = gScreenSurface->w;
	SCREEN_HEIGHT = gScreenSurface->h;
	
	//Loading success flag
	bool success = true;	

	//Load symbol surface
	gSymbolsSurface = loadSurface( "../res/symbolsMap.bmp" , gScreenSurface);
	if( gSymbolsSurface == NULL )
	{
		printf( "Failed to load symbols image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gSymbolsSurface );
	gSymbolsSurface = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

//Defining Composition
SDL_Surface* loadSurface( std::string path , SDL_Surface* gScreenSurface)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}


void CopySymbolCellToSurface(int row, int col, int x, int y, SDL_Surface* gBlitingSurface)
{
	copy.x = (col-1)*bitMapCellW;
	copy.y = (row-1)*bitMapCellH;
	copy.w = bitMapCellW;
	copy.h = bitMapCellH;

	paste.x = x;
	paste.y = y;
	paste.w = bitMapCellW;
	paste.h = bitMapCellH;

	SDL_BlitSurface( gSymbolsSurface, &copy, gBlitingSurface, &paste );
}

//Set
void SetPlayscreenBackground (SDL_Surface* gScreenSurface, SDL_Surface* gBlitingSurface)
{
	copy.x = 0;
	copy.y = SCREEN_HEIGHT;
	copy.w = SCREEN_WIDTH;
	copy.h = SCREEN_HEIGHT;

	paste.x = 0;
	paste.y = 0;
	paste.w = SCREEN_WIDTH;
	paste.h = SCREEN_HEIGHT;
	
	SDL_BlitScaled( gBlitingSurface, &copy, gScreenSurface, &paste );
}

//Projecting Image
SDL_Surface* CreateBlitingSurface (Song* song, SDL_Surface* gBlitingSurface)
{
	int xRender = SCREEN_WIDTH;
	int barWidth = bitMapCellW*(song->bars[0].tsig_numerator*8/song->bars[0].tsig_denominator);
	int eigthChain = 0;
	int restDuration = 0;

	Note prevNote;
	Note currNote;
	Note nextNote;

	//Create RGB Surface
	gBlitingSurface = SDL_CreateRGBSurface(0,song_MaxWidth, 2*SCREEN_HEIGHT,32,0,0,0,0);
	SDL_FillRect(gBlitingSurface,NULL, SDL_MapRGB (gBlitingSurface->format, 255,255,255));;

	//Apply clef onto bliting background surface
	copy.x = 0;
	copy.y = 2*bitMapCellH;
	copy.w = bitMapCellW;
	copy.h = 2*bitMapCellH;

	paste.x = 0;
	paste.y = SCREEN_HEIGHT/2-bitMapCellH+SCREEN_HEIGHT;
	paste.w = bitMapCellW;
	paste.h = 2*bitMapCellH;

	SDL_BlitSurface( gSymbolsSurface, &copy, gBlitingSurface, &paste );

	//Apply time sig onto bliting background surface
	CopySymbolCellToSurface(5, song->bars[0].tsig_numerator, bitMapCellW, SCREEN_HEIGHT/2-bitMapCellH/2+SCREEN_HEIGHT, gBlitingSurface);
	CopySymbolCellToSurface(5, song->bars[0].tsig_denominator, bitMapCellW, SCREEN_HEIGHT/2+SCREEN_HEIGHT, gBlitingSurface);

	//Apply key sig onto bliting background surface (TODO)

	//Apply notes onto bliting play surface
	for (int bar = 0; bar < song->length; bar++)
	{
		draw.x = xRender - 5;
		draw.y = SCREEN_HEIGHT/2 - bitMapCellH/4;
		draw.w = 2;
		draw.h = bitMapCellH/2;
		SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));

		//bars with only rests
		if (song->bars[bar].length == 0)
		{
			int fullRests = (song->bars[bar].tsig_numerator * 8/song->bars[bar].tsig_denominator)/8;
			int lastRestDuration = (song->bars[bar].tsig_numerator * 8/song->bars[bar].tsig_denominator)%8;

			for (int i = 0; i < fullRests; i++ )
			{
				CopySymbolCellToSurface(2, 8, xRender + bitMapCellW*i*8, SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
			}
			if (lastRestDuration != 0)
			{
				CopySymbolCellToSurface(2, lastRestDuration, xRender + bitMapCellW*fullRests*8, SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
			}			
		} 
		else
		{
			//if the bar begins with a rest
			restDuration = song->bars[bar].notes[0].time;
			if (restDuration != 0)
			{
				CopySymbolCellToSurface(2, restDuration, xRender + bitMapCellW*currNote.time, SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
			}

			for (int note = 0; note < song->bars[bar].length; note++)
			{
				prevNote = song->bars[bar].notes[note-1];
				currNote = song->bars[bar].notes[note];
				nextNote = song->bars[bar].notes[note+1];
				restDuration = nextNote.time - currNote.duration - currNote.time;

				if (currNote.duration == 1 && ((nextNote.duration == 1 && nextNote.time == currNote.time+1) || eigthChain != 0))
				{
					CopySymbolCellToSurface(1,2, xRender + bitMapCellW*currNote.time, SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
					if (prevNote.duration == 1 && eigthChain < 3)
					{
						draw.x = xRender + bitMapCellW*currNote.time - bitMapCellW/2;
						draw.y = SCREEN_HEIGHT/2 - bitMapCellH/2;
						draw.w = bitMapCellW;
						draw.h = 2;
						SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));
						eigthChain++;
					}
					else
					{
						eigthChain=0;
					}
				}
				else
				{
					CopySymbolCellToSurface(1,currNote.duration, xRender + bitMapCellW*currNote.time, SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
					eigthChain=0;
				}
				
				if (restDuration > 0)
				{
					CopySymbolCellToSurface(2, restDuration, xRender + bitMapCellW*(currNote.time+currNote.duration), SCREEN_HEIGHT/2 - bitMapCellH/2, gBlitingSurface);
					eigthChain=0;
				}
			}
		}
		eigthChain=0;
		xRender = xRender + barWidth;
	}
	//Draw the 2 lines at end of score
	draw.x = xRender;
	draw.y = SCREEN_HEIGHT/2 - bitMapCellH/4;
	draw.w = 2;
	draw.h = bitMapCellH/2;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));

	draw.x = xRender + 4;
	draw.y = SCREEN_HEIGHT/2 - bitMapCellH/4;
	draw.w = 3;
	draw.h = bitMapCellH/2;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));
	
	//Draw the line going through the notes
	draw.x = 0;
	draw.y = SCREEN_HEIGHT/2 - 1;
	draw.w = xRender + 5;
	draw.h = 3;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));

	return gBlitingSurface;
}

//Nine Lives Blade Works
void UpdatePlaySurface (SDL_Surface* gScreenSurface, SDL_Surface* gBlitingSurface, int xCoord)
{
	//Blit notes onto windows
	copy.x = xCoord;
	copy.y = SCREEN_HEIGHT/4;
	copy.w = SCREEN_WIDTH - 2*bitMapCellW;
	copy.h = SCREEN_HEIGHT/2;

	paste.x = 2*bitMapCellW;
	paste.y = SCREEN_HEIGHT/4;
	paste.w = SCREEN_WIDTH - 2*bitMapCellW;
	paste.h = SCREEN_HEIGHT/2;

	//Update the surface
	SDL_BlitSurface( gBlitingSurface, &copy, gScreenSurface, &paste );

	//Draw the line marking where the guy is supposed to hit
	draw.x = 3.5*bitMapCellW;
	draw.y = 0;
	draw.w = 3;
	draw.h = SCREEN_HEIGHT;
	SDL_FillRect(gScreenSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 100,50,180));

}
