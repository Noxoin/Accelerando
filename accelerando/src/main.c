#include <stdlib.h>
#include <stdio.h>
#include "midi_reader.h"
#include "song.h"

int main() {
    printf("Starting up Accelerando......\n");
    
    Note song[10];
    loadMIDI("res/song.mid", song);
#ifdef DEBUG
    printf("DEBUG has been set..\n");
#endif
    return 0;
}
