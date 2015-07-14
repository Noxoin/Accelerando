#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "userEvent.h"

class KeyboardPoll {
    public:
        KeyboardPoll();
        ~KeyboardPoll();
        int start();
        void stop();
};

