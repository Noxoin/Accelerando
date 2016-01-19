////#include "../src/keyboard_poll.h"
#include <string.h>
#include <stdio.h>
#include "../src/timer.h"
#include "../src/user_event.h"
#include "../src/song_player.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Event event;
int quit = 0;

LTexture gSymbol;
LTexture gBuffer[3];


int main() {

    //======================
    //===== Initialize =====
    //======================
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("SDL couldn't start up for some reason\n");
        exit(1);
    }

    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    gWindow = SDL_CreateWindow("Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
        printf("Windows is null!!\n");
        exit(1);
    }
    
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    
    if( gRenderer == NULL ){
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
        //if( TTF_Init() == -1 )
        //{
        //   printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        //}
        //gFont = TTF_OpenFont( "res/fonts/font1.ttf", 27 );
    }


    SongPlayer sp("res/songs/Cmaj_2Oct_RH_4n120_ScaleRun23Bars.mid", gRenderer, gSymbol, gBuffer);
    
    int ms = 7500/sp.getTempo();

    printf("Tempo is %dbpm (%d ms)\n", sp.getTempo(), ms);

    ////KeyboardPoll kp;
    ////kp.start();

    sp.setPlayscreenBackground(gRenderer, gBuffer);

    Timer timer;

    int count = 0;
    timer.start(ms);

    
    //=====================
    //======== Run ========
    //=====================
    while(!quit && !sp.finished) {
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_UP:
                            timer.start(ms);
                            printf("You pressed the up arrow!!\n");
                            break;
                        case SDLK_DOWN:
                            timer.stop();
                            printf("You pressed the down arrow!!\n");
                            break;
                        case SDLK_LEFT:
                            printf("You pressed the left arrow!!\n");
                            break;
                        case SDLK_RIGHT:
                            printf("You pressed the right arrow!!\n");
                            break;
                    }
                    break;
                case SDL_USEREVENT:
                    switch(event.user.code) {
                        case NOTE_PRESSED:
                            sp.notePressedHandler(event);
                            //printf("Piano Key %d was pressed\n", *(unsigned char *)event.user.data1);
                            break;
                        case NOTE_RELEASED:
                            //printf("Piano Key %d was released\n", *(unsigned char *)event.user.data1);
                            break;
                        case TIMER_EVENT:
                            //printf("Time: %02ds\n", ++count);
                            sp.timerHandler(gRenderer, gBuffer);
                            break;
                    }
                    free(event.user.data1);
                    break;
            }
        }
    }
    
    
    //=====================
    //======= Close =======
    //=====================
    int results[4];
    sp.getResults(results);
    printf("Perfect: %d, Good: %d, Ok: %d, Miss: %d\n", results[0], results[1], results[2], results[3]);

    timer.stop();
    ////kp.stop();

    //Free textures
    gSymbol.free();
    gBuffer[0].free(); 
    gBuffer[1].free();
    gBuffer[2].free(); 
    
    //Free global font
    //TTF_CloseFont( gFont );
    //gFont = NULL;
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    //TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

