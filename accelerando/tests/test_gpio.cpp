#include "../src/gpio.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Starting test_gpio_map\n");
    printf("pin is %d\n", Gpio::CLK);
    printf("high is %d\n", Gpio::HIGH);
    Gpio::init();
    for(int i = 0; i < 5; ++i) {
        Gpio::setValue(Gpio::CLK, Gpio::HIGH);
        //sleep(10);
        usleep(10000);
        Gpio::setValue(Gpio::CLK, Gpio::LOW);
        usleep(10000);
    }
    return 0;
}
