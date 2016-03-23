#include "song_player.h"


void SongPlayer::setFrontScreen( SDL_Renderer *gRenderer, LTexture* gBuffer ) {
    updateMusicSurface( gRenderer, gBuffer, (int) xCord, (int)oldXCord );
}

void merge(Note * arr, int start, int mid, int end) {
    int left_len = mid - start;
    int right_len = end - mid;
    Note * left = (Note *) malloc(sizeof(Note)*left_len);
    Note * right = (Note *) malloc(sizeof(Note)*right_len);

    for(int i = 0; i < left_len; ++i) {
        left[i].value = arr[i+start].value;
        left[i].time = arr[i+start].time;
    }
    for(int i = 0; i < right_len; ++i) {
        right[i].value = arr[i+mid].value;
        right[i].time = arr[i+mid].time;
    }

    int i = 0;
    int j = 0;
    int k = 0;
    while(i < left_len || j < right_len) {
        if(i == left_len) {
            arr[start+k].time = right[j].time;
            arr[start+k].value = right[j].value;
            j++;
        } else if( j == right_len) {
            arr[start+k].time = left[i].time;
            arr[start+k].value = left[i].value;
            i++;
        } else {
            if(left[i].time < right[j].time) {
                arr[start+k].time = left[i].time;
                arr[start+k].value = left[i].value;
                i++;
            } else {
                arr[start+k].time = right[j].time;
                arr[start+k].value = right[j].value;
                j++;
            }
        }
        k++;
    }
    free(left);
    free(right);
}

void mergesort(Note * arr, int start, int end) {
    if(end - start <= 1) {
        return;
    }
    mergesort(arr, start, (end+start)/2);
    mergesort(arr, (end+start)/2, end);
    merge(arr, start, (end+start)/2, end);
}

int countNotes(Song * song) {
    int count_notes = 0;
    for(int i = 0; i < song->length; ++i) {
        Bar bar = song->bars[i];
        count_notes += bar.length;
    }
    return count_notes;
}

void SongPlayer::genPressRelease(Song * song, int count_notes, Note * pressEvents, Note * releaseEvents) {

    int k = 0;
    for(int i = 0; i < song->length; ++i) {
        Bar bar = song->bars[i];
        tsig = bar.tsig_numerator;
        for(int j = 0; j < bar.length; ++j) {
            Note note = bar.notes[j];
            pressEvents[k].time = (i*tsig*2+note.time)*8;
            releaseEvents[k].time = (i*tsig*2+note.time + note.duration)*8-3;
            pressEvents[k].value = note.value;
            releaseEvents[k].value = note.value;
            k++;
        }
    }
    mergesort(releaseEvents, 0, count_notes);
}


SongPlayer::SongPlayer(std::string filename, SDL_Renderer *gRenderer, LTexture gSymbol, LTexture *gBuffer){
    song = (Song *) malloc(sizeof(Song));
    char *file = new char[filename.length() + 1];
    strcpy(file, filename.c_str());
    loadMIDI(file, song);
    
    count_notes = countNotes(song);
    pressedEvents = (Note *) malloc(sizeof(Note)*count_notes);
    pressedOccurred = (int *) malloc(sizeof(int)*count_notes);
    releasedEvents = (Note *) malloc(sizeof(Note)*count_notes);
    releasedOccurred = (int *) malloc(sizeof(int)*count_notes);
    genPressRelease(song, count_notes, pressedEvents, releasedEvents);

    for(int i = 0; i < count_notes; ++i) {
        pressedOccurred[i] = 0;
        releasedOccurred[i] = 0;
    }


    pressedIndex = 0;
    releasedIndex = 0;

    tick_count = -58;
    results[0] = 0;
    results[1] = 0;
    results[2] = 0;
    results[3] = 0;
    Gpio::init();
    finished = false;
    row_num = 0;
    page_num = 0;
    createMusicSurface(gRenderer, gSymbol, gBuffer, song);
    int w; 
    int h;
    xCord = -320;
    oldXCord = xCord;

}

SongPlayer::~SongPlayer(){
    //Gpio::clean();
    free(song);
    free(pressedEvents);
    free(pressedOccurred);
    free(releasedEvents);
    free(releasedOccurred);
}

