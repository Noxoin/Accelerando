#include "../src/display_song_sel.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;

int main()
{
    int num_of_songs;
    int addPage = 0;
    int addPageMax;
    //Main loop flag
    bool quit = false;
    //Go to song selection screen flag
    bool go_to_song_sel = false;
    
    //Current displayed background texture
    LTexture gTexture;
    //Dot texture
    LTexture gDotTexture[DOT_TEXTURE_TOTAL];
    //The images that correspond to a key press
    LTexture gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
     
    //Read file names
    num_of_songs = readFileNames();
    addPageMax = ceil(num_of_songs/(double)SONGS_PER_PAGE)*SONGS_PER_PAGE;
    //Song name texture
    LTexture gSongName[addPageMax];
    
    //Event handler
    SDL_Event e;
    //The dot that will be pointing to current song
    Dot dot;
    
    
    
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
        
		gWindow = SDL_CreateWindow( "Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}
                //Initialize fonts loading
                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
				}
                                gFont = TTF_OpenFont( "res/fonts/font1.ttf", 52 );
			}
		}
	}


    
    //======================
    //===== Load Media =====
    //======================
    loadSelectScreenMedia(gRenderer, gFont, gKeyPressSurfaces, gDotTexture, gSongName);


    
    //=====================
    //======== Run ========
    //=====================
    //Set default current surface
    gTexture = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
    gTexture.render(gRenderer, 0,0);
    bool old_state = go_to_song_sel;
    int old_addPage = addPage;
    
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
            if( (e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_LEFT) && addPage!=0){
                addPage -= SONGS_PER_PAGE;
            }
            if( (e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_RIGHT)){
                addPage += SONGS_PER_PAGE;
                //stop turning over to the next page if this is the last page
                if (addPage == addPageMax){
                    addPage -= SONGS_PER_PAGE;
                }
            }
        }
        
        //Move the dot
        if (go_to_song_sel){dot.move();}

        //Clear screen
        /*
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        */

        //Render background texture to screen
        //Render objects
        if (go_to_song_sel){
            if(old_state != go_to_song_sel || old_addPage != addPage) {
                gTexture.render(gRenderer, 0,0);
                for(int i = 0; i < SONGS_PER_PAGE; i++){
                    //printf("Rendering Titles\n");
                    gSongName[i+addPage].render( gRenderer, 10, 50 +i*MAX_DISTANCE);
                }
                old_state = go_to_song_sel;
                old_addPage = addPage;
            }
            SDL_Rect notePanel;
            notePanel.x = 1200;
            notePanel.y = 0;
            notePanel.w= 80;
            notePanel.h= 720;
            SDL_RenderCopy(gRenderer, gTexture.mTexture, &notePanel, &notePanel);
            //printf("Rendering Dot\n");
            dot.render(gRenderer, gDotTexture[DOT_TEXTURE_NOTE]);
        }
        //Update screen
        SDL_RenderPresent( gRenderer );
    }
    
    
    
    //=====================
    //======= Close =======
    //=====================
    //Free textures
    for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++ ){
        gKeyPressSurfaces[ i ].free();
    }
    for(int i = 0; i < num_of_songs; i++){
        gSongName[i].free();
    }
    for(int i =0; i < DOT_TEXTURE_TOTAL; i++){
        gDotTexture[i].free();
    }
    gTexture.free();
    
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
