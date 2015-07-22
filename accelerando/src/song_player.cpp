#include "song_player.h"
#include <queue>
#include "beeper.h"

Beeper b;

SongPlayer::SongPlayer(std::string filename, Image image){
    song = (Song *) malloc(sizeof(Song));
    char *file = new char[filename.length() + 1];
    strcpy(file, filename.c_str());
    loadMIDI(file, song);
    tick_count = -50;
    currBarIndex = -1;
    currNoteIndex = -3;
    results[0] = 0;
    results[1] = 0;
    results[2] = 0;
    results[3] = 0;
    Gpio::init();
    finished = false;
    total_notes = 0;
    xCord = 0;
    musicSheetSurface = createMusicSurface(image, song);
}

SongPlayer::~SongPlayer(){
    //Gpio::clean();
    free(song);
}

void SongPlayer::notePressedHandler(SDL_Event e) {
    int tick_mod = (tick_count+8) % 8;
    if(tick_mod < 0) {
        return;
    }

    Bar bar = song->bars[currBarIndex];
    for(int i = 0; i < bar.length; ++i) {
        Note note = bar.notes[i];
        if(currNoteIndex == note.time && note.value == *(unsigned char*) e.user.data1) {
            //printf("YAY!!! Pressed Key %d correctly! Bar: %d; Note:%d; Tick_mod:%d\n", *(unsigned char*) e.user.data1, currBarIndex, currNoteIndex, tick_mod);
            // Record results
            if(tick_mod < 2 || tick_mod > 6) {
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


void SongPlayer::timerHandler(SDL_Surface* gScreenSurface) {
    if(finished) {
        printf("Song has Finished\n");
        return;
    }

    tick_count++;
    int tick_mod = (tick_count+16) % 8;
    //printf("tick_mod: %d\t", tick_mod);
    if(tick_mod % 2 == 0) {
        //printf("set HIGH\t");
        Gpio::setValue(Gpio::CLK, Gpio::HIGH);
    } else {
        //printf("set LOW\t");
        Gpio::setValue(Gpio::CLK, Gpio::LOW);
    }

    if(tick_mod == 4 ) {
        currNoteIndex = (currNoteIndex+1)%8;
        if(currNoteIndex == 0) {
            total_notes += (currBarIndex < 0) ? 0 :song->bars[currBarIndex].length;
            currBarIndex++;
        }
        // Check Missed notes here
    } else if (tick_mod == 6) {
        //playing note
        //printf("Playing note at bar: %d, note: %d\n", currBarIndex, currNoteIndex);
        if(currBarIndex > -1) {
            Bar bar = song->bars[currBarIndex];
            for(int i = 0; i < bar.length; ++i) {
                Note note = bar.notes[i];
                if(currNoteIndex == note.time) {
                    int hz = (float) pow(2, (float) (note.value-57)/12)*440;
                    b.beep(hz, 55000/bar.tempo*note.duration);
                }
            }
        }
    } else if (tick_mod == 7) {
        int light_note = currNoteIndex + 2;
        int light_bar = currBarIndex;
        if(light_note > 7) {
            light_note %= 8;
            light_bar++;
        } else if (light_note > -1 && light_bar < 0 ) {
            light_bar = 0;
        }
        if(light_bar != song->length && light_bar > -1) {
            Bar bar = song->bars[light_bar];
            for(int i = 0; i < bar.length; ++i) {
                Note note = bar.notes[i];
                if(light_note == note.time) {
                    printf("ON: light_note: %d; note.time: %d; duration: %d; note: %d; bar:%d\n", light_note, note.time, note.duration, currNoteIndex, currBarIndex);
                    Gpio::setValue(Gpio::getPin(note.value), Gpio::HIGH);
                }
            }
        }
    } else if (tick_mod == 5) {
        int light_note = currNoteIndex + 2;
        int light_bar = currBarIndex;
        if(light_note > 7) {
            light_note %= 8;
            light_bar++;
        } else if (light_note > -1 && light_bar < 0 ) {
            light_bar = 0;
        }
        if((light_bar != song->length && light_bar > -1) || (light_bar == song->length && light_note == 0)) {
            if(light_note == 0 && light_bar > 0) {
                Bar bar = song->bars[light_bar-1];
                for(int i = 0; i < bar.length; ++i) {
                    Note note = bar.notes[i];
                    if(8 == note.time+note.duration) {
                        printf("OFF: light_note: 8; note.time: %d; duration: %d\n", note.time, note.duration);
                        Gpio::setValue(Gpio::getPin(note.value), Gpio::LOW);
                    }
                }
            } else {
                Bar bar = song->bars[light_bar];
                for(int i = 0; i < bar.length; ++i) {
                    Note note = bar.notes[i];
                    if(light_note == note.time+note.duration) {
                        printf("OFF: light_note: %d; note.time: %d; duration: %d\n", light_note, note.time, note.duration);
                        Gpio::setValue(Gpio::getPin(note.value), Gpio::LOW);
                    }
                }
            }
        }
    } 
    
    //if (currBarIndex == song->length && currNoteIndex == 2) {
    if (currBarIndex == song->length){
        printf("End of Song\n");
        finished = true;
        total_notes += song->bars[currBarIndex].length;
    }

    xCord += 9.35;
    updateMusicSurface(gScreenSurface, (int) xCord);
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

