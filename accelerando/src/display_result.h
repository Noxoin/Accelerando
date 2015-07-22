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
void loadMedia(LTexture *gScore, string *strScore);


#endif