void SongPlayer::noteReleasedHandler(SDL_Event e) {
    Gpio::setValue(Gpio::getPressedPin(*(unsigned char*) e.user.data1), Gpio::LOW);
    for(int i = releasedIndex; releasedEvents[i].time < tick_count + 8; ++i) {
        if(releasedEvents[i].value != *(unsigned char*) e.user.data1 || releasedOccurred[i] > 0) {
            continue;
        } else {
            //found the corresponding note
            int diff = tick_count - releasedEvents[i].time;
            diff = (diff < 0) ? diff*(-1): diff;
            if(diff < 4) {
                //PERFECT
                results[0]++;
                releasedOccurred[i] = 300;
            } else if (diff < 6) {
                results[1]++;
                releasedOccurred[i] = 100;
            }
        }
    }
}

void SongPlayer::notePressedHandler(SDL_Event e) {
    Gpio::setValue(Gpio::getPressedPin(*(unsigned char*) e.user.data1), Gpio::HIGH);
    for(int i = pressedIndex; pressedEvents[i].time < tick_count + 8; ++i) {
        if(pressedEvents[i].value != *(unsigned char*) e.user.data1 || pressedOccurred[i] > 0) {
            continue;
        } else {
            //found the corresponding note
            int diff = tick_count - pressedEvents[i].time;
            diff = (diff < 0) ? diff*(-1): diff;
            if(diff < 2) {
                //PERFECT
                results[0]++;
                pressedOccurred[i] = 300;
            } else if (diff < 4) {
                results[1]++;
                pressedOccurred[i] = 100;
            } else if (diff < 6) {
                results[2]++;
                pressedOccurred[i] = 50;
            }
        }
    }
}

void SongPlayer::timerHandler( SDL_Renderer *gRenderer, LTexture* gBuffer ) {
    if(tick_count > releasedEvents[count_notes-1].time + 7) {
        printf("Song has Finished\n");
        for(int i = 0; i < count_notes; ++i) {
            printf("Pressed: %d, %d\t Released: %d, %d\n", pressedEvents[i].time, pressedOccurred[i], releasedEvents[i].time, releasedOccurred[i]);
        }
        finished = true;
        return;
    }

    if((tick_count+16*tsig) % (16*tsig) == (16*tsig - 3)) {
        b.beep(466.164, 100);
    } else if((tick_count + 16*tsig) % 16 == 13) {
        b.beep(233.082, 100);
    }


    while(pressedEvents[pressedIndex].time < tick_count - 8) {
        pressedIndex++;
    }
    while(releasedEvents[releasedIndex].time < tick_count - 8) {
        releasedIndex++;
    }

    for(int i = pressedIndex; pressedEvents[i].time < tick_count + 7; ++i) {
        if(pressedEvents[i].time == tick_count + 6) {
            Note note = pressedEvents[i];
            Gpio::setValue(Gpio::getLEDPin(note.value), Gpio::HIGH);
        }
    }

    for(int i = releasedIndex; releasedEvents[i].time < tick_count + 7; ++i) {
        if(releasedEvents[i].time == tick_count + 6) {
            Note note = releasedEvents[i];
            Gpio::setValue(Gpio::getLEDPin(note.value), Gpio::LOW);
        }
    }
    //processTime();

    int tick_mod = (tick_count) % 8;
    //printf("tick_mod: %d\t", tick_mod);
    if(tick_mod % 2 == 0) {
        Gpio::setValue(Gpio::CLK, Gpio::HIGH);
        oldXCord = xCord;
        xCord += 5.6198*2;//9.35;
        //updateMusicSurface( gRenderer, gBuffer, (int) xCord, (int)oldXCord );
        //printf("set HIGH\t");
    } else {
        //printf("set LOW\t");
        Gpio::setValue(Gpio::CLK, Gpio::LOW);
    }
    tick_count++;
}


int SongPlayer::getTempo() {
    return song->bars[0].tempo;
}

void SongPlayer::getResults(int (&s)[4]) {
    results[3] = count_notes*2 - results[0] - results[1] - results[2];
    s[0] = results[0];
    s[1] = results[1];
    s[2] = results[2];
    s[3] = results[3];
}

