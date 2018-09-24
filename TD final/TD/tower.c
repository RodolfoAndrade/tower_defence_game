#include "tower.h"

Tower* create_Tower(int selected_tower_type, int x, int y, Round* round)
{
    Tower* new_tower = (Tower*) calloc (1,sizeof(Tower));
    new_tower->rank = 1;
    new_tower->bless = 0;
    new_tower->type = selected_tower_type;
    new_tower->x = x;
    new_tower->y = y;
    new_tower->priority = CLOSE;
    new_tower->shot_list = NULL;
    new_tower->target = NULL;
    new_tower->shooted = 0;
    switch(selected_tower_type)
    {
        case NORMAL_TOWER:
            round->normal_built += 1;
            new_tower->rest = 16;
            new_tower->damage = 25;
            new_tower->range = 2;
            new_tower->cost = 80;
            break;
        case SPLASH_TOWER:
            round->splash_built += 1;
            new_tower->rest = 22;
            new_tower->damage = 40;
            new_tower->area_damage_mult = 0.4;
            new_tower->range = 1.5;
            new_tower->radius = 1;
            new_tower->cost = 200;
            break;
        case POISON_TOWER:
            round->poison_built += 1;
            new_tower->damage = 1;
            new_tower->range = 1.5;
            new_tower->cost = 160;
            break;
        case SLOW_TOWER:
            round->slow_built += 1;
            new_tower->slow_tax = 0.35;
            new_tower->range = 1.5;
            new_tower->cost = 140;
            break;
        case BEACON_TOWER:
            round->beacon_built += 1;
            new_tower->normal_bless = 1.05;
            new_tower->splash_bless = 0.5;
            new_tower->slow_bless = 0.05;
            new_tower->poison_bless = 1;
            new_tower->range = 2;
            new_tower->cost = 400;
            break;
    }
    return new_tower;
}

Tower* search_Tower(int x, int y, Tower* player_towers[], int qtd_towers)
{
    int i = 0;
    int achou = 0;
    while(!achou && (i < qtd_towers))
    {
        if((x == player_towers[i]->x)&&(y == player_towers[i]->y))
            achou = 1;
        else
            i++;
    }

    return player_towers[i];
}

Tower* increase_Stats(Tower* chosen_tower)
{
    chosen_tower->rank += 1;
    chosen_tower->cost += (chosen_tower->cost * 1.5);
    switch(chosen_tower->type)
    {
        case NORMAL_TOWER:
            chosen_tower->damage += 20;
            chosen_tower->range += 0.3;
            break;
        case SPLASH_TOWER:
            chosen_tower->damage += 35;
            chosen_tower->area_damage_mult += 0.15;
            chosen_tower->range += 0.25;
            break;
        case POISON_TOWER:
            chosen_tower->damage += 1;
            chosen_tower->range += 0.2;
            break;
        case SLOW_TOWER:
            chosen_tower->range += 0.25;
            break;
        case BEACON_TOWER:
            chosen_tower->normal_bless += 0.05;
            chosen_tower->splash_bless += 0.5;
            chosen_tower->poison_bless += 1;
            chosen_tower->slow_bless += 5;
            chosen_tower->range += 0.5;
            break;
    }
    return chosen_tower;
}

int locate_Tower(Tower* tower, Tower* player_towers[], int qtd_towers)
{
    int i = 0;
    int achou = 0;
    while(!achou && (i < qtd_towers))
    {
        if((tower->x == player_towers[i]->x)&&(tower->y == player_towers[i]->y))
            achou = 1;
        else
            i++;
    }
    return i;
}

Tower** sell_Tower(int x, int y, Tower* player_towers[], int* qtd_towers)
{
    *qtd_towers -= 1;
    Tower* chosen_tower = search_Tower(x,y,player_towers,*qtd_towers);
    Tower* aux = NULL;
    int i = locate_Tower(chosen_tower,player_towers,*qtd_towers);

    if(i != *qtd_towers){
        aux = chosen_tower;
        player_towers[i] = player_towers[*qtd_towers];
        player_towers[*qtd_towers] = aux;
    }

    player_towers = (Tower**) realloc(player_towers, *qtd_towers * sizeof(Tower*));

    return player_towers;
}

