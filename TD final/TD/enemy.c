#include "enemy.h"

Enemy* insert_at_End(Enemy* enemy_list, int enumType, int level, double spawn_x, double spawn_y)
{
    Enemy* aux = enemy_list;
    Enemy* newEnemy = (Enemy*) malloc(sizeof(Enemy));
    if(enemy_list != NULL)  /*SE JA TIVER ALGUM INIMIGO NA LISTA*/
    {
        newEnemy = put_enemy_Info(level,spawn_x,spawn_y,enumType);
        if(enemy_list->prox ==  NULL)  /*SE ESSE FOR O ULTIMO INIMIGO DA LISTA*/
        {
            enemy_list->prox = newEnemy;
        }
        else    /*SE ESSE NAO É O ULTIMO INIMIGO DA LISTA*/
        {
            while(aux->prox != NULL)  /*PERCORRE A LISTA ATÉ CHEGAR NO ULTIMO*/
            {
                aux = aux->prox;
            }
            aux->prox = newEnemy;
        }
    }
    else /*PRIMEIRO INIMIGO*/
        enemy_list = put_enemy_Info(level,spawn_x,spawn_y,enumType);

    return enemy_list;
}

Enemy* put_enemy_Info(int level,double spawn_x,double spawn_y,int enumType)
{
    int maxSpeed;
    Enemy* newEnemy = (Enemy*) malloc(sizeof(Enemy));
    newEnemy->status = OUT_GAME;
    newEnemy->lvl = level;
    newEnemy->x = spawn_x;
    newEnemy->y = spawn_y;
    newEnemy->pace_counter = 0;
    newEnemy->cont = 0;
    newEnemy->slowed = 0;
    newEnemy->add = 0;
    newEnemy->contado = 0;
    switch(enumType)
    {

    case NORMAL:
        maxSpeed = 2;
        newEnemy->type = NORMAL;
        newEnemy->hp = 100 + (50 * (level-1));
        newEnemy->defense = 0 + (4 * (level-1));
        newEnemy->speed = 1 + (0.2 * (level-1));
        newEnemy->cash = 20 + (5 * (level-1));
        break;
    case RESISTANT:
        maxSpeed = 2;
        newEnemy->type = RESISTANT;
        newEnemy->hp = 120 + (60 * (level-1));
        newEnemy->defense = 4 + (5 * (level-1));
        newEnemy->speed = 1 + (0.2 * (level-1));
        newEnemy->cash = 35 + (7 * (level-1));
        break;
    case STRONG:
        maxSpeed = 1.5;
        newEnemy->type = STRONG;
        newEnemy->hp = 150 + (100 * (level-1));
        newEnemy->defense = 5 + (6 * (level-1));
        newEnemy->speed = 0.8 + (0.15 * (level-1));
        newEnemy->cash = 40 + (10 * (level-1));
        break;
    case FAST:
        maxSpeed = 3;
        newEnemy->type = FAST;
        newEnemy->hp = 80 + (40 * (level-1));
        newEnemy->defense = 2 + (3 * (level-1));
        newEnemy->speed = 1.5 + (0.3 * (level-1));
        newEnemy->cash = 30 + (7 * (level-1));
        break;
    case IMUNE:
        maxSpeed = 2;
        newEnemy->type = IMUNE;
        newEnemy->hp = 90 + (45 * (level-1));
        newEnemy->defense = 2 + (4 * (level-1));
        newEnemy->speed = 1 + (0.2 * (level-1));
        newEnemy->cash = 30 + (7 * (level-1));
        break;
    }
    if(newEnemy->speed > 2)
        newEnemy->speed = 2;
    newEnemy->prox = NULL;
    return newEnemy;
}

Enemy* movement_Updater(int* pause, Enemy* aux,int path_tiles,int* player_lifes,int* frame_atual,BITMAP* grass,int movement_instructions[],int* view,int tempo_troca,int num_frame,int j,BITMAP* buffer,int frame_w,int frame_h,BITMAP* enemy_images[])
{
    int hp = 0;
    while(aux != NULL)
    {
        if(aux->status == IN_GAME)
        {

            if(!*pause) aux->pace_counter += (0.03 * grass->w * aux->speed);

            if(movement_instructions[aux->cont] == RIGHT)
            {
                if(!*pause) aux->x += (0.03 * grass->w  * aux->speed);
                *view = RIGHT;
            }
            else if(movement_instructions[aux->cont] == DOWN)
            {
                if(!*pause) aux->y += (0.03 * grass->w  * aux->speed) ;
                *view = DOWN;
            }
            else if(movement_instructions[aux->cont] == LEFT)
            {
                if(!*pause) aux->x -= (0.03 * grass->w  * aux->speed);
                *view = LEFT;
            }
            else if(movement_instructions[aux->cont] == UP)
            {
                if(!*pause) aux->y -= (0.03 * grass->w  * aux->speed) ;
                *view = UP;
            }

            if(((aux->pace_counter > grass->w)) && (!*pause))
            {
                aux->cont++;
                aux->add += aux->pace_counter - grass->w;

                if(movement_instructions[aux->cont - 1] != movement_instructions[aux->cont])
                {
                    if(movement_instructions[aux->cont - 1] == RIGHT)
                    {
                        aux->x -= aux->add;
                    }
                    else if(movement_instructions[aux->cont - 1] == LEFT)
                    {
                        aux->x += aux->add;
                    }
                    else if(movement_instructions[aux->cont - 1] == UP)
                    {
                        aux->y += aux->add;
                    }
                    else if(movement_instructions[aux->cont - 1] == DOWN)
                    {
                        aux->y -= aux->add;
                    }
                    aux->add = 0;
                }

                if(aux->cont > path_tiles)
                {
                    aux->status = WON;
                    *player_lifes -= 1;
                }

                aux->pace_counter = 0;
            }


            switch(aux->type)
            {
            case NORMAL:
                hp = 100 + (50 * (aux->lvl-1));
                break;
            case RESISTANT:
                hp = 120 + (60 * (aux->lvl-1));
                break;
            case STRONG:
                hp = 150 + (100 * (aux->lvl-1));
                break;
            case FAST:
                hp = 80 + (40 * (aux->lvl-1));
                break;
            case IMUNE:
                hp = 90 + (45 * (aux->lvl-1));
                break;
            }
            /*DESENHA O INIMIGO*/
            *frame_atual = (ticks / tempo_troca) % num_frame;
            rectfill(buffer, aux->x-9, aux->y-18, aux->x+9, aux->y-20,makecol(0,0,0));
            rectfill(buffer, aux->x-9, aux->y-18, aux->x-9+(aux->hp*18)/hp, aux->y-20,makecol(255,0,0));
            masked_blit(enemy_images[aux->type], buffer, *frame_atual * frame_w, *view * frame_h, aux->x - 72/6, aux->y - 128/8, frame_w, frame_h);
        }

        aux = aux->prox;
        j+=1;

    }
    return aux;
}

int check_EOR(Enemy* enemy_list)
{
    int eor = 1;
    if(enemy_list)
    {
        if(!enemy_list->prox){
            if((enemy_list->status == IN_GAME) || (enemy_list->status == OUT_GAME))
                eor = 0;
        }
        else
        {
            while(enemy_list->prox)
            {
                if((enemy_list->status == IN_GAME) || (enemy_list->status == OUT_GAME))
                {
                    eor = 0;
                }
            }
        }
    }
    return eor;
}
