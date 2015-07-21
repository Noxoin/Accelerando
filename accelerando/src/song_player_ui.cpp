#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "song_player.h"


//const int bar_Height = 80;
const int song_MaxWidth = 10000;

const int bitMapCellW = 75;
const int bitMapCellH = 125;

SDL_Rect copy;
SDL_Rect paste;
SDL_Rect draw;

//Screen dimension

void copySymbolCellToSurface(Image image, int row, int col, int x, int y, SDL_Surface* gBlitingSurface)
{
	copy.x = (col-1)*bitMapCellW;
	copy.y = (row-1)*bitMapCellH;
	copy.w = bitMapCellW;
	copy.h = bitMapCellH;

	paste.x = x;
	paste.y = y;
	paste.w = bitMapCellW;
	paste.h = bitMapCellH;

	SDL_BlitSurface( image.getSymbolsMap(), &copy, gBlitingSurface, &paste );
}

//Set
void SongPlayer::setPlayscreenBackground (Image image, SDL_Surface* gScreenSurface)
{
	copy.x = 0;
	copy.y = image.screen_height;
	copy.w = image.screen_width;
	copy.h = image.screen_height;

	paste.x = 0;
	paste.y = 0;
	paste.w = image.screen_width;
	paste.h = image.screen_height;
	
	SDL_BlitScaled( musicSheetSurface, &copy, gScreenSurface, &paste );
}

//Projecting Image
SDL_Surface* SongPlayer::createMusicSurface (Image image, Song* song)
{
        screen_width = image.screen_width;
        screen_height = image.screen_height;
	int xRender = screen_width;
	int barWidth = bitMapCellW*(song->bars[0].tsig_numerator*8/song->bars[0].tsig_denominator);
	int eigthChain = 0;
	int restDuration = 0;

	Note prevNote;
	Note currNote;
	Note nextNote;

	//Create RGB Surface
	SDL_Surface *gBlitingSurface = SDL_CreateRGBSurface(0,song_MaxWidth, 2*screen_height,32,0,0,0,0);
	SDL_FillRect(gBlitingSurface,NULL, SDL_MapRGB (gBlitingSurface->format, 255,255,255));;

	//Apply clef onto bliting background surface
	copy.x = 0;
	copy.y = 2*bitMapCellH;
	copy.w = bitMapCellW;
	copy.h = 2*bitMapCellH;

	paste.x = 0;
	paste.y = screen_height/2-bitMapCellH+screen_height;
	paste.w = bitMapCellW;
	paste.h = 2*bitMapCellH;

	SDL_BlitSurface( image.getSymbolsMap(), &copy, gBlitingSurface, &paste );

	//Apply time sig onto bliting background surface
	copySymbolCellToSurface(image, 5, song->bars[0].tsig_numerator, bitMapCellW, screen_height/2-bitMapCellH/2+screen_height, gBlitingSurface);
	copySymbolCellToSurface(image, 5, song->bars[0].tsig_denominator, bitMapCellW, screen_height/2+screen_height, gBlitingSurface);

	//Apply key sig onto bliting background surface (TODO)

	//Apply notes onto bliting play surface
	for (int bar = 0; bar < song->length; bar++)
	{
		draw.x = xRender - 5;
		draw.y = screen_height/2 - bitMapCellH/4;
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
				copySymbolCellToSurface(image, 2, 8, xRender + bitMapCellW*i*8, screen_height/2 - bitMapCellH/2, gBlitingSurface);
			}
			if (lastRestDuration != 0)
			{
				copySymbolCellToSurface(image, 2, lastRestDuration, xRender + bitMapCellW*fullRests*8, screen_height/2 - bitMapCellH/2, gBlitingSurface);
			}			
		} 
		else
		{
			//if the bar begins with a rest
			restDuration = song->bars[bar].notes[0].time;
			if (restDuration != 0)
			{
				copySymbolCellToSurface(image, 2, restDuration, xRender + bitMapCellW*currNote.time, screen_height/2 - bitMapCellH/2, gBlitingSurface);
			}

			for (int note = 0; note < song->bars[bar].length; note++)
			{
				prevNote = song->bars[bar].notes[note-1];
				currNote = song->bars[bar].notes[note];
				nextNote = song->bars[bar].notes[note+1];
				restDuration = nextNote.time - currNote.duration - currNote.time;

				if (currNote.duration == 1 && ((nextNote.duration == 1 && nextNote.time == currNote.time+1) || eigthChain != 0))
				{
					copySymbolCellToSurface(image, 1,2, xRender + bitMapCellW*currNote.time, screen_height/2 - bitMapCellH/2, gBlitingSurface);
					if (prevNote.duration == 1 && eigthChain < 3)
					{
						draw.x = xRender + bitMapCellW*currNote.time - bitMapCellW/2;
						draw.y = screen_height/2 - bitMapCellH/2;
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
					copySymbolCellToSurface(image, 1,currNote.duration, xRender + bitMapCellW*currNote.time, screen_height/2 - bitMapCellH/2, gBlitingSurface);
					eigthChain=0;
				}
				
				if (restDuration > 0)
				{
					copySymbolCellToSurface(image, 2, restDuration, xRender + bitMapCellW*(currNote.time+currNote.duration), screen_height/2 - bitMapCellH/2, gBlitingSurface);
					eigthChain=0;
				}
			}
		}
		eigthChain=0;
		xRender = xRender + barWidth;
	}
	//Draw the 2 lines at end of score
	draw.x = xRender;
	draw.y = screen_height/2 - bitMapCellH/4;
	draw.w = 2;
	draw.h = bitMapCellH/2;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));

	draw.x = xRender + 4;
	draw.y = screen_height/2 - bitMapCellH/4;
	draw.w = 3;
	draw.h = bitMapCellH/2;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));
	
	//Draw the line going through the notes
	draw.x = 0;
	draw.y = screen_height/2 - 1;
	draw.w = xRender + 5;
	draw.h = 3;
	SDL_FillRect(gBlitingSurface,&draw, SDL_MapRGB (gBlitingSurface->format, 0,0,0));

	return gBlitingSurface;
}

//Nine Lives Blade Works
void SongPlayer::updateMusicSurface(SDL_Surface* gScreenSurface, int xCoord)
{
	//Blit notes onto windows
	copy.x = xCoord;
	copy.y = screen_height/4;
	copy.w = screen_width - 2*bitMapCellW;
	copy.h = screen_height/2;

	paste.x = 2*bitMapCellW;
	paste.y = screen_height/4;
	paste.w = screen_width - 2*bitMapCellW;
	paste.h = screen_height/2;

	//Update the surface
	SDL_BlitSurface( musicSheetSurface, &copy, gScreenSurface, &paste );

	//Draw the line marking where the guy is supposed to hit
	draw.x = 3.5*bitMapCellW;
	draw.y = 0;
	draw.w = 3;
	draw.h = screen_height;
	SDL_FillRect(gScreenSurface,&draw, SDL_MapRGB (musicSheetSurface->format, 100,50,180));

}