Tower** new_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y)
{
    int i;
    Tower* beacon = search_Tower(x,y,player_towers,qtd_towers);
    double distance;
    for(i = 0; i < qtd_towers; i++)
    {
        if(player_towers[i]->type != BEACON_TOWER){
            distance = pow(x - player_towers[i]->x,2);
            distance += pow(y - player_towers[i]->y,2);
            distance = sqrt(distance);

            if(distance < (beacon->range * 36)){
                switch(player_towers[i]->type)
                {
                    case NORMAL_TOWER:
                        player_towers[i]->damage *= beacon->normal_bless;
                        break;
                    case SPLASH_TOWER:
                        player_towers[i]->radius += beacon->splash_bless;
                        break;
                    case POISON_TOWER:
                        player_towers[i]->damage += beacon->poison_bless;
                        break;
                    case SLOW_TOWER:
                        player_towers[i]->slow_tax += beacon->slow_bless;
                        break;
                }
            }
        }
    }

    return player_towers;
}

Tower** near_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y)
{
    int i;
    Tower* new_tower = search_Tower(x,y,player_towers,qtd_towers);
    double distance;

    for(i = 0; i < qtd_towers; i++)
    {
        if(player_towers[i]->type == BEACON_TOWER){
            distance = pow(x - player_towers[i]->x,2);
            distance += pow(y - player_towers[i]->y,2);
            distance = sqrt(distance);
            if(distance < (player_towers[i]->range * 36)){
                switch(new_tower->type)
                {
                    case NORMAL_TOWER:
                        new_tower->damage *= player_towers[i]->normal_bless;
                        break;
                    case SPLASH_TOWER:
                       new_tower->radius += player_towers[i]->splash_bless;
                        break;
                    case POISON_TOWER:
                        new_tower->damage += player_towers[i]->poison_bless;
                        break;
                    case SLOW_TOWER:
                        new_tower->slow_tax += player_towers[i]->slow_bless;
                        break;
                }
            }
        }
    }

    return player_towers;
}

Tower** sold_beacon_influence(Tower* player_towers[], int qtd_towers, int x, int y)
{
    int i;
    Tower* beacon = search_Tower(x,y,player_towers,qtd_towers);
    double distance;

    for(i = 0; i < qtd_towers; i++)
    {
        if(player_towers[i]->type != BEACON_TOWER){
            distance = pow(x - player_towers[i]->x,2);
            distance += pow(y - player_towers[i]->y,2);
            distance = sqrt(distance);
            if(distance < (beacon->range * 36)){
                switch(player_towers[i]->type)
                {
                    case NORMAL_TOWER:
                        player_towers[i]->damage /= beacon->normal_bless;
                        break;
                    case SPLASH_TOWER:
                        player_towers[i]->radius -= beacon->splash_bless;
                        break;
                    case POISON_TOWER:
                        player_towers[i]->damage -= beacon->slow_bless;
                        break;
                    case SLOW_TOWER:
                        player_towers[i]->slow_tax -= beacon->poison_bless;
                        break;
                }
            }
        }
    }

    return player_towers;
}

Tower** priority_Change(Tower* player_towers[], int qtd_towers, int priority, int chosen_tower_type)
{
    int i;
    for(i = 0; i < qtd_towers; i++)
    {
        if((chosen_tower_type == NORMAL_TOWER) || (chosen_tower_type == SPLASH_TOWER) ||(chosen_tower_type == SLOW_TOWER))
            if(player_towers[i]->type == chosen_tower_type)
                player_towers[i]->priority = priority;
    }
    return player_towers;
}

double distance(Tower* tower, Enemy* enemy)
{
    double distance;

    distance = pow(tower->x+18 - enemy->x,2);
    distance += pow(tower->y+18 - enemy->y,2);
    distance = sqrt(distance);

    return distance;
}

int check_Target(Tower* tower, Enemy* enemy)
{
    int enemy_near = 0;

    if(enemy != NULL && enemy->status != LOST && distance(tower, enemy) < (tower->range * 36))  //36 é tile size
        enemy_near = 1;

//    if((tower->type == SLOW_TOWER) && tower->target->slowed)
//    {
//        tower->target->slowed = 0;
//        tower->target->speed *= (1/(1-tower->slow_tax));
//    }

    return enemy_near;
}

int select_Target(Tower* tower,Enemy* enemy_list)
{
    int enemy_detected = 0;
    Enemy* aux = enemy_list;

    while(aux)
    {
        if(aux->status != LOST && distance(tower, aux) < (tower->range * 36)){
            enemy_detected = 1;

            if(!tower->target)
                tower->target = aux;
            else if((tower->priority == FAR)&&(aux->pace_counter > tower->target->pace_counter))
                tower->target = aux;
            else if((tower->priority == CLOSE)&&(aux->pace_counter < tower->target->pace_counter))
                tower->target = aux;
            else if((tower->priority == MORE_HP)&&(aux->hp > tower->target->hp))
                tower->target = aux;
            else if((tower->priority == LESS_HP)&&(aux->hp < tower->target->hp))
                tower->target = aux;
        }
        aux = aux->prox;
    }

    return enemy_detected;
}

