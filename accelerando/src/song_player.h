#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "midi_reader.h"
#include "gpio.h"
#include <string>
#include <SDL2/SDL.h>
#include "image.h"

class SongPlayer {
    private:
        Song *song;
        int tick_count;
        int currBarIndex;
        Bar currBar;
        int currNoteIndex;
        int results[4]; //0 = Perfect, 1 = Good, 2 = Satisfactory; 3 = misses (calculated at the end
        
        // Rendering
        int screen_width;
        int screen_height;
        double xCord;
        SDL_Surface *musicSheetSurface;
        SDL_Surface *createMusicSurface (Image image, Song *song);
        void updateMusicSurface(SDL_Surface *gScreenSurface, int xCoord);
    public:
        SongPlayer(std::string filename, Image image);
        ~SongPlayer();
        bool finished;
        int total_notes;
        int getTempo();
        void getResults(int (&s)[4]);
        //void getResults(int &excellent, int &good, int &sat, int *miss);
        void notePressedHandler(SDL_Event e);
        void noteReleasedHandler(SDL_Event e);
        void timerHandler(SDL_Surface *windowSurface);
        void setPlayscreenBackground(Image image, SDL_Surface *gScreenSurface);
};
#endif

