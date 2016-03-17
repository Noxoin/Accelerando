#ifndef DISPLAY_RESULT_H
#define	DISPLAY_RESULT_H

#include <sstream>
#include <cmath>
#include "LTexture.h"

using namespace std;

enum score
{
    PERFECT,
    GOOD,
    BAD,
    MISS,
    TOTAL,
    ACCURACY,
    RANK,
    SCORE_LEVELS,
};

//convert from integer to string
string toString(int intValue);

//load background picture
void loadBackground(SDL_Renderer *renderer, LTexture *gBackground);

//calculate result and return string
string* calculateResult(int idealHits, int *intScore);

//load text texture
void loadResultsMedia(SDL_Renderer *renderer, LTexture *gScore, string *strScore, TTF_Font *font);

//calculate data
int* calData(int *data, int size_of_data);

//average
int* calAvg(int *data_1, int *data_2, int size_of_data);

//plot the data
void plot(SDL_Renderer *renderer, int *data_1, int *data_2, int *data_3, int size_of_data);

#endif
