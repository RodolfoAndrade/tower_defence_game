#ifndef TOWER_H_INCLUDED
#define TOWER_H_INCLUDED
#include "global.h"
#include "gamescreen.h"
#include "player.h"
#include "shots.h"
#include "enemy.h"
#include "round.h"

enum tower_type { NORMAL_TOWER = 4, SPLASH_TOWER, POISON_TOWER, SLOW_TOWER, BEACON_TOWER };

enum priority_type { MORE_HP, LESS_HP, FAR, CLOSE };

typedef struct tower
{
    int x;
    int y;
    int priority;
    int bless;
    int rank;
    int type;
    int rest;
    double damage;
    int area_damage_mult;
    double slow_tax;
    float range;
    double radius;
    double normal_bless;
    double splash_bless;
    double slow_bless;
    double poison_bless;
    int cost;
    int shooted;
    int periodo;
    Enemy* target;
    Shot* shot_list;
} Tower;

Tower* create_Tower(int selected_tower_type, int x, int y, Round* round);

Tower* search_Tower(int x, int y, Tower* player_towers[], int qtd_towers);

Tower* increase_Stats(Tower* chosen_tower);

int locate_Tower(Tower* tower, Tower* player_towers[], int qtd_towers);

Tower** sell_Tower(int x, int y, Tower* player_towers[], int* qtd_towers);

Tower** new_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y);

Tower** near_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y);

Tower** sold_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y);

Tower** priority_Change(Tower* player_towers[], int qtd_towers, int priority, int chosen_tower_type);

double distance(Tower* tower, Enemy* enemy);

int check_Target(Tower* tower, Enemy* enemy_list);

int select_Target(Tower* tower,Enemy* enemy_list);

void poison_Effect(Tower* tower, Enemy* enemy_list,BITMAP* buffer,SAMPLE* dead2,Stats* stats);

void establish_Connection(Tower* tower,  BITMAP* buffer, Enemy* enemy_list);

double distance_splash(Shot* shot, Enemy* enemy);

void splash_effect(Tower* tower, Enemy* enemy_list, Shot* shot, BITMAP* buffer);

void reset(Tower* tower, BITMAP* buffer, Enemy* enemy_list);

#endif // TOWER_H_INCLUDED
