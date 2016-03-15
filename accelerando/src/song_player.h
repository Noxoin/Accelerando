#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include <string>
#include <queue>
#include <math.h>
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
        double xCord;
        double oldXCord;
        int row_num;
        int page_num;
        int replace_row_num;
        void copySymbolCellToSurface( SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int row, int col, int x, int y );
        int resetNoteValue(Note note);
        int lookUpNote(int rowHeight, int currNoteValue, int sharpTrue);
        void drawExtraLines(SDL_Renderer *gRenderer, int currNoteValue, int extraLineX, int notePos, int rowHeight, bool sharpTrue);
        void drawAccKey(SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int keyNum, int currNoteValue, int x, int y, bool goingUp);
        void drawKeySig(SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int keyNum, int x, int y);
        void createMusicSurface ( SDL_Renderer *gRenderer, LTexture gSymbol, LTexture *gBuffer, Song* song );
        void updateMusicSurface( SDL_Renderer *gRenderer, LTexture* gBuffer, int xCoord, int oldXCoord );
        void updateScreen(SDL_Renderer *gRenderer, LTexture *gBuffer);

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
        void setFrontScreen( SDL_Renderer *gRenderer, LTexture* gBuffer );
};

#endif

