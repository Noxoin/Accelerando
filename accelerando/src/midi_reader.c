#include "midi_reader.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

short key;
short major_minor;
short tsig_numerator;
short tsig_denominator;

int division;       // num of Ticks per beat
int ticks_per_metronome;
int tempo;

int numbars;

void processTrack(Track *track, Song **song) {
    int i, time, numNotes, bar_time, currBar;
    Song *s = *song;

    time = 0;
    numNotes = 0;
    bar_time = 0;
    currBar = 0;

    for(i = 0; i < track->numEvents; ++i) {
        Event *event;
        event = &(track->events[i]);
        time += event->delta_time;
        if(event->type == META) {
            int k;
            switch(event->subEventType) {
                case 88: //Time Signature
                    tsig_numerator = (short) event->data[0];
                    if(event->data[1] == (unsigned char) 1) {
                        tsig_denominator = 2;
                    } else if(event->data[1] == (unsigned char) 2) {
                        tsig_denominator = 4;
                    } else if (event->data[1] == (unsigned char) 3) {
                        tsig_denominator = 8;
                    } else if (event->data[1] == (unsigned char) 4) {
                        tsig_denominator = 16;
                    }
                    ticks_per_metronome = (int) event->data[2];
                    break;
                case 89: //Key Signature
                    key = (short) event->data[0];
                    major_minor = (short) event->data[1];
                    break;
                case 3: //Song Name
                    s->name_length = event->data_length;
                    if(s->name_length > 49) {
                        s->name_length = 49;
                    }
#ifdef DEBUG
                    printf("Length: %d\t", event->data_length);
                    printf("Song name: ");
#endif
                    for( k = 0; k < s->name_length; ++k) {
                        s->name[k] = (char) event->data[k];
#ifdef DEBUG
                        printf("%c", (char) event->data[k]);
#endif
                    }
                    s->name[k] = '\0';
#ifdef DEBUG
                    printf("\tk: %d", k);
                    printf("\n");
#endif
                    break;
                case 81: // Tempo
                    tempo = 0;
                    for( k = 0; k < event->data_length; ++k) {
                        tempo = tempo << 8 | (int) event->data[k];
                    }
                    break;
            }
        } else if (event->type == MIDI) {
            if(event->subEventType == 144) {
                int dur=0;
                int seek = i+1;
                Note note;
                note.value = (short) event->data[0];
                if(time-bar_time > division*tsig_numerator-division/4) {
                    bar_time += division*tsig_numerator;
                    currBar++;
                    numNotes=0;
                }
                note.time = (time-bar_time)/(division/2);
                if(note.time < 0) {
                    note.time = 0;
                }
                // On Press Event
                // Find when it is released
                for( ; seek < track->numEvents; ++seek) {
                    Event *e;
                    e = &(track->events[seek]);
                    dur += e->delta_time;
                    if(e->subEventType == 128 && note.value == (short) e->data[0]) {
                        break;
                    }
                }
                note.duration = (dur+division/4)/(division/2);
                s->bars[currBar].notes[numNotes++] = note;
            }
            s->bars[currBar].key = key;
            s->bars[currBar].major_minor = major_minor;
            s->bars[currBar].tsig_numerator = tsig_numerator;
            s->bars[currBar].tsig_denominator = tsig_denominator;
            s->bars[currBar].tempo = tempo;
            s->bars[currBar].length = (long) numNotes;
        }
        s->length = currBar + 1;
    }
                    

}

int getNextInt(FILE *file) {
    unsigned char *temp;
    int type;

    temp = (unsigned char*) malloc(sizeof(unsigned char)*4);
    fread(temp, 1, 4, file);

    type = (int) temp[0] << 24 | temp[1] << 16 | temp[2] << 8 | temp[3];

    free(temp);
    return type;
}

void loadTrack(unsigned char *buffer, int length, Track **track) {
    int event_count = 0;
    int l = 0, i = 0;
    Event *event;

    while(l < length) {
        int type, eventLen, subEventType;
        int delta_time = 0;
        
        do {
            delta_time = delta_time << 7 | ((int) buffer[l] & 127);
        } while(buffer[l++] & (unsigned char) 128);

        event = &((*track)->events[event_count++]);
        type = (int) buffer[l++];

        if(type == 255) {
            subEventType = (int) buffer[l++];
            eventLen = (int) buffer[l++];
            event->type = META;
        } else if(type >> 4 == 15) {
            subEventType = type;
            do {
                eventLen = eventLen << 7 | ((int) buffer[l] & 127);
            } while(buffer[l++] & (unsigned char) 128);
            event->type = SYS;
        } else {
            subEventType = type;
            eventLen = 1;
            if(type >> 4 != 12 && type >> 4 != 13) {
                eventLen++;
            }
            event->type = MIDI;
        }

        event->delta_time = delta_time;
        event->data_length = eventLen;
        event->subEventType = subEventType;

        for(i = 0; i < eventLen; ++i) {
            event->data[i] = buffer[l++];
        }

#ifdef DEBUG
        printf("delta: %d\t", event->delta_time);
        printf("eventType: %d\t", event->type);
        printf("subEventType: %02x\t", event->subEventType);
        for( i = 0; i < eventLen; ++i) {
            printf("%02x", event->data[i]);
        }
        printf("\n");
#endif

                    
    }
    (*track)->numEvents = event_count;

    return;

}

