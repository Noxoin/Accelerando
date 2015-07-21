
typedef struct {
    short time;
    short duration;
    short value;
} Note;

typedef struct {
    short key;
    short major_minor;
    short tsig_numerator;
    short tsig_denominator;
    short tempo;
    long length;
    Note notes[100];
} Bar;

typedef struct {
    int length;
    int name_length;
    char name[50];
    Bar bars[1000];
} Song;

