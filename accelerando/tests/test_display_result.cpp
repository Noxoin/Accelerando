#include "../src/display_result.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;

int main( )
{
    //Main loop flag
    bool quit = false;
    bool renderTrue = false;
    //Variables storing results
    int idealHits;
    int intScore[SCORE_LEVELS];
    string *strScore = new string[SCORE_LEVELS];
    //Rendered texture
    LTexture gScore[SCORE_LEVELS];
    //data to plot
    int* data_1;
    int* data_2;
    int* data_3;
    int size_of_data;
    
    //test data
    idealHits = 100;
    intScore[PERFECT] = 85;
    intScore[GOOD] = 5;
    intScore[BAD] = 8;
    intScore[MISS] = 2;
    
    size_of_data = 10;
    data_1 = new int[size_of_data];
    data_2 = new int[size_of_data];
    data_3 = new int[size_of_data];
    data_1[0] = 90;
    data_1[1] = 91;
    data_1[2] = 92;
    data_1[3] = 93;
    data_1[4] = 94;
    data_1[5] = 100;
    data_1[6] = 99;
    data_1[7] = 98;
    data_1[8] = 97;
    data_1[9] = 96;
    
    data_2[0] = 84;
    data_2[1] = 12;
    data_2[2] = 95;
    data_2[3] = 17;
    data_2[4] = 90;
    data_2[5] = 17;
    data_2[6] = 88;
    data_2[7] = 62;
    data_2[8] = 71;
    data_2[9] = 73;
    
    data_3[0] = 70;
    data_3[1] = 10;
    data_3[2] = 0;
    data_3[3] = 99;
    data_3[4] = 43;
    data_3[5] = 86;
    data_3[6] = 12;
    data_3[7] = 86;
    data_3[8] = 12;
    data_3[9] = 75;
    
    //background picture
    LTexture gBackground;
    //Event handler
    SDL_Event e;
    
    //Calculate result and return string
    strScore = calculateResult(idealHits, intScore);

    
    
    //======================
    //===== Initialize =====
    //======================
    //Start up SDL and create window
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
        
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}
                
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
				}
                //Open the font
                gFont = TTF_OpenFont( "res/fonts/font1.ttf", 32 );
			}
		}
	}

    
    
    //======================
    //===== Load Media =====
    //=====================
    loadBackground(gRenderer, &gBackground);
    loadResultsMedia(gRenderer, gScore, strScore, gFont);

    
    
    //=====================
    //======== Run ========
    //=====================
    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        if(!renderTrue){
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            gBackground.render(gRenderer, 0, 0);
            //Render current frame
            gScore[RANK].render( gRenderer, ( SCREEN_WIDTH - gScore[RANK].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[RANK].getHeight() ) / 2-150 );
            gScore[PERFECT].render( gRenderer, ( SCREEN_WIDTH - gScore[PERFECT].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[PERFECT].getHeight() ) / 2-100 );
            gScore[GOOD].render( gRenderer, ( SCREEN_WIDTH - gScore[GOOD].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[GOOD].getHeight() ) / 2 -50);
            gScore[BAD].render( gRenderer, ( SCREEN_WIDTH - gScore[BAD].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[BAD].getHeight() ) / 2 );
            gScore[MISS].render( gRenderer, ( SCREEN_WIDTH - gScore[MISS].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[MISS].getHeight() ) / 2 +50);
            gScore[TOTAL].render( gRenderer, ( SCREEN_WIDTH - gScore[TOTAL].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[TOTAL].getHeight() ) / 2 +100);
            gScore[ACCURACY].render( gRenderer, ( SCREEN_WIDTH - gScore[ACCURACY].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[ACCURACY].getHeight() ) / 2 +150);

            //plot the data
            plot(gRenderer, data_1, data_2, data_3, size_of_data);

            //Update screen
            SDL_RenderPresent( gRenderer );
            printf("im rendering\n");
            renderTrue = true;
        }
    }
    
    
    
    //=====================
    //======= Close =======
    //=====================
    //Free loaded images
	for( int i = 0; i <=SCORE_LEVELS; ++i ){
		gScore[ i ].free();
	}
    
	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;
    
    //Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
    
	return 0;
}
