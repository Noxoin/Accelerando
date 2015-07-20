//Loads media
bool loadMedia(SDL_Surface* gBlitingSurface);

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path , SDL_Surface* gBlitingSurface);

//Creates the surface to be displayed
SDL_Surface* CreateBlitingSurface (Song* song, SDL_Surface* gBlitingSurface);

//Sets the background on the screen surface
void SetPlayscreenBackground (SDL_Surface* gScreenSurface, SDL_Surface* gBlitingSurface);

//Updates the play window screen with part of the bliting surface
void UpdatePlaySurface (SDL_Surface* gScreenSurface, SDL_Surface* gBlitingSurface, int xCoord);
