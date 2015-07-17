/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1067;
const int SCREEN_HEIGHT = 600;

enum score
{
    PERFECT,
    GOOD,
    BAD,
    MISS,
    TOTAL,
    SCORE_LEVELS,
};


//Texture wrapper class
class LTexture
{
	public:
		LTexture();

		~LTexture();

		bool loadFromFile( std::string path );
		
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

		void free();

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );
		
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;

LTexture gSCORE[SCORE_LEVELS];
LTexture gAccuracy;
LTexture gRank;


LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        	mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

string toString(int intValue){
    stringstream ss;
    ss << intValue;
    string str = ss.str();
    return str;
}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(string strPerfect, string strGood, string strBad, string strMiss, string strTotalScore, string strAccuracy, string strRank)
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "../res/fonts/font1.ttf", 50 );
	if( gFont == NULL )
	{
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
        if( !gRank.loadFromRenderedText( strRank, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
		if( !gSCORE[PERFECT].loadFromRenderedText( strPerfect, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        if( !gSCORE[GOOD].loadFromRenderedText(strGood, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        if( !gSCORE[BAD].loadFromRenderedText( strBad, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        if( !gSCORE[MISS].loadFromRenderedText( strMiss, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        if( !gSCORE[TOTAL].loadFromRenderedText( strTotalScore, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        if( !gAccuracy.loadFromRenderedText( strAccuracy, textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	for( int i = 0; i <=TOTAL; ++i )
	{
		gSCORE[ i ].free();
	}
    	gAccuracy.free();
	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		int idealHits;
		double idealScore;
		double intScore[SCORE_LEVELS];
		double accuracy;
		string strScore[SCORE_LEVELS];
		string strAccuracy;
		string strRank;
		
		idealHits = 100;
		idealScore = 300*idealHits;
		intScore[PERFECT] = 85;
		intScore[GOOD] = 5;
		intScore[BAD] = 8;
		intScore[MISS] = 2;
		intScore[TOTAL] = 300*intScore[PERFECT]+100*intScore[GOOD] +50*intScore[BAD];
		accuracy = (intScore[TOTAL]/idealScore)*100;
		//convert from int to string
		strScore[PERFECT] = "Perfect = 300 points x "+toString(intScore[PERFECT]);
		strScore[GOOD] = "Good = 100 points x "+toString(intScore[GOOD]);
		strScore[BAD] = "Bad = 50 points x "+toString(intScore[BAD]);
		strScore[MISS] = "Miss = 0 point x "+toString(intScore[MISS]);
		strScore[TOTAL] = "Total = "+toString(intScore[TOTAL])+" points";
		strAccuracy = "Accuracy = "+toString(accuracy)+" %";
		
		if (accuracy >95){
		    strRank = "Rank: S";
		}else if(accuracy>90){
		    strRank = "Rank: A";
		}else if(accuracy>80){
		    strRank = "Rank: B";
		}else if(accuracy>70){
		    strRank = "Rank: C";
		}else if(accuracy>60){
		    strRank = "Rank: D";
		}else{
		    strRank = "Failed";
		}
		

		if( !loadMedia(strScore[PERFECT], strScore[GOOD], strScore[BAD], 
            strScore[MISS], strScore[TOTAL], strAccuracy, strRank))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current frame
                		gRank.render( ( SCREEN_WIDTH - gRank.getWidth() ) / 2, ( SCREEN_HEIGHT - gRank.getHeight() ) / 2-200 );
				gSCORE[PERFECT].render( ( SCREEN_WIDTH - gSCORE[PERFECT].getWidth() ) / 2, ( SCREEN_HEIGHT - gSCORE[PERFECT].getHeight() ) / 2-150 );
                		gSCORE[GOOD].render( ( SCREEN_WIDTH - gSCORE[GOOD].getWidth() ) / 2, ( SCREEN_HEIGHT - gSCORE[GOOD].getHeight() ) / 2 -100);
                		gSCORE[BAD].render( ( SCREEN_WIDTH - gSCORE[BAD].getWidth() ) / 2, ( SCREEN_HEIGHT - gSCORE[BAD].getHeight() ) / 2 -50 );
                		gSCORE[MISS].render( ( SCREEN_WIDTH - gSCORE[MISS].getWidth() ) / 2, ( SCREEN_HEIGHT - gSCORE[MISS].getHeight() ) / 2 );
                		gSCORE[TOTAL].render( ( SCREEN_WIDTH - gSCORE[TOTAL].getWidth() ) / 2, ( SCREEN_HEIGHT - gSCORE[TOTAL].getHeight() ) / 2 +50);
                		gAccuracy.render( ( SCREEN_WIDTH - gAccuracy.getWidth() ) / 2, ( SCREEN_HEIGHT - gAccuracy.getHeight() ) / 2 +100);
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	close();

	return 0;
}
