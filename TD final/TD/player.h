#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "global.h"
#include "tower.h"

typedef struct player
{
    int lifes;
    int cash;
    int rounds;
    struct tower** towers;
} Player;


#endif // PLAYER_H_INCLUDED
