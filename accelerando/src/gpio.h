
class Gpio {
    public:
        const static int OUT = 1;
        const static int IN = 0;
        const static int HIGH = 1;
        const static int LOW = 0;

        const static int CLK = 9;
        const static int A2 = 33;
        const static int AS2 = 34;
        const static int BF2 = 34;
        const static int B2 = 35;
        const static int C3 = 20;
        const static int CS3 = 21;
        const static int DF3 = 21;
        const static int D3 = 22;
        const static int DS3 = 23;
        const static int EF3 = 23;
        const static int E3 = 24;
        const static int F3 = 25;
        const static int FS3 = 26;
        const static int GF3 = 26;
        const static int G3 = 27;
        const static int GS3 = 12;
        const static int AF3 = 12;
        const static int A3 = 13;
        const static int AS3 = 14;
        const static int BF3 = 14;
        const static int B3 = 15;
        const static int C4 = 7;
        const static int CS4 = 17;
        const static int DF4 = 17;
        const static int D4 = 18;
        const static int DS4 = 19;
        const static int EF4 = 19;
        const static int E4 = 4;
        const static int F4 = 5;
        const static int FS4 = 6;
        const static int GF4 = 5;
        const static int G4 = 7;
        const static int GS4 = 8;
        const static int AF4 = 8;
        const static int A4 = 1;
        const static int AS4 = 2;
        const static int BF4 = 2;
        const static int B4 = 3;
        const static int C5 = 9;
        const static int CS5 = 10;
        const static int DF5 = 10;
        const static int D5 = 61;
        const static int DS5 = 60;
        const static int EF5 = 60;
        const static int E5 = 63;

        //Gpio();
        //~Gpio();
        static void init();
        static void setValue(int gpio, int value);
        static int getPin(int midiNote);
        static void clean();
        static void reset();
};

