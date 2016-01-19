#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("Starting up Accelerando......\n");
    
#ifdef DEBUG
    printf("DEBUG has been set..\n");
#endif
    return 0;
}

/*#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "user_event.h"
#include "keyboard_poll.h"
#include "song_player.h"
#include "display_song_sel.h"
#include "display_result.h"
#include "SDLSetup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Surface *screen = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
     

SDL_Event event;
int quit = 0;

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("SDL couldn't start up for some reason\n");
        exit(1);
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    window = SDL_CreateWindow("Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Windows is null!!\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Rnderer could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }
    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    font = TTF_OpenFont("res/fonts/font1.ttf", 27);
        
    screen = SDL_GetWindowSurface(window);
    
}

std::string song_selection() {
    int num_of_songs;
    int addPage = 0;
    int old_addPage = -1;
    int currRow = 0;
    int addPageMax;
    //Main loop flag
    bool selected = false;
    //Go to song selection screen flag
    bool go_to_song_sel = false;
    Dot dot;

    //Dot texture
    LTexture gDotTexture[DOT_TEXTURE_TOTAL];
    //The images that correspond to a key press
    LTexture gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
    
    //Read file names
    num_of_songs = readFileNames();
    addPageMax = ceil(num_of_songs/(double)SONGS_PER_PAGE)*SONGS_PER_PAGE;

    printf("Finished reading files names\n");

    //Song name texture
    LTexture gSongName[addPageMax];

    SDL_RenderClear(renderer);
    
    loadSelectScreenMedia(renderer, font, gKeyPressSurfaces, gDotTexture, gSongName);

    printf("Loaded Media\n");

    //Current displayed background texture
    LTexture bgTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_ENTER];

    while(!quit && !selected) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }

            dot.handleEvent(event);

            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                selected = true;
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_UP)){
                currRow++;
                if(currRow == SONGS_PER_PAGE) {
                    currRow--;
                }
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_DOWN)) {
                currRow--;
                if(currRow == -1) {
                    currRow++;
                }
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LEFT) && addPage!=0){
                addPage -= SONGS_PER_PAGE;
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RIGHT)){
                addPage += SONGS_PER_PAGE;
                //stop turning over to the next page if this is the last page
                if (addPage == addPageMax){
                    addPage -= SONGS_PER_PAGE;
                }
            }
        }

        dot.move();
        if(old_addPage != addPage) {
            bgTexture.render(renderer, 0,0);
            for(int i = 0; i < SONGS_PER_PAGE; i++){
                printf("Rendering Titles\n");
                gSongName[i+addPage].render(renderer, 10, 50 +i*MAX_DISTANCE);
            }
            old_addPage = addPage;
        }

        SDL_Rect notePanel;
        notePanel.x = 580;
        notePanel.y = 0;
        notePanel.w= 60;
        notePanel.h= 480;
        SDL_RenderCopy(renderer, bgTexture.mTexture, &notePanel, &notePanel);
        printf("Rendering Dot\n");
        dot.render(renderer, gDotTexture[DOT_TEXTURE_NOTE]);

        SDL_RenderPresent(renderer);
    }

    std::string selectedSong;
    std::vector<std::string> files = getFileNames();
    int count = 0;
    for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it){
        if(count++ == currRow+addPage) {
            selectedSong = (std::string) *it;
        }
    }

    return selectedSong;
}

void results_screen(int (&results)[4], int idealHits) {
    bool exit = false;
    int r[SCORE_LEVELS];
    string *strScore = new string[SCORE_LEVELS];
    LTexture gScore[SCORE_LEVELS];

    r[PERFECT] = results[0];
    r[GOOD] = results[1];
    r[BAD] = results[2];
    r[MISS] = results[3];

    strScore = calculateResult(idealHits, r);

    loadResultsMedia(renderer, gScore, strScore, font);

    SDL_RenderClear(renderer);

    gScore[RANK].render( renderer, ( SCREEN_WIDTH - gScore[RANK].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[RANK].getHeight() ) / 2-150 );
    gScore[PERFECT].render( renderer, ( SCREEN_WIDTH - gScore[PERFECT].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[PERFECT].getHeight() ) / 2-100 );
    gScore[GOOD].render( renderer, ( SCREEN_WIDTH - gScore[GOOD].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[GOOD].getHeight() ) / 2 -50);
    gScore[BAD].render( renderer, ( SCREEN_WIDTH - gScore[BAD].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[BAD].getHeight() ) / 2 );
    gScore[MISS].render( renderer, ( SCREEN_WIDTH - gScore[MISS].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[MISS].getHeight() ) / 2 +50);
    gScore[TOTAL].render( renderer, ( SCREEN_WIDTH - gScore[TOTAL].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[TOTAL].getHeight() ) / 2 +100);
    gScore[ACCURACY].render( renderer, ( SCREEN_WIDTH - gScore[ACCURACY].getWidth() ) / 2, ( SCREEN_HEIGHT - gScore[ACCURACY].getHeight() ) / 2 +150);
    //Update screen
    SDL_RenderPresent( renderer );

    while(!quit && !exit) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }

            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                exit = true;
            }
        }
    }

}

void play(std::string filename, Image image) {
    SongPlayer sp("res/songs/demo1.mid", image);
    
    int ms = 7500/sp.getTempo();

    printf("Tempo is %dbpm (%d ms)\n", sp.getTempo(), ms);

    KeyboardPoll kp;
    kp.start();

    sp.setPlayscreenBackground(image, screen);
    SDL_UpdateWindowSurface(window);

    Timer timer;

    int count = 0;
    timer.start(ms);
    
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
                            sp.timerHandler(screen);
                            SDL_UpdateWindowSurface(window);
                            break;
                    }
                    free(event.user.data1);
                    break;
            }
        }
    }

    timer.stop();
    kp.stop();

    int results[4];
    sp.getResults(results);
    printf("Perfect: %d, Good: %d, Ok: %d, Miss: %d\n", results[0], results[1], results[2], results[3]);
    results_screen(results, sp.total_notes);
}

int main() {

    init();
    SDL_RenderClear(renderer);
    // Init
    LTexture gTexture;
    if(!gTexture.loadFromFile(renderer, "res/screen/WelcomeScreen.png")){
        printf("Failed to load welcome screen\n");
    }
    SDL_RenderCopy(renderer, gTexture.mTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    bool enter = false;

    while(!quit) {
        // Display Welcome
        while(!enter && !quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                }

                if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
            }
        }

        printf("Entering song select\n");
        std::string filename = song_selection();
        printf("Selected Song is %s\n", filename.c_str());
        Image image(screen);
        play(filename, image);

    }

    //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    SDL_DestroyWindow(window);
    SDL_Quit();

}
*/
