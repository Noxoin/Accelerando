#include "../src/gpio.h"
#include <stdio.h>

int main() {
    printf("Starting test_gpio_map\n");
    gpio_init();
    setDirection(GPIO_CLK, GPIO_OUT);
    setValue(GPIO_CLK, GPIO_HIGH);
    return 0;
}
