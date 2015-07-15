#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class KeyboardPoll {
    public:
        KeyboardPoll();
        ~KeyboardPoll();
        int start();
        void stop();
};

