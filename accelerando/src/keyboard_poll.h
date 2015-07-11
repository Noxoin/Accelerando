#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 1

#define NOTE_PRESSED 0
#define NOTE_RELEASED 1


int start_keyboard_polling();
void stop_keyboard_polling();
