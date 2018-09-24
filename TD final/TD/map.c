#include "map.h"
#include "tower.h"

void load_Map(BITMAP* buffer, BITMAP* grass, BITMAP* sand, BITMAP* tower_images[], int map[][16])
{
    int i, j;
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 16; j++)
        {
            switch(map[i][j])
            {
                case TOWER_SECTION:
                    draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                    break;
                case ENEMY_SECTION:
                    draw_sprite(buffer,sand,20+(j * sand->w), 20+(i * sand->h));
                    break;
                case START_SECTION:
                    break;
                case FINISH_SECTION:
                    break;
            }
        }
    }
}

void load_Map_start_finish(BITMAP* buffer, BITMAP* grass,BITMAP* base_saida,BITMAP* base_entrada, int map[][16])
{
    int i, j;
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 16; j++)
        {
            switch(map[i][j])
            {
                case START_SECTION:
                    draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                    draw_sprite(buffer,base_saida,20+(j * grass->w)-48+ grass->w/2 , 20+(i * grass->h)-40+ grass->h/2);
                    break;
                case FINISH_SECTION:
                    draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                    draw_sprite(buffer,base_entrada,20+(j * grass->w)-48+ grass->w/2 , 20+(i * grass->h)-40+ grass->h/2);
                    break;
            }
        }
    }
}

void load_Towers(BITMAP* buffer, BITMAP* grass, BITMAP* tower_images[],int map[][16])
{
    int i, j;
    set_trans_blender(255,255,255,50);
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 16; j++)
        {
            switch(map[i][j])
            {
                case NORMAL_T:
                        draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                        draw_sprite(buffer,tower_images[0],20+(j * grass->w)- tower_images[0]->w/2 + grass->w/2,20+(i * grass->h)- tower_images[0]->h/2 + 5);
                        break;
                case SPLASH_T:
                        draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                        draw_sprite(buffer,tower_images[1],20+(j * grass->w)- tower_images[0]->w/2 + grass->w/2,20+(i * grass->h)- tower_images[0]->h/2 + 5);
                        break;
                case POISON_T:
                        draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                        draw_sprite(buffer,tower_images[3],20+(j * grass->w)- tower_images[0]->w/2 + grass->w/2,20+(i * grass->h)- tower_images[0]->h/2 + 5);
                        break;
                case SLOW_T:
                        draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                        draw_sprite(buffer,tower_images[2],20+(j * grass->w)- tower_images[0]->w/2 + grass->w/2,20+(i * grass->h)- tower_images[0]->h/2 + 5);
                        break;
                case BEACON_T:
                        draw_sprite(buffer,grass,20+(j * grass->w),20+(i * grass->h));
                        draw_sprite(buffer,tower_images[4],20+(j * grass->w)- tower_images[0]->w/2 + grass->w/2,20+(i * grass->h)- tower_images[0]->h/2 + 5);
                        break;
            }
        }
    }
}

int* set_Trajectory(int map[][16], int path_tiles, Dot* start)
{
    int* movement_instructions = (int*) malloc(path_tiles * sizeof(int));
    Dot* aux = (Dot*) malloc(sizeof(Dot));
    aux = start;
    int counter = 0;
    Dot* prev = (Dot*) malloc(sizeof(Dot));
    prev->i = -1;
    prev->j = -1;
    while(counter <= path_tiles)
    {
        if((aux->i > 0)&&((map[aux->i-1][aux->j] == ENEMY_SECTION)||(map[aux->i-1][aux->j] == FINISH_SECTION))&&(!prev || (aux->i-1 != prev->i))){
            movement_instructions[counter] = UP;
            prev->i = aux->i;
            prev->j = aux->j;
            aux->i -= 1;
        }
        else if((aux->j < 16)&&((map[aux->i][aux->j+1] == ENEMY_SECTION)||(map[aux->i][aux->j+1] == FINISH_SECTION))&&(!prev || (aux->j+1 != prev->j))){
            movement_instructions[counter] = RIGHT;
            prev->i = aux->i;
            prev->j = aux->j;
            aux->j += 1;
        }
        else if((aux->i < 12)&&((map[aux->i+1][aux->j] == ENEMY_SECTION)||(map[aux->i+1][aux->j] == FINISH_SECTION))&&(!prev || (aux->i+1 != prev->i))){
            movement_instructions[counter] = DOWN;
            prev->i = aux->i;
            prev->j = aux->j;
            aux->i += 1;
        }
        else if((aux->j > 0)&&((map[aux->i][aux->j-1] == ENEMY_SECTION)||(map[aux->i][aux->j-1] == FINISH_SECTION))&&(!prev || (aux->j-1 != prev->j))){
            movement_instructions[counter] = LEFT;
            prev->i = aux->i;
            prev->j = aux->j;
            aux->j -= 1;
        }
        counter++;
    }
    return movement_instructions;
}
