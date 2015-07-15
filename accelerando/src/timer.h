#include <SDL2/SDL.h>

class Timer {
    private:
        SDL_TimerID timer;
        Uint32 delay;
        bool enabled;
    public:
        Timer();
        ~Timer();
        void start(int ms);
        void stop();
};

