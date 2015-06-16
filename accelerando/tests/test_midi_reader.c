#include <stdlib.h>
#include <stdio.h>
#include "../src/midi_reader.h"
#include "../src/structs.h"

int main() {
    printf("Starting Test MIDI Reader.....\n");
    
    Note song[10];
    loadMIDI("res/song.mid", song);
    return 0;
}
