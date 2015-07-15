#include "keyboard_poll.h"
#include <SDL2/SDL.h>
#include "userEvent.h"

#define NUM_THREADS 1

int poll_status = 0;
int thread_count = 0;

pthread_t thread_list[NUM_THREADS];

void errormessage(const char *format, ...);

void *polling(void *arg) {
    int status;
    int mode = SND_RAWMIDI_SYNC;
    snd_rawmidi_t* midiin = NULL;
    const char* portname = "hw:2,0,0";
    char buffer[1];

    printf("I'm here\n");

    if ((status = snd_rawmidi_open(&midiin, NULL, portname, mode)) < 0) {
        errormessage("Problem opening MIDI input: %s", snd_strerror(status));
        exit(1);
    }

    while(poll_status) {
        if((status = snd_rawmidi_read(midiin, buffer, 1)) < 0) {
            errormessage("Problem reading MIDI input: %s", snd_strerror(status));
        }
        if((unsigned char) buffer[0] >= 0x80) {
            unsigned char event = buffer[0];
            if (event == 0x80 || event == 0x90) {
                if((status = snd_rawmidi_read(midiin, buffer, 1)) < 0) {
                    errormessage("Problem reading MIDI input: %s", snd_strerror(status));
                }
                unsigned char *note = (unsigned char*)malloc(sizeof(unsigned char));
                *note = buffer[0];
                if((status = snd_rawmidi_read(midiin, buffer, 1)) < 0) {
                    errormessage("Problem reading MIDI input: %s", snd_strerror(status));
                }
                SDL_Event e;
                e.type = SDL_USEREVENT;
                e.user.data1 = note;
                if(buffer[0] == 0) {
                    // Generate SDL_KeyBoardPressed_Event
                    e.user.code = (int) NOTE_RELEASED;
                    printf("Note released: %02d\n", *note);
                } else {
                    // Generate SDL_KeyboardReleased Event
                    e.user.code = (int) NOTE_PRESSED;
                    printf("Note pressed: %02d\n", *note);
                }
                SDL_PushEvent(&e);
            }
        } 
        fflush(stdout);
    }
    snd_rawmidi_close(midiin);
    midiin = NULL;

    pthread_exit(NULL);
}

KeyboardPoll::KeyboardPoll() {}
KeyboardPoll::~KeyboardPoll() {}

int KeyboardPoll::start() {
    int rc;
    poll_status = 1;
    if (thread_count == 1) {
        printf("polling thread already started\n");
        return EXIT_FAILURE;
    }
    if ((rc = pthread_create(&thread_list[0], NULL, polling, NULL))) {
        fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
        return EXIT_FAILURE;
    }
    thread_count++;
    printf("Thread should be started\n");
    return EXIT_SUCCESS;
}

void KeyboardPoll::stop() {
    poll_status= 0;
    pthread_join(thread_list[0], NULL);
}

void errormessage(const char *format, ...) {
    printf("OMFG something went wrong :( in keyboard polling\n");
}
