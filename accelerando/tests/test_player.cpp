#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <queue>
#include <cmath>
#include "../src/player.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_AUDIO);

    // Duration and Frequency of Key Press
    int duration = 1000;
    double Hz = 440;

    Beeper b;
    b.beep(Hz, duration);
    b.wait();

    return 0;
}
