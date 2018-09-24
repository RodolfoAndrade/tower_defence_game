#include "round.h"

/*cria os rounds*/
Round* insert_round_at_end(Round* round, int* rest_vector, Enemy* enemy_list,int current_round)
{
    Round* aux = round;
    Round* newRound = (Round*) calloc(1,sizeof(Round));

    newRound->rest_vector = rest_vector;
    newRound->enemy_list = enemy_list;
    newRound->prox = NULL;
    newRound->number = current_round;

    if(aux == NULL)
    {
        round = newRound;
    }
    else
    {
        if(aux->prox == NULL)
        {
            aux->prox = newRound;
        }
        else
        {
            while(aux->prox != NULL)
            {
                aux = aux->prox;
            }
            aux->prox = newRound;
        }
    }
    return round;
}
