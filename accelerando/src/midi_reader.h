#include "song.h"

typedef enum { MIDI, SYS, META } MidiEType;

typedef struct {
    MidiEType type;
    int delta_time;
    int data_length;
    int subEventType;
    unsigned char data[10];
} Event;

typedef struct {
    int type;
    int length;
    int numEvents;
    Event events[1000];
} Track;

typedef struct {
    int header_length;
    short format;
    short ntrks;
    short division;
    Track tracks[16];
} MIDI_File;

void loadMIDI(char *filename, Song *song);

