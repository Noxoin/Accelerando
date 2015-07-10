#include "../src/keyboard_poll.h"
#include <unistd.h>

int main() {
    int i;
    start_keyboard_polling();
    for(i = 0; i < 20; ++i) {
        printf("This is second %d\n", i);
        sleep(1);
    }
    stop_keyboard_polling();
}
