#define GPIO_CLK 11

#define GPIO_A2 33

#define GPIO_AS2 34
#define GPIO_BF2 34

#define GPIO_B2 35

#define GPIO_C3 20

#define GPIO_CS3 21
#define GPIO_DF3 21

#define GPIO_D3 22

#define GPIO_DS3 23
#define GPIO_EF3 23

#define GPIO_E3 24

#define GPIO_F3 25

#define GPIO_FS3 26
#define GPIO_GF3 26

#define GPIO_G3 27

#define GPIO_GS3 12
#define GPIO_AF3 12

#define GPIO_A3 13

#define GPIO_AS3 14
#define GPIO_BF3 14

#define GPIO_B3 15

#define GPIO_C4 16

#define GPIO_CS4 17
#define GPIO_DF4 17

#define GPIO_D4 18

#define GPIO_DS4 19
#define GPIO_EF4 19

#define GPIO_E4 4

#define GPIO_F4 5

#define GPIO_FS4 6
#define GPIO_GF4 6

#define GPIO_G4 7

#define GPIO_GS4 8
#define GPIO_AF4 8

#define GPIO_A4 1

#define GPIO_AS4 2
#define GPIO_BF4 2

#define GPIO_B4 3

#define GPIO_C5 9

#define GPIO_CS5 10
#define GPIO_DF5 10

#define GPIO_D5 61

#define GPIO_DS5 60
#define GPIO_EF5 60

#define GPIO_E5 63

// GPIO Direction
#define GPIO_OUT 1
#define GPIO_IN 0

// GPIO VALUE
#define GPIO_HIGH 1
#define GPIO_LOW 0

void gpio_init();
void setDirection(int gpio, int out);
void setValue(int gpio, int value);
void gpio_clean();
