#include "display_screen.h"

//declare and define renderer and window
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

//The images that correspond to a keypress
LTexture gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
//Current displayed texture
LTexture gTexture;
//dot texture
LTexture gDotTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	//Load default surface
    if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT].loadFromFile( "res/screen/WelcomeScreen.png" ) )
	{
		printf( "Failed to load default image!\n" );
		success = false;
	}

	//Load selection screen
    if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_ENTER].loadFromFile( "res/screen/SongSelectionScreen.png" ) )
	{
		printf( "Failed to load song selection screen image!\n" );
		success = false;
	}
    
    //Load dot texture
	if( !gDotTexture.loadFromFile( "res/screen/dot.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
/*
	//Load up surface
	if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP].loadFromFile( "../res/screen/up.png" ) )
	{
		printf( "Failed to load up image!\n" );
		success = false;
	}

	//Load down surface
	if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP].loadFromFile( "../res/screen/down.png" ) )
	{
		printf( "Failed to load down image!\n" );
		success = false;
	}

	//Load left surface
	if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP].loadFromFile( "../res/screen/left.png" ) )
	{
		printf( "Failed to load left image!\n" );
		success = false;
	}

	//Load right surface
	if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP].loadFromFile( "../res/screen/right.png" ) )
	{
		printf( "Failed to load right image!\n" );
		success = false;
	}
*/
	return success;
}

void close()
{
	//Deallocate surfaces
	for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
	{
		gKeyPressSurfaces[ i ].free();
	}
    //Free loaded images
	gDotTexture.free();
    gTexture.free();
    
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void run(){
    //Main loop flag
    bool quit = false;
    //Go to song selection screen flag
    bool go_to_song_sel = false;

    //Event handler
    SDL_Event e;

    //Set default current surface
    gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
    
    //The dot that will be pointing to current song
    Dot dot;

    //While application is running
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            //User requests quit
            if( e.type == SDL_QUIT ){
                quit = true;
            }
            //Handle input for the dot
            if (go_to_song_sel){dot.handleEvent( e );}
            //User presses a key
            if( (e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_RETURN)){
                gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_ENTER ];
                go_to_song_sel = true;
            }
        }
        
        //Move the dot
        if (go_to_song_sel){dot.move();}

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render texture to screen
        gTexture.render(0,0);
        //Render objects
        // !!DOT HAS TO BE RENDERED AFTER TEXTURE!!
        if (go_to_song_sel){dot.render(gDotTexture);}
        
        //Update screen
        SDL_RenderPresent( gRenderer );
    }
}
