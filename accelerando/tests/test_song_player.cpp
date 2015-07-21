#include "../src/keyboard_poll.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include "../src/timer.h"
#include "../src/user_event.h"
#include "../src/song_player.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Surface *screen = NULL;
SDL_Window *window = NULL;

SDL_Event event;
int quit = 0;

int main() {

    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("SDL couldn't start up for some reason\n");
        exit(1);
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Windows is null!!\n");
        exit(1);
    }
        
    screen = SDL_GetWindowSurface(window);

    Image image(screen);

    //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));


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

    int results[4];
    sp.getResults(results);
    printf("Perfect: %d, Good: %d, Ok: %d, Miss: %d\n", results[0], results[1], results[2], results[3]);

    timer.stop();
    kp.stop();

    SDL_DestroyWindow(window);
    SDL_Quit();

}

