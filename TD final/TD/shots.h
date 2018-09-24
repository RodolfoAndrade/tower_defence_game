#ifndef SHOTS_H_INCLUDED
#define SHOTS_H_INCLUDED
#include "global.h"
#include "enemy.h"
#include "gamescreen.h"

typedef struct shot
{
    double damage;
    int target_x;
    int target_y;
    int origin_x;
    int origin_y;
    double speed;
    struct shot* prox;
    int ticks;
    int wait;
}Shot;

double shot_Damage(double tower_damage);

Shot* insert_Shot(Shot* tower_shot_list,double tower_damage, int tower_x,int tower_y, Enemy* enemy_list); //mod

Shot* remove_Shot(Shot* tower_shot_list);

int shot_Follow(Shot* tower_shot_list, Enemy* enemy_list, SAMPLE* dead, Stats* stats, int* player_cash); //mod

#endif // SHOTS_H_INCLUDED
