
typedef enum { MIDI, SYS, META } MidiEType;

typedef struct {
    MidiEType type;
    int delta_time;
    int data_length;
    int subEventType;
    unsigned char *data;
} Event;

typedef struct {
    int type;
    int length;
    int numEvents;
    Event *events;
} Track;

typedef struct {
    int header_length;
    short format;
    short ntrks;
    short division;
    Track *tracks;
} MIDI_File;

void loadMIDI();

