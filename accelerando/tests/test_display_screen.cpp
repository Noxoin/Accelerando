#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../src/display_screen.h"

using namespace std;

int main()
{
	//Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Load media
		if( !loadMedia() ){
			printf( "Failed to load media!\n" );
		}
		else{	
            run();
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}