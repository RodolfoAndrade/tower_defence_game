#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED
#include "global.h"
#include "map.h"

enum enemy_type { NORMAL, RESISTANT, STRONG, FAST, IMUNE };

enum status{ WON, LOST, IN_GAME, OUT_GAME };

typedef struct enemy
{
    struct enemy* prox;
    double x;
    double y;
    int lvl;
    double hp;
    int defense;
    double speed;
    int status;
    int cash;
    double pace_counter;
    int type;
    int cont;
    double add;
    int slowed;
    int contado;
} Enemy;

Enemy* insert_at_End(Enemy* enemy_list, int enumType, int level, double spawn_x, double spawn_y);

Enemy* put_enemy_Info(int level, double spawn_x, double spawn_y, int enumType);

Enemy* movement_Updater(int* pause,Enemy* aux,int path_tiles,int* player_lifes,int* frame_atual,BITMAP* grass,int movement_instructions[],int* view,
                        int tempo_troca,int num_frame,int j,BITMAP* buffer,int frame_w,int frame_h,BITMAP* enemy_images[]);

int check_EOR(Enemy* enemy_list);

#endif // ENEMY_H_INCLUDED
