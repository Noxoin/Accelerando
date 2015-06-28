#include <stdlib.h>
#include <stdio.h>
#include "../src/midi_reader.h"
#include "../src/song.h"

int main() {
    int i,j;
    printf("Starting Test MIDI Reader.....\n");
    
    Song *song = (Song *) malloc(sizeof(Song));
    loadMIDI("res/ABCDEFG.mid", song);
    printf("Song name: ");
    for(i = 0; i < song->name_length; ++i) {
        printf("%c", song->name[i]);
    }
    printf("\n");
    printf("Length of song: %d\n", song->length);
    for( i = 0; i < song->length; ++i) {
        Bar bar = song->bars[i];
        printf("Bar %03d:\t", i);
        printf("Key: %d\t TimeSig: %d/%d\t Tempo: %d\n\t", bar.key, bar.tsig_numerator, bar.tsig_denominator, (unsigned) bar.tempo);
        for( j = 0; j < bar.length; ++j) {
            Note note = bar.notes[j];
            printf(" {%d,%d,%d}", note.time, note.duration, note.value);
        }
        printf("\n");
    }

    free(song);

    song = (Song *) malloc(sizeof(Song));

    loadMIDI("res/TestMusicLv1C+_arpeggios_hs_8n.mid", song);
    printf("Song name: ");
    for(i = 0; i < song->name_length; ++i) {
        printf("%c", song->name[i]);
    }
    printf("\n");
    printf("Length of song: %d\n", song->length);
    for( i = 0; i < song->length; ++i) {
        Bar bar = song->bars[i];
        printf("Bar %03d:\t", i);
        printf("Key: %d\t TimeSig: %d/%d\t Tempo: %d\n\t", bar.key, bar.tsig_numerator, bar.tsig_denominator, bar.tempo);
        for( j = 0; j < bar.length; ++j) {
            Note note = bar.notes[j];
            printf(" {%d,%d,%d}", note.time, note.duration, note.value);
        }
        printf("\n");
    }


    free(song);
    return 0;
}
