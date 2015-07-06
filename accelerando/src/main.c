#include <stdlib.h>
#include <stdio.h>
#include "midi_reader.h"

int main() {
    printf("Starting up Accelerando......\n");
    
    Song *song;
    loadMIDI("res/song.mid", song);
#ifdef DEBUG
    printf("DEBUG has been set..\n");
#endif
    return 0;
}