void poison_Effect(Tower* tower, Enemy* enemy_list,BITMAP* buffer,SAMPLE* dead2,Stats* stats)
{
    int i;
    Enemy* aux = enemy_list;
    set_trans_blender(255,255,255,100);
    BITMAP* poison_e = load_bitmap("sprites/poison_e.bmp",NULL);
    while(aux)
    {
        if(distance(tower, aux) <= (tower->range * 36)){
            if((aux->hp > 0)&&(aux->type != IMUNE)){
                aux->hp -= 0.33 * tower->damage;
                draw_trans_sprite(buffer,poison_e,aux->x-20,aux->y-20);
                //drawing_mode(DRAW_MODE_TRANS, 0,0,0);
                //circlefill(buffer,aux->x,aux->y, 10, makecol(255,128,255));
            }
            else if(!aux->contado){
                aux->contado = 1;
                aux->status = LOST;
                play_sample(dead2,128,0,1000,0);
                switch(aux->type)
                {
                    case NORMAL: stats->normal_killed +=1; break;
                    case RESISTANT: stats->resistant_killed += 1; break;
                    //case IMUNE: stats->imune_killed += 1; break;
                    case STRONG: stats->strong_killed += 1; break;
                    case FAST: stats->fast_killed += 1; break;
                }
            }
        }
        aux = aux->prox;
    }
}

void reset(Tower* tower, BITMAP* buffer, Enemy* enemy_list)
{
    int rank = tower->rank;
    int n = 0;
    double distance;
    Enemy* aux2 = enemy_list;
    while(aux2 != NULL)
    {

        if(check_Target(tower, aux2) == 1 && (aux2->slowed == 1))
        {
        printf("lvl %d %d\n", aux2->lvl, aux2->slowed);
            line(buffer,tower->x+18,tower->y+18,aux2->x,aux2->y,makecol(0,0,0));
            n++;
        }
        if(check_Target(tower, aux2) == 0 && (aux2->slowed == 1))
        {
            printf("rasdf");
            rank++;
            aux2->slowed = 0;
            aux2->speed *= (1/(1-tower->slow_tax));
        }

        aux2=aux2->prox;
    }
    printf("\n\n");
}

void establish_Connection(Tower* tower, BITMAP* buffer, Enemy* enemy_list)
{
    int rank = tower->rank;
    int n = 0;
    double distance;
    Enemy* aux2 = enemy_list;

//    for(rank = 3; rank > 0; rank--)
//    {
//
//    }
    while(aux2 != NULL && rank > 0)
    {

        if(check_Target(tower, aux2) == 1 && aux2->slowed == 1)
        {
            rank--;
        }
        if(check_Target(tower, aux2) == 1 && aux2->slowed == 0)
        {
            aux2->speed *= (1 - tower->slow_tax);
            aux2->slowed = 1;
            rank--;
            printf("2 lvl %d %d %d\n", aux2->lvl, aux2->slowed, rank);
        }
//        if(check_Target(tower, aux2) == 1 && (aux2->slowed == 1))
//        {
//            aux2->slowed = 0;
//            aux2->speed *= (1/(1-tower->slow_tax));
//        }

        aux2=aux2->prox;
    }
    printf("\n\n");
}



double distance_splash(Shot* shot, Enemy* enemy)
{
    double distance;

    distance = pow(shot->target_x - enemy->x,2);
    distance += pow(shot->target_y - enemy->y,2);
    distance = sqrt(distance);

    return distance;
}

void splash_effect(Tower* tower, Enemy* enemy_list, Shot* shot, BITMAP* buffer)
{
    Enemy* aux = enemy_list;
    BITMAP* splash_e = load_bitmap("sprites/splash_e.bmp",NULL);

    while(aux!=NULL)
    {
        if(distance_splash(shot, aux) < 36)
        {
            draw_sprite(buffer,splash_e,shot->target_x-splash_e->w/2,shot->target_y-splash_e->h/2);
            aux->hp -= tower->area_damage_mult*tower->damage;
        }
        aux=aux->prox;
    }
}
