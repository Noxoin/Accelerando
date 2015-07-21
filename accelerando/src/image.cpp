#include "image.h"
#include <string>

SDL_Surface* Image::loadBMPSurface(std::string path) {
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str() );
    if( loadedSurface == NULL ) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, fmt, 0);
        if( optimizedSurface == NULL) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

Image::Image(SDL_Surface *windowSurface){
    fmt = windowSurface->format;
    screen_width = windowSurface->w;
    screen_height = windowSurface->h;
    symbolsMap = loadBMPSurface("res/images/symbolsMap.bmp");
}

SDL_Surface* Image::getSymbolsMap() {
    if(symbolsMap == NULL) {
        bool success = true;
        symbolsMap = loadBMPSurface("res/images/symbolsMap.bmp");
        if(symbolsMap == NULL) {
            printf( "Failed to load symbols image!\n");
            return NULL;
        }
    }

    return symbolsMap;
}

Image::~Image(){
    printf("Getting deallocated image\n");
    //SDL_FreeSurface(symbolsMap);
    //symbolsMap = NULL;
}
    
