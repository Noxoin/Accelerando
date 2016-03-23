#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "user_event.h"
#include "keyboard_poll.h"
#include "song_player.h"
#include "display_song_sel.h"
#include "display_result.h"
#include "SDLSetup.h"
#include "server_api.h"

const int SCREEN_WIDTH = 1280; const int SCREEN_HEIGHT = 720;

//SDL_Surface *screen = NULL;
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *font = NULL;

LTexture gSymbol;
LTexture gBuffer[3];
     
SDL_Event event;
int quit = 0;

User *user = NULL;

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("SDL couldn't start up for some reason\n");
        exit(1);
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    gWindow = SDL_CreateWindow("Accelerando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
        printf("Windows is null!!\n");
        exit(1);
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL) {
        printf("Rnderer could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }
    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    //font = TTF_OpenFont("res/fonts/font1.ttf", 52);
        
    //screen = SDL_GetWindowSurface(gWindow);
    
}

std::string song_selection() {
    int num_of_songs;
    int addPage = 0;
    int old_addPage = -1;
    int currRow = 0;
    int addPageMax;
    int currSel = 0;
    int currPage = -1;
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

    SDL_RenderClear(gRenderer);
    
    font = TTF_OpenFont( "res/fonts/font1.ttf", 52 );
    loadSelectScreenMedia(gRenderer, font, gKeyPressSurfaces, gDotTexture, gSongName);

    printf("Loaded Media\n");

    //Current displayed background texture
    LTexture bgTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_ENTER];

    while(!quit && !selected) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
                break;
            }

            dot.handleEvent(event);

            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                selected = true;
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_UP)){
            /*
                currRow++;
                if(currRow == SONGS_PER_PAGE) {
                    currRow--;
                }
                */
                if(currSel % SONGS_PER_PAGE > 0) {
                    currSel--;
                }
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_DOWN)) {
            /*
                currRow--;
                if(currRow == -1) {
                    currRow++;
                }
            */
                if(currSel % SONGS_PER_PAGE < (SONGS_PER_PAGE - 1)) {
                    currSel++;
                }
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_LEFT)){
                //addPage -= SONGS_PER_PAGE;
                if(currSel-SONGS_PER_PAGE >= 0) {
                    currSel -= SONGS_PER_PAGE;
                }
            }
            if( (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RIGHT)){
            /*
                addPage += SONGS_PER_PAGE;
                //stop turning over to the next page if this is the last page
                if (addPage == addPageMax){
                    addPage -= SONGS_PER_PAGE;
                }
            */
                if(currSel+SONGS_PER_PAGE < addPageMax) {
                    currSel += SONGS_PER_PAGE;
                }
            }
            dot.move();
            /*
            if(old_addPage != addPage) {
                bgTexture.render(gRenderer, 0,0);
                for(int i = 0; i < SONGS_PER_PAGE; i++){
                    //printf("Rendering Titles\n");
                    gSongName[i+addPage].render(gRenderer, 10, 50 +i*MAX_DISTANCE);
                }
                old_addPage = addPage;
            }
            */
            if(currPage != currSel/SONGS_PER_PAGE) {
                bgTexture.render(gRenderer, 0,0);
                for(int i = 0; i < SONGS_PER_PAGE; i++){
                    gSongName[i+SONGS_PER_PAGE*(currSel/SONGS_PER_PAGE)].render(gRenderer, 10, 50 +i*MAX_DISTANCE);
                }
                currPage = currSel/SONGS_PER_PAGE;
            }
                

            SDL_Rect notePanel;
            notePanel.x = 1200;//580;
            notePanel.y = 0;
            notePanel.w= 70;//60;
            notePanel.h= 720;//480;
            SDL_RenderCopy(gRenderer, bgTexture.mTexture, &notePanel, &notePanel);
            dot.render(gRenderer, gDotTexture[DOT_TEXTURE_NOTE]);
            SDL_RenderPresent(gRenderer);
        }

    }

    std::string selectedSong;
    std::vector<std::string> files = getFileNames();
    int count = 0;
    for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it){
        if(count == currSel) {
            selectedSong = (std::string) *it;
            break;
        }
        count++;
    }


    return selectedSong;
}

