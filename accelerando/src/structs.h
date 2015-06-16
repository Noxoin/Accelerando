
typedef struct {
    long time;
    short duration;
    short value;
} Note;

typedef struct {
    short key;
    short tsig_numerator;
    short tsig_denominator;
    long length;
    Note *notes;
} Song;

