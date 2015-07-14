#include "../src/gpio.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Starting test_gpio_map\n");
    Gpio g;
    printf("pin is %d\n", Gpio::CLK);
    printf("high is %d\n", Gpio::HIGH);
    g.setValue(Gpio::CLK, Gpio::HIGH);
    sleep(30);
    return 0;
}
