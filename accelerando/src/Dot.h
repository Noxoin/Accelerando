#ifndef DOT_H
#define	DOT_H

#include "LTexture.h"

//dot texture
//LTexture gDotTexture;

//The dot that will move around on the screen
class Dot
{
    public:
		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move();

		//Shows the dot on the screen
		void render(LTexture& gDotTexture);

    private:
		//The X and Y offsets of the dot
		int mPosX, mPosY, mOldPosY;

		//The velocity of the dot
		int mVelY;//mVelX,
};

#endif