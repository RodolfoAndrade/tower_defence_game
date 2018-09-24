#include "shots.h"

double shot_Damage(double tower_damage) //passa tower->damage como parametro
{
    double damage = tower_damage;
    int random = rand() % 99;
    if(random <= 4) /*CRITICO*/
        damage *= 1.3;
    else if((random > 4) && (random <= 7)) /*FRACO*/
        damage *= 0.8;

    return damage;
}

Shot* insert_Shot(Shot* tower_shot_list,double tower_damage, int tower_x,int tower_y, Enemy* target) //mod //passa tower->shot_list e tower->damage e tower->x e tower->y
{
//    Shot* aux = tower_shot_list;
       /*CRIA UM TIRO*/
    Shot* new_shot = (Shot*) malloc(sizeof(Shot));
    if(!new_shot)
        printf("\nERRO DE ALOCACAO DO SHOT\n");

    new_shot->prox = tower_shot_list;
    new_shot->damage = shot_Damage(tower_damage);
    new_shot->speed = 0.12 * 36; //36 é tile
    new_shot->target_x = target->x;   //usar logo a prioridade pra determinar o target_x e target_y do inimigo? t
    new_shot->target_y = target->y;
    new_shot->origin_x = tower_x + 18;
    new_shot->origin_y = tower_y + 18;
    new_shot->wait = 0;

    /*COLOCA NO FIM DA LISTA*/
//    if(aux){
//        while(aux->prox)
//            aux = aux->prox;
//        aux->prox = new_shot;
//    }
//    else
//        tower_shot_list = new_shot;

    return new_shot;
}

Shot* remove_Shot(Shot* tower_shot_list)
{
    Shot* aux = tower_shot_list;
    tower_shot_list = tower_shot_list->prox;
    free(aux);
    return tower_shot_list;
}

int shot_Follow(Shot* shot, Enemy* target, SAMPLE* dead, Stats* stats, int* player_cash) //mod
{
    int pass_x = 0;
    int pass_y = 0;
    int head_shot = 0;
    shot->target_x = target->x;
    shot->target_y = target->y;
    double x = pow(shot->origin_x - shot->target_x, 2);
    double y = pow(shot->origin_y - shot->target_y, 2);
    double constant = sqrt((x+y)/pow(0.12*36, 2));
    int damage;

    x = sqrt(x);
    y = sqrt(y);

    if(shot->target_x > shot->origin_x)
    {
        shot->origin_x+=x/constant;
        if(shot->target_x <= shot->origin_x)
        pass_x = 1;
    }
    else
    {
        shot->origin_x-=x/constant;
        if(shot->target_x >= shot->origin_x)
        pass_x = 1;
    }

    if(shot->target_y > shot->origin_y)
    {
        shot->origin_y+=y/constant;
        if(shot->target_y <= shot->origin_y)
        pass_y = 1;
    }
    else
    {
        shot->origin_y-=y/constant;
        if(shot->target_y >= shot->origin_y)
        pass_y = 1;
    }

    if(pass_x == 1 && pass_y == 1)
    {
        if(target->hp > 0 && target->hp - shot->damage > 0)
        {
            damage = shot->damage - target->defense;
            if(damage < 5)
            {
                damage = 5;
            }
            if(rand() % 100 < 5)
            {
                damage *= 1.30;
            }
            else if(rand() % 100 < 3)
            {
                damage *= 0.8;
            }
            target->hp -= damage;

        }
        else{
            target->status = LOST;
            *player_cash += target->cash;
            play_sample(dead,128,0,1000,0);
            switch(target->type)
            {
                case NORMAL: stats->normal_killed +=1; break;
                case RESISTANT: stats->resistant_killed += 1; break;
                case IMUNE: stats->imune_killed += 1; break;
                case STRONG: stats->strong_killed += 1; break;
                case FAST: stats->fast_killed += 1; break;
            }
        }
        head_shot = 1;
    }
    return head_shot;
}
