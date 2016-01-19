#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include <string>
#include <queue>
#include "midi_reader.h"
#include "gpio.h"
#include "LTexture.h"
#include "beeper.h"

class SongPlayer {
    private:
        Song *song;
        int tick_count;
        int currBarIndex;
        Bar currBar;
        int currNoteIndex;
        int results[4]; //0 = Perfect, 1 = Good, 2 = Satisfactory; 3 = misses (calculated at the end)
        
        // Rendering
        int screen_width;
        int screen_height;
        int ROW_X;
        int total_page_num;
        int row_num;
        int page_num;
        int replace_row_num;
        double xCord;
        double oldXCord;
        void copySymbolCellToSurface( SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int row, int col, int x, int y );
        int lookUpNote(int rowHeight, int currNoteValue);
        void createMusicSurface ( SDL_Renderer *gRenderer, LTexture gSymbol, LTexture *gBuffer, Song* song );
        void updateMusicSurface( SDL_Renderer *gRenderer, LTexture* gBuffer, int xCoord, int oldXCoord );

    public:
        SongPlayer(std::string filename, SDL_Renderer *gRenderer, LTexture gSymbol, LTexture *gBuffer);
        //SongPlayer(std::string filename, Image image);
        ~SongPlayer();
        bool finished;
        int total_notes;
        int getTempo();
        void getResults(int (&s)[4]);
        //void getResults(int &excellent, int &good, int &sat, int *miss);
        void notePressedHandler(SDL_Event e);
        void noteReleasedHandler(SDL_Event e);
        void timerHandler( SDL_Renderer *gRenderer, LTexture* gBuffer );
        void setPlayscreenBackground( SDL_Renderer *gRenderer, LTexture* gBuffer );
};
#endif

