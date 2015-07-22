#include "display_result.h"

string toString(int intValue){
    stringstream ss;
    ss << intValue;
    string str = ss.str();
    return str;
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


void loadMedia(LTexture *gScore, string *strScore){
	if( gFont == NULL )
	{
		printf( "Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !gScore[PERFECT].loadFromRenderedText( strScore[PERFECT], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[GOOD].loadFromRenderedText(strScore[GOOD], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[BAD].loadFromRenderedText( strScore[BAD], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[MISS].loadFromRenderedText( strScore[MISS], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[TOTAL].loadFromRenderedText( strScore[TOTAL], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[ACCURACY].loadFromRenderedText( strScore[ACCURACY], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
        if( !gScore[RANK].loadFromRenderedText( strScore[RANK], textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
	}
}