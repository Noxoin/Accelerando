#include "display_song_sel.h"

using namespace std;

//store file names
vector<string> file_names;

int readFileNames() {
	DIR *dp;
	struct dirent *dirp;
	char dirname[]="res/songs/";
        int num_of_songs;

	if((dp=opendir(dirname))==NULL){
		printf("opendir error");
		exit(1);
	}

	while((dirp=readdir(dp))!=NULL){
		if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0)||(strcmp(dirp->d_name,"fromInternet")==0))
		        continue;
		file_names.push_back(dirp->d_name);
	}
    num_of_songs = file_names.size();
    return num_of_songs;
}

void loadSelectScreenMedia(SDL_Renderer *renderer, TTF_Font *font, LTexture *gKeyPressSurfaces, LTexture *gDotTexture, LTexture *gSongName)
{
	//Load PNG texture
	//Load default surface
    if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT].loadFromFile( renderer, "res/screen/WelcomeScreen.png" ) )
	{
		printf( "Failed to load default image!\n" );
	}

	//Load selection screen
    if( !gKeyPressSurfaces[ KEY_PRESS_SURFACE_ENTER].loadFromFile( renderer, "res/screen/SongSelectionScreen.png" ) )
	{
		printf( "Failed to load song selection screen image!\n" );
	}
    
    //Load dot texture
	if( !gDotTexture[DOT_TEXTURE_NOTE].loadFromFile( renderer, "res/screen/dot.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
	}

    //Load song names
    SDL_Color textColor = { 255, 255, 255 };
    for(int i =0; i < file_names.size(); i++){
        if( !gSongName[i].loadFromRenderedText( renderer, font, file_names[i], textColor ) ){
            printf( "Failed to load song names!\n" );
        }
    }
    //Load names for empty space
    int empty_song = SONGS_PER_PAGE - (file_names.size()- ((int)(file_names.size()/SONGS_PER_PAGE))*SONGS_PER_PAGE);
    if (empty_song != SONGS_PER_PAGE){
        for(int i=0; i < empty_song; i++){
            if( !gSongName[file_names.size() + i].loadFromRenderedText( renderer, font, "--------------------", textColor ) ){
                printf( "Failed to names for empty space!\n" );
            }
        }
    }
}

vector<string> getFileNames() {
    return file_names;
}
