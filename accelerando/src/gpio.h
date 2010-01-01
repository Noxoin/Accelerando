
class Gpio {
    public:
        const static int OUT = 1;
        const static int IN = 0;
        const static int HIGH = 1;
        const static int LOW = 0;

        const static int CLK = 31;

        const static int LED_PINS[20];
        const static int PRESSED_PINS[20];

        //Gpio();
        //~Gpio();
        static void init();
        static void setValue(int gpio, int value);
        static int getLEDPin(int midiNote);
        static int getPressedPin(int midiNote);
        static void clean();
        static void reset();
};

