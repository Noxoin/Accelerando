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

void loadMIDI(char *filename, Note *song) {
    FILE *file;
    long lSize;
    unsigned char * buffer;
    size_t result;
    int type, length, i, l = 0;

    file = fopen ( filename , "rb" );
    if (file==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (file , 0 , SEEK_END);
    lSize = ftell(file);
    rewind(file);

    l = 0;
    while(l < lSize) {
        type = getNextInt(file);
        length = getNextInt(file);
        l += length + 8;

#ifdef DEBUG
        printf("Track Type: %d\n", type);
        printf("Buffer Length: %d\n", length);
#endif

        buffer = (unsigned char*) malloc(sizeof(char)*length);
        fread(buffer, 1, length, file);
        //Track is now in Buffer

#ifdef DEBUG
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

        free(buffer);
    }

    fclose(file);
}
