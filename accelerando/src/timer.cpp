#include "timer.h"
#include "userEvent.h"

Uint32 timerCallback(Uint32 interval, void *params) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = TIMER_EVENT;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return (interval);
}

Timer::Timer() {
}

Timer::~Timer() {}

void Timer::start(int ms) {
    if(!enabled) {
        delay = ms;
        timer = SDL_AddTimer(delay, timerCallback, NULL);
        enabled = true;
    }
}

void Timer::stop() {
    SDL_RemoveTimer(timer);
    enabled = false;
}

