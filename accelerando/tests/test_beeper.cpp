#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <queue>
#include <cmath>
#include "../src/beeper.h"
#include <stdlib.h>
#include <stdio.h>
#include "../src/midi_reader.h"

int main(int argc, char* argv[])
{
	int i,j;
	// Duration and Frequency of Key Press, for debugging
	int duration;
	int hz;
	Beeper b;
	short currNote=-1;

	SDL_Init(SDL_INIT_AUDIO);

	printf("Starting MIDI Reader.....\n");

	Song *song = (Song *) malloc(sizeof(Song));
	loadMIDI("res/ABCDEFG.mid", song);

	for( i = 0; i < song->length; ++i) {
	//for( i = 0; i < 1; ++i) {
		Bar bar = song->bars[i];
		//printf("Bar %03d:\t", i);
		//printf("Key: %d\t TimeSig: %d/%d\t Tempo: %d\n\t", bar.key, bar.tsig_numerator, bar.tsig_denominator, (unsigned) bar.tempo);
		for( j = 0; j < bar.length; ++j) {
			Note note = bar.notes[j];
			// Piano Key to Frequency Conversion: https://en.wikipedia.org/wiki/Piano_key_frequencies
			// This If statement is a hack for now just for testing			
			if (currNote != note.time) {
				printf(" {%d,%d,%d}", note.time, note.duration, note.value);
				hz = (float) pow(2, (float) (note.value-49)/12)*440;
				/*printf("hz: %d \n",hz);
				printf("note.value-49: %d \n", (note.value-49));
				printf("/12: %f \n", (float) (note.value-49)/12);
				printf("pow: %f \n", (float) pow(2, (float) (note.value-49)/12));
				printf("*400: %f \n", (float) 400*pow(2, (float) (note.value-49)/12));*/
				b.beep(hz, 30*1000/bar.tempo*note.duration);
				currNote = note.time;
				b.wait();
			}
			//b.wait();
		}
		currNote = -1;
		printf("\n");
	}
	//b.wait();
	free(song);

	return 0;
}
