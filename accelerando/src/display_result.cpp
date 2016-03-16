#include "display_result.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

string toString(int intValue){
    stringstream ss;
    ss << intValue;
    string str = ss.str();
    return str;
}

void loadBackground(SDL_Renderer *renderer, LTexture *gBackground){
    //Load the background image
    if( !gBackground->loadFromFile( renderer, "res/images/bg.png" ) ){
        printf( "Failed to load background image\n" );
    }
}

string* calculateResult(int idealHits, int *intScore){
        int idealScore;
        double accuracy;
        string *strScore;
        strScore = new string[SCORE_LEVELS];

        idealScore = 300*idealHits;
        intScore[TOTAL] = 300*intScore[PERFECT]+100*intScore[GOOD] +50*intScore[BAD];
        accuracy = (double)intScore[TOTAL]/idealScore*100;
        //convert from int to string
        strScore[PERFECT] = "Perfect = 300 points x "+toString(intScore[PERFECT]);
        strScore[GOOD] = "Good = 100 points x "+toString(intScore[GOOD]);
        strScore[BAD] = "Bad = 50 points x "+toString(intScore[BAD]);
        strScore[MISS] = "Miss = 0 point x "+toString(intScore[MISS]);
        strScore[TOTAL] = "Total = "+toString(intScore[TOTAL])+" points";
        strScore[ACCURACY] = "Accuracy = "+toString(accuracy)+" %";
        
        if (accuracy >95){
            strScore[RANK] = "Rank: S";
        }else if(accuracy>90){
            strScore[RANK] = "Rank: A";
        }else if(accuracy>80){
            strScore[RANK] = "Rank: B";
        }else if(accuracy>70){
            strScore[RANK] = "Rank: C";
        }else if(accuracy>60){
            strScore[RANK] = "Rank: D";
        }else{
            strScore[RANK] = "Failed";
        }
        return strScore;
}


void loadResultsMedia(SDL_Renderer *renderer, LTexture *gScore, string *strScore, TTF_Font *font){
	if( font == NULL )
	{
		printf( "Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !gScore[PERFECT].loadFromRenderedText( renderer, font, strScore[PERFECT], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[GOOD].loadFromRenderedText(renderer, font, strScore[GOOD], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[BAD].loadFromRenderedText(renderer, font, strScore[BAD], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[MISS].loadFromRenderedText(renderer, font, strScore[MISS], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[TOTAL].loadFromRenderedText(renderer, font, strScore[TOTAL], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[ACCURACY].loadFromRenderedText(renderer, font, strScore[ACCURACY], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[RANK].loadFromRenderedText(renderer, font, strScore[RANK], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
	}
}

void plot(SDL_Renderer *renderer, int *data_1, int *data_2, int *data_3, int size_of_data){
    int x = 20;
    int y = SCREEN_HEIGHT-20;
    int x_pu = (SCREEN_WIDTH-2*x)/(size_of_data-1);
    int y_pu = (SCREEN_HEIGHT/2 - 20)/100;
    SDL_Rect background;

    //SDL_Point start_xy = {x, y};
    //SDL_Point end_xy = {SCREEN_WIDTH - x, y};
    //SDL_Point line[] = {start_xy, end_xy};
    //SDL_RenderDrawLines(renderer, line, 3);

    //draw light grey background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 0xeb, 0xeb, 0xeb, 0x66);
    background.x = x;
    background.y = y - y_pu*100;
    background.w = x_pu*(size_of_data-1);
    background.h = y_pu*100;
    SDL_RenderFillRect(renderer, &background);

    //draw x-y axis
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    for(int k = 0; k < 2; k++){
        SDL_RenderDrawLine(renderer, x, y+k, x + x_pu*(size_of_data-1), y+k);
        SDL_RenderDrawLine(renderer, x+k, y - y_pu*100, x+k, y);
    }

    SDL_SetRenderDrawColor( renderer, 0xF0, 0x80, 0x80, 0xFF );
    for(int i = 0; i < size_of_data-1; i++){
        for(int j = 0; j < 3; j++){
            SDL_RenderDrawLine(renderer, x, y - y_pu*data_1[i] - 1 + j, x+x_pu, y - y_pu*data_1[i+1] -1 + j);
        }
        x += x_pu;
    }

    x = 20;
    SDL_SetRenderDrawColor( renderer, 0x46, 0x82, 0xB4, 0xFF );
    for(int i = 0; i < size_of_data-1; i++){
        for(int j = 0; j < 3; j++){
            SDL_RenderDrawLine(renderer, x, y - y_pu*data_2[i] - 1 + j, x+x_pu, y - y_pu*data_2[i+1] -1 + j);
        }
        x += x_pu;
    }

    x = 20;
    SDL_SetRenderDrawColor( renderer, 0x32, 0xCD, 0x32, 0xFF ); 
    for(int i = 0; i < size_of_data-1; i++){
        for(int j = 0; j < 3; j++){
            SDL_RenderDrawLine(renderer, x, y - y_pu*data_3[i] - 1 + j, x+x_pu, y - y_pu*data_3[i+1] -1 + j);
        }
        x += x_pu;
    }
}


