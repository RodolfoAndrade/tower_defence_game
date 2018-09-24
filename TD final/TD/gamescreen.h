#ifndef GAMESCREEN_H_INCLUDED
#define GAMESCREEN_H_INCLUDED
#include "global.h"

enum mode { BUILD, CHOOSE_TOWER, NONE };

typedef struct stats
{
    int normal_killed;
    int resistant_killed;
    int strong_killed;
    int fast_killed;
    int imune_killed;
    int money_spent;
}Stats;


void gamescreen(int* screen_state);


#endif // GAMESCREEN_H_INCLUDED
