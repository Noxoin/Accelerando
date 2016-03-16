#include "gpio.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#define O_WRONLY 01
const int Gpio::LED_PINS[] = {
                              30, 28, 42, 39, 37, 51, 49, 46, 44, 58, 56, 54, // C4 - B4
                              52, 65, 62, 60, 48 //C5 - E5
                            };
                                
const int Gpio::PRESSED_PINS[] = {
                                  31, 29, 43, 40, 38, 36, 50, 47, 45, 59, 57, 55, // C4 - B4
                                  53, 66, 63, 61, 41 //C5 - E5
                                };

const std::string gpio_map[68] =    { "",     "pg3",  "pb19", "pb18", "pg6",  "pg5",  "pg4",  "pg1",  "pg2",  "pg0", 
                                 "ph14", "ph15", "pi6",  "pi5",  "pi4",  "pg11", "pg10", "pg9",  "pg8",  "pg7",
                                 "pe8",  "pe7",  "pe6",  "pe5",  "pe4",  "pi9",  "pi8",  "pi7",  "pd4",  "pd3",
                                 "pd2",  "pd1",  "pd0",  "pe11", "pe10", "pe9",  "pd12", "pd11", "pd10", "pd9",
                                 "pd8",  "pd7",  "pd6",  "pd5",  "pd20", "pd19", "pd18", "pd17", "pd16", "pd15",
                                 "pd14", "pd13", "pb2",  "pd25", "pd24", "pd26", "pd27", "pd23", "pd22", "pd21",
                                 "pi11", "pi13", "pi10", "pi12", "pb13", "pb11", "pb10", "ph7"
                               };

void setDirection(int gpio, int out) {
    if(gpio < 0) {
        return;
    }
    int fd;
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%d_%s/direction", gpio, gpio_map[gpio].c_str());
    //printf("buffer: %s\n", buf);
    fd = open(buf, O_WRONLY);
    if(out) {
        write(fd, "out", 3);
    } else {
        write(fd, "in", 2);
    }
    close(fd);
}

void allgpio(int fd) {
    
    char buf[100];

    sprintf(buf, "%d", Gpio::CLK);
    write(fd, buf, 100);

    for(int i = 0; i < 17; ++i) {
        sprintf(buf, "%d", Gpio::LED_PINS[i]);
        write(fd, buf, 100);
        sprintf(buf, "%d", Gpio::PRESSED_PINS[i]);
        write(fd, buf, 100);
    }
}

void Gpio::init() {
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    allgpio(fd);
    close(fd);

    setDirection(Gpio::CLK, Gpio::OUT);
    for(int i = 0; i < 17; ++i) {
        setDirection(Gpio::LED_PINS[i], Gpio::OUT);
        setDirection(Gpio::PRESSED_PINS[i], Gpio::OUT);
    }
    Gpio::reset();
}

void Gpio::reset(){

    Gpio::setValue(Gpio::CLK, Gpio::LOW);
    for(int i = 0; i < 17; ++i) {
        Gpio::setValue(Gpio::LED_PINS[i], Gpio::LOW);
        Gpio::setValue(Gpio::PRESSED_PINS[i], Gpio::LOW);
    }
}

void Gpio::setValue(int gpio, int value) {
    if(gpio < 0) {
        return;
    }
    int fd;
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%d_%s/value", gpio, gpio_map[gpio].c_str());

    fd = open(buf, O_WRONLY);
    if(value) {
        write(fd, "1", 1);
    } else {
        write(fd, "0", 1);
    }
    close(fd);
}

void Gpio::clean() {
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    allgpio(fd);

    close(fd);
}

int Gpio::getLEDPin(int midiNote) {
    if(midiNote < 57 || midiNote - 60 > 16) {
        printf("No such Key\n");
        return -1;
    }
    return LED_PINS[midiNote-60];
}

int Gpio::getPressedPin(int midiNote) {
    if(midiNote < 57 || midiNote - 60 > 16) {
        printf("No such Key\n");
        return -1;
    }
    return PRESSED_PINS[midiNote-60];
}

