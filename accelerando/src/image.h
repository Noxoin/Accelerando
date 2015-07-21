#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

class Image {
    private:
        SDL_Surface *symbolsMap;
        SDL_Surface *loadBMPSurface(std::string path);
        SDL_PixelFormat* fmt;
    public:
        int screen_width;
        int screen_height;
        Image(SDL_Surface *windowSurface);
        ~Image();
        SDL_Surface* getSymbolsMap();
};

