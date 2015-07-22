#ifndef LTEXTURE_H
#define	LTEXTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "SDLSetup.h"

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( SDL_Renderer *renderer, std::string path );
        
		//Creates image from font string
		bool loadFromRenderedText( SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( SDL_Renderer *renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		SDL_Texture* mTexture;

	private:
		//The actual hardware texture

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif
