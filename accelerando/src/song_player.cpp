#include "song_player.h"
#include <queue>
#include "beeper.h"

Beeper b;

SongPlayer::SongPlayer(std::string filename){
    song = (Song *) malloc(sizeof(Song));
    char *file = new char[filename.length() + 1];
    strcpy(file, filename.c_str());
    loadMIDI(file, song);
    tick_mod = -1;
    currBarIndex = 0;
    currNoteIndex = 0;
    results[0] = 0;
    results[1] = 0;
    results[2] = 0;
    results[3] = 0;
    Gpio::init();
    finished = false;
    total_notes = 0;
}

SongPlayer::~SongPlayer(){
    //Gpio::clean();
    free(song);
}

void SongPlayer::notePressedHandler(SDL_Event e) {
    Bar bar = song->bars[currBarIndex];
    for(int i = 0; i < bar.length; ++i) {
        Note note = bar.notes[i];
        if(currNoteIndex == note.time && note.value == *(unsigned char*) e.user.data1) {
            printf("YAY!!! Pressed Key %d correctly! Bar: %d; Note:%d\n", *(unsigned char*) e.user.data1, currBarIndex, currNoteIndex);
            // Record results
            if(tick_mod < 2 && tick_mod > 6) {
                //PERFECT!!;
                results[0]++;
            } else if (tick_mod == 4) {
                results[2]++;
            } else {
                results[1]++;
            }
            return;
        }
    }
    printf("Sorry.... you pressed key %d incorrectly! Bar: %d; Note:%d\n",*(unsigned char*) e.user.data1, currBarIndex, currNoteIndex);

}


void SongPlayer::timerHandler() {
    if(finished) {
        return;
    }
    tick_mod = (tick_mod+1) % 8;
    //printf("tick_mod: %d\t", tick_mod);
    if(tick_mod % 2 == 0) {
        printf("set HIGH\t");
        Gpio::setValue(Gpio::CLK, Gpio::HIGH);
    } else {
        printf("set LOW\t");
        Gpio::setValue(Gpio::CLK, Gpio::LOW);
    }

    if(tick_mod == 4 ) {
        currNoteIndex = (currNoteIndex+1)%8;
        if(currNoteIndex == 0) {
            total_notes += song->bars[++currBarIndex].length;
        }
        // Check Missed notes here
    } else if (tick_mod == 0) {
        //playing note
        //printf("Playing note at bar: %d, note: %d\n", currBarIndex, currNoteIndex);
        Bar bar = song->bars[currBarIndex];
        for(int i = 0; i < bar.length; ++i) {
            Note note = bar.notes[i];
            if(currNoteIndex == note.time) {
                int hz = (float) pow(2, (float) (note.value-57)/12)*440;
                b.beep(hz, 55000/bar.tempo*note.duration);
                break;
            }
        }
    } else if (tick_mod == 7 && currNoteIndex == 7 && currBarIndex == song->length) {
        printf("End of Song\n");
        finished = true;
    }

}

int SongPlayer::getTempo() {
    return song->bars[0].tempo;
}

void SongPlayer::getResults(int (&s)[4]) {
    results[3] = total_notes - results[0] - results[1] - results[2];
    s[0] = results[0];
    s[1] = results[1];
    s[2] = results[2];
    s[3] = results[3];
}
    

/*
void SongPlayer::getResults(int &excellent, int &good, int &sat, int &miss) {
    results[3] = total_notes - results[0] - results[1] - results[2];
    excellent = results[0];
    good = results[1];
    sat = results[2];
    miss = results[3];
}
*/