void results_screen(int (&results)[4], int idealHits, int *pressedOccurred, int *releasedOccurred, std::string filename) {
    bool exit = false;
    int r[SCORE_LEVELS];
    string *strScore = new string[SCORE_LEVELS];
    LTexture gScore[SCORE_LEVELS];
    LTexture gRBackround;

    r[PERFECT] = results[0];
    r[GOOD] = results[1];
    r[BAD] = results[2];
    r[MISS] = results[3];
    
    strScore = calculateResult(idealHits, r);

    font = TTF_OpenFont( "res/fonts/font1.ttf", 32 );
    loadResultsMedia(gRenderer, gScore, strScore, font);
    loadBackground(gRenderer, &gRBackround);

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    gRBackround.render(gRenderer, 0, 0);
    gScore[RANK].render( gRenderer, ( SCREEN_WIDTH - gScore[RANK].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[RANK].getHeight() ) / 2-150 );
    gScore[PERFECT].render( gRenderer, ( SCREEN_WIDTH - gScore[PERFECT].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[PERFECT].getHeight() ) / 2-100 );
    gScore[GOOD].render( gRenderer, ( SCREEN_WIDTH - gScore[GOOD].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[GOOD].getHeight() ) / 2 -50);
    gScore[BAD].render( gRenderer, ( SCREEN_WIDTH - gScore[BAD].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[BAD].getHeight() ) / 2 );
    gScore[MISS].render( gRenderer, ( SCREEN_WIDTH - gScore[MISS].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[MISS].getHeight() ) / 2 +50);
    gScore[TOTAL].render( gRenderer, ( SCREEN_WIDTH - gScore[TOTAL].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[TOTAL].getHeight() ) / 2 +100);
    gScore[ACCURACY].render( gRenderer, ( SCREEN_WIDTH - gScore[ACCURACY].getWidth() ) / 2, ( SCREEN_HEIGHT/2 - gScore[ACCURACY].getHeight() ) / 2 +150);

    //Sheena: functions are in display_result.cpp
            //data_1, data_2, and data_3 have been declared
    int *data_1 = calData(pressedOccurred, idealHits);
    int *data_2 = calData(releasedOccurred, idealHits);
    int *data_3 = calAvg(data_1, data_2, idealHits);
    plot(gRenderer, data_1, data_2, data_3, idealHits);

    if(user != NULL) {
        int ideal = 300*idealHits;
        int score = 300*r[PERFECT] + 100*r[GOOD] + 50*r[BAD];
        float accuracy = (float) (score*100.0) / ideal;
        std:string num = filename.substr(0, 2);
        int song_id = stoi(num, NULL);
        user->recordScore(song_id, score, accuracy);
    }


    //Update screen
    SDL_RenderPresent( gRenderer );

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

    free(data_1);
    free(data_2);
    free(data_3);

}

void play(std::string filename) {
    std::string str;
    str.append("res/songs/");
    str.append(filename);
    SongPlayer sp(str, gRenderer, gSymbol, gBuffer);
    
    int ms = 3000/sp.getTempo();
    //int ms = (int) 800.0/3.0/sp.getTempo();

    printf("Tempo is %dbpm (%d ms)\n", sp.getTempo(), ms);

#ifdef WITHKEYBOARD
    KeyboardPoll kp;
    kp.start();
#endif

    sp.setPlayscreenBackground(gRenderer, gBuffer);
    sp.setFrontScreen(gRenderer, gBuffer);

    Timer timer;

    int count = 0;
    timer.start(ms);
    
    while(!quit && !sp.finished) {
        while(SDL_PollEvent(&event) != 0) {
            if(quit) { break;}
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                /*case SDL_KEYDOWN:
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
                    break;*/
                case SDL_USEREVENT:
                    switch(event.user.code) {
                        case NOTE_PRESSED:
                            sp.notePressedHandler(event);
                            //printf("Piano Key %d was pressed\n", *(unsigned char *)event.user.data1);
                            break;
                        case NOTE_RELEASED:
                            //printf("Piano Key %d was released\n", *(unsigned char *)event.user.data1);
                            sp.noteReleasedHandler(event);
                            break;
                        case TIMER_EVENT:
                            //printf("Time: %02ds\n", ++count);
                            sp.timerHandler(gRenderer, gBuffer);
                            //SDL_UpdateWindowSurface(window);
                            break;
                    }
                    free(event.user.data1);
                    free(event.user.data2);
                    break;
            }
        }
    }

    timer.stop();
#ifdef WITHKEYBOARD
    kp.stop();
#endif

    int results[4];
    sp.getResults(results);
    printf("Perfect: %d, Good: %d, Ok: %d, Miss: %d\n", results[0], results[1], results[2], results[3]);
    results_screen(results, sp.count_notes, sp.pressedOccurred, sp.releasedOccurred, filename);

    gSymbol.free();
    for(int i = 0; i < 3; ++i) {
        gBuffer[i].free();
    }

}

int main(int argc, char *argv[]) {

    if(argc == 3) {
        printf("Attempting to login in as %s with pass %s\n", argv[1], argv[2]);
        user = (User *) malloc(sizeof(User));
        *user = User(argv[1], argv[2]);
        if(!(user->isLoggedIn())) {
            printf("Not correct credentials\n");
            free(user);
            user = NULL;
        }
    } else if(argc == 2) {
        printf("Not enough arguments\n");
        exit(0);
    } else if (argc > 3) {
        printf("Too many arguments\n");
    }

    init();
    SDL_RenderClear(gRenderer);
    // Init
    LTexture gTexture;
    if(!gTexture.loadFromFile(gRenderer, "res/screen/WelcomeScreen.png")){
        printf("Failed to load welcome screen\n");
    }
    SDL_RenderCopy(gRenderer, gTexture.mTexture, NULL, NULL);
    SDL_RenderPresent(gRenderer);
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
        if(quit) { break;}
        printf("Selected Song is %s\n", filename.c_str());
        play(filename);

    }

    //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();

}
