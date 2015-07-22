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

//calculate result and return string
string* calculateResult(int idealHits, int *intScore);

//load text texture
void loadResultsMedia(SDL_Renderer *renderer, LTexture *gScore, string *strScore, TTF_Font *font);


#endif