void loadMIDI(char *filename, Song *song) {
    FILE *file;
    long lSize;
    unsigned char * buffer;
    int type, length, l, trk_count;
    MIDI_File *midi;

#ifdef DEBUG
    int i;
#endif

    midi = (MIDI_File *) malloc(sizeof(MIDI_File));
    file = fopen(filename , "rb" );

    if (file==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    lSize = ftell(file);
    rewind(file);

    l = 0;
    trk_count = 0;

    type = getNextInt(file);
    length = getNextInt(file);

    buffer = (unsigned char*) malloc(sizeof(char)*length);
    fread(buffer, 1, length, file);

    l += length + 8;

#ifdef DEBUG
    printf("Track Type: %d\n", type);
    printf("Buffer Length: %d\n", length);
    for(i = 0; i < length; i++) {
        printf("%02x", buffer[i]);
        if(i%4 == 3) {
            printf(" ");
        }
        if(i%32 == 15) {
            printf("\n");
        }
    }
    printf("\n");
#endif

    //Header Track
    if(type != 1297377380) {
        printf("Expecting a header file\n");
        return;
    }

    midi->header_length = length;
    
    midi->format = (short) buffer[0]<<8 | buffer[1];
    midi->ntrks = (short) buffer[2]<<8 | buffer[3];
    midi->division = (short) buffer[4]<<8 | buffer[5];
    division = midi->division;
    //midi->tracks = (Track *) malloc(midi->ntrks * sizeof(Track));
#ifdef DEBUG
    //printf("midi->ntrks * sizeof(Track*) = %lu\n", midi->ntrks * sizeof(Track*));
    printf("Format: %d\tNtrks: %d\tTicksPerDivision: %d\n", midi->format, midi->ntrks, midi->division);
#endif


    //Process Tracks
    while(trk_count < midi->ntrks) {
        Track *track;
        type = getNextInt(file);
        length = getNextInt(file);
        l += length + 8;

#ifdef DEBUG
        printf("Track Count: %d\n", trk_count);
        printf("Track Type: %d\n", type);
        printf("Buffer Length: %d\n", length);
#endif

        buffer = (unsigned char*) malloc(sizeof(char)*length);
        fread(buffer, 1, length, file);

#ifdef DEBUG
        for(i = 0; i < length; i++) {
            printf("%02x", buffer[i]);
            if(i%4 == 3) {
                printf(" ");
            }
            if(i%32 == 31) {
                printf("\n");
            }
        }
        printf("\n");
#endif

        track = &(midi->tracks[trk_count]);
        track->type = type;
        track->length = length;
#ifdef DEBUG
        printf("Track #: %d\t Length: %d\n", trk_count, track->length);
#endif
        loadTrack(buffer, length, &track);
        processTrack(track, &song);
#ifdef DEBUG
        printf("\tlength: %d\tnumEvents: %d\n", track->length, track->numEvents);
#endif
        trk_count++;

        free(buffer);
    }

#ifdef DEBUG
    printf("header_length: %d\tformat: %d\tntrks: %d\tdivision: %d\n", 
                midi->header_length, 
                midi->format,
                midi->ntrks,
                midi->division
            );

    for(i = 0; i < midi->ntrks; ++i) {
        int j;
        Track *track;

        track = &(midi->tracks[i]);
        printf("Track %d:\n", i);
        printf("\tlength: %d\tnumEvents: %d\n", track->length, track->numEvents);
        for(j = 0; j < track->numEvents; ++j) {
            int k;
            Event *event;
            event = &(track->events[j]);
            printf("\t\tdelta: %d\tsubEventType: %d\t data: ", event->delta_time, event->subEventType);
            for( k = 0; k < event->data_length; ++k) {
                printf("%02x", event->data[k]);
            }
            printf("\n");

        }
    }

#endif

    free(midi);
    fclose(file);
}
