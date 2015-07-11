#include "../src/keyboard_poll.h"
#include <SDL2/SDL.h>
#include <stdio.h>

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

    SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

    SDL_UpdateWindowSurface(window);

/*
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if(screen == null) {
        printf("Failed to produce a screen\n");
        exit(1);
    }

    SDL_WM_SetCaption("Event test", NULL);
*/
    start_keyboard_polling();
    
    while(!quit) {
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_UP:
                            printf("You pressed the up arrow!!\n");
                            break;
                        case SDLK_DOWN:
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
                            printf("Piano Key %d was pressed\n", *(unsigned char *)event.user.data1);
                            break;
                        case NOTE_RELEASED:
                            printf("Piano Key %d was released\n", *(unsigned char *)event.user.data1);
                            break;
                    }
                    free(event.user.data1);
                    break;
            }
        }
    }

    stop_keyboard_polling();

    SDL_DestroyWindow(window);
    SDL_Quit();

}

