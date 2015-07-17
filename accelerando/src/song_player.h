#include "midi_reader.h"
#include "gpio.h"
#include <string>
#include <SDL2/SDL.h>

class SongPlayer {
    private:
        Song *song;
        int tick_mod;
        int currBarIndex;
        Bar currBar;
        int currNoteIndex;
        int total_notes;
        int results[4]; //0 = Perfect, 1 = Good, 2 = Satisfactory; 3 = misses (calculated at the end
    public:
        SongPlayer(std::string filename);
        ~SongPlayer();
        bool finished;
        int getTempo();
        void getResults(int (&s)[4]);
        //void getResults(int &excellent, int &good, int &sat, int *miss);
        void notePressedHandler(SDL_Event e);
        void noteReleasedHandler(SDL_Event e);
        void timerHandler();
};

