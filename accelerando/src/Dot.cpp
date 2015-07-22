#include "Dot.h"

Dot::Dot()
{
    //Initialize the offsets
    mPosX = INITIAL_POS_X;
    mPosY = INITIAL_POS_Y;

    //Initialize the velocity
    //mVelX = 0;
    mVelY = INITIAL_VEL;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            //case SDLK_LEFT: mVelX -= DOT_VEL; break;
            //case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
        mOldPosY = mPosY;
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            //case SDLK_LEFT: mVelX += DOT_VEL; break;
            //case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move()
{
	/*
    //Move the dot left or right
    //mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }*/
    
    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    //or the dot moves more than MAX_DISTANCE
    if( ( mPosY < INITIAL_POS_Y) || ( mPosY  > FINAL_POS_Y ) 
        || (abs(mPosY - mOldPosY) > MAX_DISTANCE) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render(SDL_Renderer *renderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render( renderer, mPosX, mPosY );
}
