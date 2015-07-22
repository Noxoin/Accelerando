#include "gpio.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#define O_WRONLY 01

const std::string gpio_map[68] =    { "",     "pg3",  "pb19", "pb18", "pg6",  "pg5",  "pg4",  "pg1",  "pg2",  "pg0", 
                                 "ph14", "ph15", "pi6",  "pi5",  "pi4",  "pg11", "pg10", "pg9",  "pg8",  "pg7",
                                 "pe8",  "pe7",  "pe6",  "pe5",  "pe4",  "pi9",  "pi8",  "pi7",  "pd4",  "pd3",
                                 "pd2",  "pd1",  "pd0",  "pe11", "pe10", "pe9",  "pd12", "pd11", "pd10", "pd9",
                                 "pd8",  "pd7",  "pd6",  "pd5",  "pd20", "pd19", "pd18", "pd17", "pd16", "pd15",
                                 "pd14", "pd13", "pb2",  "pd25", "pd24", "pd26", "pd27", "pd23", "pd22", "pd21",
                                 "pi11", "pi13", "pi10", "pi12", "pb13", "pb11", "pb10", "ph7"
                               };

void setDirection(int gpio, int out) {
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

    sprintf(buf, "%d", Gpio::A2);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::AS2);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::B2);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::C3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::CS3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::D3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::DS3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::E3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::F3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::FS3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::G3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::GS3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::A3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::AS3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::B3);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::C4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::CS4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::D4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::DS4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::E4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::F4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::FS4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::G4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::GS4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::A4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::AS4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::B4);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::C5);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::CS5);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::D5);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::DS5);
    write(fd, buf, 100);

    sprintf(buf, "%d", Gpio::E5);
    write(fd, buf, 100);
}

void Gpio::init() {
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    allgpio(fd);
    close(fd);

    setDirection(Gpio::CLK, Gpio::OUT);
    setDirection(Gpio::A2, Gpio::OUT);
    setDirection(Gpio::AS2, Gpio::OUT);
    setDirection(Gpio::B2, Gpio::OUT);
    setDirection(Gpio::C3, Gpio::OUT);
    setDirection(Gpio::CS3, Gpio::OUT);
    setDirection(Gpio::D3, Gpio::OUT);
    setDirection(Gpio::DS3, Gpio::OUT);
    setDirection(Gpio::E3, Gpio::OUT);
    setDirection(Gpio::F3, Gpio::OUT);
    setDirection(Gpio::FS3, Gpio::OUT);
    setDirection(Gpio::G3, Gpio::OUT);
    setDirection(Gpio::GS3, Gpio::OUT);
    setDirection(Gpio::A3, Gpio::OUT);
    setDirection(Gpio::AS3, Gpio::OUT);
    setDirection(Gpio::B3, Gpio::OUT);
    setDirection(Gpio::C4, Gpio::OUT);
    setDirection(Gpio::CS4, Gpio::OUT);
    setDirection(Gpio::D4, Gpio::OUT);
    setDirection(Gpio::DS4, Gpio::OUT);
    setDirection(Gpio::E4, Gpio::OUT);
    setDirection(Gpio::F4, Gpio::OUT);
    setDirection(Gpio::FS4, Gpio::OUT);
    setDirection(Gpio::G4, Gpio::OUT);
    setDirection(Gpio::GS4, Gpio::OUT);
    setDirection(Gpio::A4, Gpio::OUT);
    setDirection(Gpio::AS4, Gpio::OUT);
    setDirection(Gpio::B4, Gpio::OUT);
    setDirection(Gpio::C5, Gpio::OUT);
    setDirection(Gpio::CS5, Gpio::OUT);
    setDirection(Gpio::D5, Gpio::OUT);
    setDirection(Gpio::DS5, Gpio::OUT);
    setDirection(Gpio::E5, Gpio::OUT);

    Gpio::reset();
}

void Gpio::reset(){

    Gpio::setValue(Gpio::CLK, Gpio::LOW);
    Gpio::setValue(Gpio::A2, Gpio::LOW);
    Gpio::setValue(Gpio::AS2, Gpio::LOW);
    Gpio::setValue(Gpio::B2, Gpio::LOW);
    Gpio::setValue(Gpio::C3, Gpio::LOW);
    Gpio::setValue(Gpio::CS3, Gpio::LOW);
    Gpio::setValue(Gpio::D3, Gpio::LOW);
    Gpio::setValue(Gpio::DS3, Gpio::LOW);
    Gpio::setValue(Gpio::E3, Gpio::LOW);
    Gpio::setValue(Gpio::F3, Gpio::LOW);
    Gpio::setValue(Gpio::FS3, Gpio::LOW);
    Gpio::setValue(Gpio::G3, Gpio::LOW);
    Gpio::setValue(Gpio::GS3, Gpio::LOW);
    Gpio::setValue(Gpio::A3, Gpio::LOW);
    Gpio::setValue(Gpio::AS3, Gpio::LOW);
    Gpio::setValue(Gpio::B3, Gpio::LOW);
    Gpio::setValue(Gpio::C4, Gpio::LOW);
    Gpio::setValue(Gpio::CS4, Gpio::LOW);
    Gpio::setValue(Gpio::D4, Gpio::LOW);
    Gpio::setValue(Gpio::DS4, Gpio::LOW);
    Gpio::setValue(Gpio::E4, Gpio::LOW);
    Gpio::setValue(Gpio::F4, Gpio::LOW);
    Gpio::setValue(Gpio::FS4, Gpio::LOW);
    Gpio::setValue(Gpio::G4, Gpio::LOW);
    Gpio::setValue(Gpio::GS4, Gpio::LOW);
    Gpio::setValue(Gpio::A4, Gpio::LOW);
    Gpio::setValue(Gpio::AS4, Gpio::LOW);
    Gpio::setValue(Gpio::B4, Gpio::LOW);
    Gpio::setValue(Gpio::C5, Gpio::LOW);
    Gpio::setValue(Gpio::CS5, Gpio::LOW);
    Gpio::setValue(Gpio::D5, Gpio::LOW);
    Gpio::setValue(Gpio::DS5, Gpio::LOW);
    Gpio::setValue(Gpio::E5, Gpio::LOW);
}

void Gpio::setValue(int gpio, int value) {
    int fd;
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%d_%s/value", gpio, gpio_map[gpio].c_str());
    //printf("Buffer: %s, Value: %d\n", buf, value);
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

int Gpio::getPin(int midiNote) {
    switch(midiNote) {
        case 45: return A2;
        case 46: return AS2;
        case 47: return B2;
        case 48: return C3;
        case 49: return CS3;
        case 50: return D3;
        case 51: return DS3;
        case 52: return E3;
        case 53: return F3;
        case 54: return FS3;
        case 55: return G3;
        case 56: return GS3;
        case 57: return A3;
        case 58: return AS3;
        case 59: return B3;
        case 60: return C4;
        case 61: return CS4;
        case 62: return D4;
        case 63: return DS4;
        case 64: return E4;
        case 65: return F4;
        case 66: return FS4;
        case 67: return G4;
        case 68: return GS4;
        case 69: return A4;
        case 70: return AS4;
        case 71: return B4;
        case 72: return C5;
        case 73: return CS5;
        case 74: return D5;
        case 75: return DS5;
        case 76: return E5;
        default: return 0;
    }
}

