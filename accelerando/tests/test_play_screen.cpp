//Using SDL, standard IO, and strings
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "../src/playscreen.h"


int main( int argc, char* args[] )
{
	Song *song = (Song *) malloc(sizeof(Song));

	loadMIDI("res/songs/demo1.mid", song);

//=======================================================================================================================

	int xCoord = 0;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;	

	//The window we'll be bliting to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gWindowSurface = NULL;

	printf( "\n Setting up SDL");
	//Start up SDL and create window
	SDL_Init( SDL_INIT_VIDEO );
	//Create window

	printf( "\n Creating Blit Surface");
	SDL_Surface* gBlitingSurface;

	printf( "\n Creating Window");
	gWindow = SDL_CreateWindow( "Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		printf( "\n Getting Window Surface");
		//Get window surface
		gWindowSurface = SDL_GetWindowSurface( gWindow );

                if(gWindowSurface == NULL) {
                    printf("WTF\n");
                    exit(1);
                }

                Image image(gWindowSurface);

		//Main loop flag
		printf( "\n Finished Getting Window Surface");
		bool quit = false;
		SDL_Event e;
		
		printf( "\n Preparing to Blit");
		gBlitingSurface = createBlitingSurface(image, song, gBlitingSurface);

		printf( "\n Setting Background");
		setPlayscreenBackground (gWindowSurface, gBlitingSurface);

		while( !quit )
		{
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
			}
			updatePlaySurface( gWindowSurface,gBlitingSurface, xCoord);

			SDL_UpdateWindowSurface( gWindow );
			SDL_Delay(50);
			xCoord = xCoord + 10;
		}
                //delete image;
	}

	//Free resources and close SDL

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	return 0;
}
