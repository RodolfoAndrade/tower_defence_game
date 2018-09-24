#ifndef ROUND_H_INCLUDED
#define ROUND_H_INCLUDED
#include "global.h"
#include "enemy.h"

typedef struct round
{
    int number;
    int* rest_vector;
    int normal_built;
    int splash_built;
    int poison_built;
    int slow_built;
    int beacon_built;
    struct enemy* enemy_list;
    struct round* prox;
} Round;

Round* insert_round_at_end(Round* round, int* rest_vector, Enemy* enemy_list, int current_round);

#endif // ROUND_H_INCLUDED
