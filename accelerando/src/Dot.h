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
		void render(SDL_Renderer *renderer, LTexture& gDotTexture);

    private:
		//The X and Y offsets of the dot
		double mPosX, mPosY, mOldPosY;

		//The velocity of the dot
		double mVelY;//mVelX,
};

#endif
