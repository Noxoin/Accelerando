#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define O_WRONLY 01

char* gpio_map[68] =   { "",     "pg3",  "pb19", "pb18", "pg6",  "pg5",  "pg4",  "pg1",  "pg2",  "pg0", 
                         "ph14", "ph15", "pi6",  "pi5",  "pi4",  "pg11", "pg10", "pg9",  "pg8",  "pg7",
                         "pe8",  "pe7",  "pe6",  "pe5",  "pe4",  "pi9",  "pi8",  "pi7",  "pd4",  "pd3",
                         "pd2",  "pd1",  "pd0",  "pe11", "pe10", "pe9",  "pd12", "pd11", "pd10", "pd9",
                         "pd8",  "pd7",  "pd6",  "pd5",  "pd20", "pd19", "pd18", "pd17", "pd16", "pd15",
                         "pd14", "pd13", "pb2",  "pd25", "pd24", "pd26", "pd27", "pd23", "pd22", "pd21",
                         "pi11", "pi13", "pi10", "pi12", "pb13", "pb11", "pb10", "ph7"
                       };

void allgpio(int fd) {
    
    char buf[100];

    sprintf(buf, "%d", GPIO_CLK);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_A2);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_AS2);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_B2);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_C3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_CS3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_D3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_DS3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_E3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_F3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_FS3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_G3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_GS3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_A3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_AS3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_B3);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_C4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_CS4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_D4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_DS4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_E4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_F4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_FS4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_G4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_GS4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_A4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_AS4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_B4);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_C5);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_CS5);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_D5);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_DS5);
    write(fd, buf, strlen(buf));

    sprintf(buf, "%d", GPIO_E5);
    write(fd, buf, strlen(buf));
}

void gpio_init() {
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    allgpio(fd);
    close(fd);

    setDirection(GPIO_CLK, GPIO_OUT);
    setDirection(GPIO_A2, GPIO_OUT);
    setDirection(GPIO_AS2, GPIO_OUT);
    setDirection(GPIO_B2, GPIO_OUT);
    setDirection(GPIO_C3, GPIO_OUT);
    setDirection(GPIO_CS3, GPIO_OUT);
    setDirection(GPIO_D3, GPIO_OUT);
    setDirection(GPIO_DS3, GPIO_OUT);
    setDirection(GPIO_E3, GPIO_OUT);
    setDirection(GPIO_F3, GPIO_OUT);
    setDirection(GPIO_FS3, GPIO_OUT);
    setDirection(GPIO_G3, GPIO_OUT);
    setDirection(GPIO_GS3, GPIO_OUT);
    setDirection(GPIO_A3, GPIO_OUT);
    setDirection(GPIO_AS3, GPIO_OUT);
    setDirection(GPIO_B3, GPIO_OUT);
    setDirection(GPIO_C4, GPIO_OUT);
    setDirection(GPIO_CS4, GPIO_OUT);
    setDirection(GPIO_D4, GPIO_OUT);
    setDirection(GPIO_DS4, GPIO_OUT);
    setDirection(GPIO_E4, GPIO_OUT);
    setDirection(GPIO_F4, GPIO_OUT);
    setDirection(GPIO_FS4, GPIO_OUT);
    setDirection(GPIO_G4, GPIO_OUT);
    setDirection(GPIO_GS4, GPIO_OUT);
    setDirection(GPIO_A4, GPIO_OUT);
    setDirection(GPIO_AS4, GPIO_OUT);
    setDirection(GPIO_B4, GPIO_OUT);
    setDirection(GPIO_C5, GPIO_OUT);
    setDirection(GPIO_CS5, GPIO_OUT);
    setDirection(GPIO_D5, GPIO_OUT);
    setDirection(GPIO_DS5, GPIO_OUT);
    setDirection(GPIO_E5, GPIO_OUT);

    setValue(GPIO_CLK, GPIO_LOW);
    setValue(GPIO_A2, GPIO_LOW);
    setValue(GPIO_AS2, GPIO_LOW);
    setValue(GPIO_B2, GPIO_LOW);
    setValue(GPIO_C3, GPIO_LOW);
    setValue(GPIO_CS3, GPIO_LOW);
    setValue(GPIO_D3, GPIO_LOW);
    setValue(GPIO_DS3, GPIO_LOW);
    setValue(GPIO_E3, GPIO_LOW);
    setValue(GPIO_F3, GPIO_LOW);
    setValue(GPIO_FS3, GPIO_LOW);
    setValue(GPIO_G3, GPIO_LOW);
    setValue(GPIO_GS3, GPIO_LOW);
    setValue(GPIO_A3, GPIO_LOW);
    setValue(GPIO_AS3, GPIO_LOW);
    setValue(GPIO_B3, GPIO_LOW);
    setValue(GPIO_C4, GPIO_LOW);
    setValue(GPIO_CS4, GPIO_LOW);
    setValue(GPIO_D4, GPIO_LOW);
    setValue(GPIO_DS4, GPIO_LOW);
    setValue(GPIO_E4, GPIO_LOW);
    setValue(GPIO_F4, GPIO_LOW);
    setValue(GPIO_FS4, GPIO_LOW);
    setValue(GPIO_G4, GPIO_LOW);
    setValue(GPIO_GS4, GPIO_LOW);
    setValue(GPIO_A4, GPIO_LOW);
    setValue(GPIO_AS4, GPIO_LOW);
    setValue(GPIO_B4, GPIO_LOW);
    setValue(GPIO_C5, GPIO_LOW);
    setValue(GPIO_CS5, GPIO_LOW);
    setValue(GPIO_D5, GPIO_LOW);
    setValue(GPIO_DS5, GPIO_LOW);
    setValue(GPIO_E5, GPIO_LOW);
}

void setDirection(int gpio, int out) {
    int fd;
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%d_%s/direction", gpio, gpio_map[gpio]);
    fd = open(buf, O_WRONLY);
    if(out) {
        write(fd, "out", 3);
    } else {
        write(fd, "in", 2);
    }
    close(fd);
}

void setValue(int gpio, int value) {
    int fd;
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%d_%s/value", gpio, gpio_map[gpio]);
    fd = open(buf, O_WRONLY);
    if(value) {
        write(fd, "1", 1);
    } else {
        write(fd, "0", 1);
    }
    close(fd);
}

void gpio_clean() {
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    allgpio(fd);

    close(fd);
}


