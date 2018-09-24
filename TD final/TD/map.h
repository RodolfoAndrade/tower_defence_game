#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "global.h"

enum map_section { TOWER_SECTION, ENEMY_SECTION, START_SECTION, FINISH_SECTION, NORMAL_T, SPLASH_T, POISON_T, SLOW_T, BEACON_T };

typedef struct dot
{
    int i;
    int j;
}Dot;

void load_Map(BITMAP* buffer, BITMAP* grass, BITMAP* sand, BITMAP* tower_images[], int map[][16]);

void load_Map_start_finish(BITMAP* buffer, BITMAP* grass, BITMAP* base_saida, BITMAP* base_entrada, int map[][16]);

void load_Towers(BITMAP* buffer, BITMAP* grass, BITMAP* tower_images[],int map[][16]);

enum movement_Directions { UP, RIGHT, DOWN, LEFT};

int* set_Trajectory(int map[][16], int path_tiles, Dot* start);

#endif // MAP_H_INCLUDED
