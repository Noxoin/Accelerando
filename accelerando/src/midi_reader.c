#include "midi_reader.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

int getNextInt(FILE *file) {
    unsigned char *temp;
    int type;

    temp = (unsigned char*) malloc(sizeof(unsigned char)*4);
    fread(temp, 1, 4, file);

    type = (int) temp[0] << 24 | temp[1] << 16 | temp[2] << 8 | temp[3];

    free(temp);
    return type;
}

void processTrack(unsigned char *buffer, int length, Track **track) {
    int event_count = 0;
    int l = 0, i = 0;
    Event *event;
    Event *events;


    events = (Event *) malloc(sizeof(Event)*length/2);
    while(l < length) {
        int type, eventLen, subEventType;
        int delta_time = 0;

        if(event_count >= length/2) {
            printf("Too many events, need to realloc for more memory");
        }
        
        do {
            delta_time = delta_time << 7 | ((int) buffer[l] & 127);
        } while(buffer[l++] & (unsigned char) 128);

        *event = events[event_count++];
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

        event->data = (unsigned char *) malloc(sizeof(unsigned char)*eventLen);
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
    (*track)->events = events;
    return;

}

void loadMIDI(char *filename, Note *song) {
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
    midi->tracks = (Track *) malloc(midi->ntrks * sizeof(Track));
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
        //Data is now in Buffer

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

        *track = midi->tracks[trk_count];
        track->type = type;
        track->length = length;
#ifdef DEBUG
        printf("Track #: %d\t Length: %d\n", trk_count, track->length);
#endif
        processTrack(buffer, length, &track);
        printf("\tlength: %d\tnumEvents: %d\t", track->length, track->numEvents);
        trk_count++;

        free(buffer);
    }

/*
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

        *track = midi->tracks[i];
        printf("Track %d:\n", i);
        printf("\tlength: %d\tnumEvents: %d\t", track->length, track->numEvents);
    }

#endif
*/

    free(midi);
    fclose(file);
}
