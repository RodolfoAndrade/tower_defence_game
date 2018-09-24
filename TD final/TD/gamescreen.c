#include "gamescreen.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "round.h"
//if(enemy_list->status == IN_GAME) //era pra fazer o inimigo desaparecer no fim do caminho, mas n ficou mt legal

void gamescreen(int* screen_state)
{
    int exit_screen = FALSE;
    int i, j;
    int Xo,Yo;
    int victory = FALSE;
    int map[12][16];
    int path_tiles = 0;
    int mouse_timer = 0;
    int spawn_timer = 0;
    int mode = NONE;
    Tower* chosen_tower = NULL;
    int ct_x, ct_y;
    int pause = FALSE;
    int show_help = FALSE;

    int k = 0;

    Enemy* aux;
    Enemy* aux2;

    Dot* start = (Dot*) malloc(sizeof(Dot));
    Dot* finish = (Dot*) malloc(sizeof(Dot));

    /*ALOCANDO STATS*/
    Stats* stats = (Stats*) calloc (1,sizeof(Stats));
    stats->normal_killed = 0;
    stats->imune_killed = 0;
    stats->resistant_killed = 0;
    stats->fast_killed = 0;
    stats->strong_killed = 0;

    /*ALOCANDO O PLAYER*/
    Player* player_aux;
    player_aux = (Player*) malloc (sizeof(Player));
    if(!player_aux)
    {
        printf("\nERRO DE ALOCACAO 1\n");
        exit(1);
    }
    Player* player = player_aux;

    /*ABRINDO O ARQUIVO DE ENTRADA E CRIANDO O DE SAIDA*/
    freopen("input.in", "r", stdin);
    FILE* saida = fopen("LOG.out","w");

    /*ATRIBUINDO TIPOS AOS TILES DO MAPA, CONTANDO A QTD DE TILES DO CAMINHO E SALVANDO O (i,j) DO PONTO DE SPAWN*/
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 16; j++)
        {
            scanf("%d",&map[i][j]);
            if(map[i][j] == ENEMY_SECTION)
                path_tiles += 1;
            if(map[i][j] == START_SECTION)
            {
                start->i = i;
                start->j = j;
            }
            if(map[i][j] == FINISH_SECTION)
            {
                finish->i = i;
                finish->j = j;
            }
        }
    }

    /*MAP TILES*/
    BITMAP* grass = load_bitmap("sprites/grass.bmp",NULL);
    BITMAP* sand = load_bitmap("sprites/sand.bmp",NULL);
    BITMAP* base_saida = load_bitmap("sprites/base_saida.bmp",NULL);
    BITMAP* base_entrada = load_bitmap("sprites/base_entrada.bmp",NULL);

    /*SALVANDO O (x,y) DO PONTO DE SPAWN*/
    double spawn_x = (start->j * sand->w)+20+sand->w/2;
    double spawn_y = (start->i * sand->h)+20+sand->w/2;

    /*ATRIBUINDO VIDA, DINHEIRO E TURNOS*/
    scanf("%d",&player->lifes);
    scanf("%d",&player->cash);
    scanf("%d",&player->rounds);

    /*INICIALIZANDO O VETOR DE TORRES DO JOGADOR COM NULL PARA PODER REALOCAR DEPOIS*/
    player->towers = NULL;
    Tower** aux_towers = NULL;
    int qtd_towers = 0;

    /*ATRIBUINDO OS INIMIGOS*/
    Enemy* enemy_list = NULL;
    int* rest_vector = NULL;
    char type;
    int level, enumType, qtd_enemies = 0;
    char lixo; //tirar lixo

    /*ROUND*/
    Round* round = NULL;
    int round_enemies[player->rounds];
    int first = 1;
    int start_round = 1;
    int end = 0;
    int flag = 0;
    int current_round = 0;

    while(!feof(stdin))
    {
        if(lixo!= '\n')
            scanf("%c", &lixo);
        scanf("%c%d%c",&type,&level,&lixo);
        if(lixo != '\n')
        {
            rest_vector = (int*) realloc (rest_vector,(qtd_enemies+1) * sizeof(int));
            if(!rest_vector)
            {
                printf("\nERRO DE ALOCACAO 1\n");
                exit(1);
            }
            scanf("%d",&rest_vector[qtd_enemies]);
            qtd_enemies++;
        }

        else
        {
            rest_vector = (int*) realloc (rest_vector,(qtd_enemies+1) * sizeof(int));
            if(!rest_vector)
            {
                printf("\nERRO DE ALOCACAO 1\n");
                exit(1);
            }
            rest_vector[qtd_enemies] = 0;
        }

        switch(type)
        {
        case 'N':
            enumType = NORMAL;
            break;
        case 'R':
            enumType = RESISTANT;
            break;
        case 'S':
            enumType = STRONG;
            break;
        case 'F':
            enumType = FAST;
            break;
        case 'I':
            enumType = IMUNE;
            break;
        }
        enemy_list = insert_at_End(enemy_list,enumType,level,spawn_x,spawn_y);

        if(lixo == '\n' || feof(stdin))
        {
            round_enemies[current_round] = qtd_enemies;
            current_round++;
            round = insert_round_at_end(round, rest_vector, enemy_list, current_round);
            rest_vector = NULL;
            enemy_list = NULL;
            qtd_enemies = 0;
        }
    }
    aux2 = round->enemy_list;

    Round* first_round = round;

    int num_frame = 3;
    int frame_atual = 0;
    int tempo_troca = 5;
    int view;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* menu = load_bitmap("sprites/menu.bmp",NULL);
    BITMAP* selection_menu = load_bitmap("sprites/selection_menu.bmp",NULL);
    BITMAP* priority = load_bitmap("sprites/priority.bmp",NULL);
    BITMAP* round_button = load_bitmap("sprites/round_button.bmp",NULL);
    BITMAP* game_menu = load_bitmap("sprites/game_menu.bmp",NULL);
    BITMAP* help_pressed = load_bitmap("sprites/help_pressed.bmp",NULL);
    BITMAP* exit_pressed = load_bitmap("sprites/exit_pressed.bmp",NULL);
    BITMAP* return_pressed = load_bitmap("sprites/return_pressed.bmp",NULL);
    BITMAP* next = load_bitmap("sprites/next.bmp",NULL);
    BITMAP* help1 = load_bitmap("sprites/help1.bmp",NULL);
    BITMAP* help2 = load_bitmap("sprites/help2.bmp",NULL);

    BITMAP* bullet = load_bitmap("sprites/bullet.bmp",NULL);

    /*ICONS*/
    BITMAP* icon_images[5];
    icon_images[0] = load_bitmap("sprites/normal_icon.bmp", NULL);
    icon_images[1] = load_bitmap("sprites/splash_icon.bmp", NULL);
    icon_images[2] = load_bitmap("sprites/poison_icon.bmp", NULL);
    icon_images[3] = load_bitmap("sprites/slow_icon.bmp", NULL);
    icon_images[4] = load_bitmap("sprites/beacon_icon.bmp", NULL);

    /*TOWERS*/
    BITMAP* tower_images[5];
    tower_images[0] = load_bitmap("sprites/normal_tower.bmp", NULL);
    tower_images[1] = load_bitmap("sprites/splash_tower.bmp", NULL);
    tower_images[2] = load_bitmap("sprites/slow_tower.bmp", NULL);
    tower_images[3] = load_bitmap("sprites/poison_tower.bmp", NULL);
    tower_images[4] = load_bitmap("sprites/beacon_tower.bmp", NULL);

    /*ENEMIES*/
    BITMAP* enemy_images[5];
    enemy_images[0] = load_bitmap("sprites/normal_enemy.bmp", NULL);
    enemy_images[1] = load_bitmap("sprites/resistant_enemy.bmp", NULL);
    enemy_images[2] = load_bitmap("sprites/strong_enemy.bmp", NULL);
    enemy_images[3] = load_bitmap("sprites/fast_enemy.bmp", NULL);
    enemy_images[4] = load_bitmap("sprites/imune_enemy.bmp", NULL);

    int frame_w = enemy_images[0]->w / num_frame;
    int frame_h = enemy_images[0]->h / 4;

//    Enemy* enemy_aux = (Enemy*) malloc(sizeof(Enemy));
//    if(!enemy_aux)
//    {
//        printf("\nERRO DE ALOCACAO 2.\n");
//        exit(1);
//    }

    Cursor* cursor_aux = (Cursor*) malloc(sizeof(Cursor));
    if(!cursor_aux)
    {
        printf("\nERRO DE ALOCACAO 1\n");
        exit(1);
    }
    Cursor* cursor = cursor_aux;

    cursor->image = load_bitmap("sprites/cursor_image.bmp",NULL);

    int selected_tower_type = 0;
    BITMAP* selected_tower = NULL;
    int tower_cost = 0;

    ///SOUNDS
    int game_music = 0;
    /*THEMES*/
    MIDI* theme1 = load_midi("sounds/theme1.mid");
    MIDI* theme2 = load_midi("sounds/theme2.mid");
    MIDI* theme3 = load_midi("sounds/theme3.mid");
    MIDI* theme4 = load_midi("sounds/theme4.mid");
    MIDI* theme5 = load_midi("sounds/theme5.mid");
    MIDI* theme6 = load_midi("sounds/theme6.mid");
    MIDI* theme7 = load_midi("sounds/theme7.mid");

    SAMPLE* button = load_sample("sounds/button.wav");
    SAMPLE* tower_sounds[5];
    tower_sounds[2] = load_sample("sounds/poison.wav");
    tower_sounds[4] = load_sample("sounds/beacon.wav");
    SAMPLE* dead = load_sample("sounds/dead.wav");
    SAMPLE* dead2 = load_sample("sounds/dead2.wav");

    int* movement_instructions = (int*) malloc(path_tiles * sizeof(int));
    movement_instructions = set_Trajectory(map,path_tiles,start);

    ///FONTS

    play_midi(theme1,1);

    ticks = 0;

    ///GAME LOOP
    while(!exit_program && !exit_screen && (player->lifes > 0) && !victory)
    {
        /*PARA EVITAR QUE ENTRE NO WHILE VARIAS VEZES EM 1 TICK*/
        int ticker = ticks;
        if(!pause)
            while(ticker == ticks);
        mouse_timer += 1;
        spawn_timer += 1;

        if(mouse_b & 2)
            mode = NONE;

        if(mode != CHOOSE_TOWER)
            chosen_tower = NULL;

        /*DRAW MENU AND MAP*/
        draw_sprite(buffer,menu,0,0);
        load_Map(buffer,grass,sand,tower_images,map);
        load_Map_start_finish(buffer, grass, base_saida, base_entrada, map);

        /*DETERMINE CURSOR POSITION*/
        cursor->x = mouse_x;
        cursor->y = mouse_y;

        /*SELECAO DE TORRE*/
        if(mode == CHOOSE_TOWER)
        {
            circle(buffer, chosen_tower->x+grass->w/2, chosen_tower->y+grass->h/2, chosen_tower->range * grass->w, makecol(172,5,252));
            draw_sprite(buffer,selection_menu,0,0);
            switch(chosen_tower->type)
            {
            case NORMAL_TOWER:
                draw_sprite(buffer,priority,36,480);
                rect(buffer, 36, 480+20*(chosen_tower->priority+1), 118, 500+20*(chosen_tower->priority+1), makecol(172,5,252));
                textout_centre_ex(buffer, font, "NORMAL" , 355 ,488, makecol(200, 190, 30),-1);
                break;
            case SPLASH_TOWER:
                draw_sprite(buffer,priority,36,480);
                rect(buffer, 36, 480+20*(chosen_tower->priority+1), 118, 500+20*(chosen_tower->priority+1), makecol(172,5,252));
                textout_centre_ex(buffer, font, "SPLASH" , 355 ,488, makecol(200, 190, 30),-1);
                break;
            case POISON_TOWER:
                textout_centre_ex(buffer, font, "POISON" , 355 ,488, makecol(200, 190, 30),-1);
                break;
            case SLOW_TOWER:
                draw_sprite(buffer,priority,36,480);
                rect(buffer, 36, 480+20*(chosen_tower->priority+1), 118, 500+20*(chosen_tower->priority+1), makecol(172,5,252));
                textout_centre_ex(buffer, font, "SLOW" , 355 ,488, makecol(200, 190, 30),-1);
                break;
            case BEACON_TOWER:
                textout_centre_ex(buffer, font, "BEACON" , 355 ,488, makecol(200, 190, 30),-1);
                break;
            }

            textprintf_ex(buffer, font,195 ,568, makecol(200, 190, 30),-1, "%d", chosen_tower->rank);
            textprintf_ex(buffer, font,410 ,524, makecol(200, 190, 30),-1, "%.2f", chosen_tower->damage);
            textprintf_ex(buffer, font,410,545, makecol(200, 190, 30),-1, "%.1f", chosen_tower->range);
            textprintf_ex(buffer, font,410, 566, makecol(200, 190, 30),-1, "%d", chosen_tower->rest);
            draw_sprite(buffer,icon_images[chosen_tower->type-4],141,479);

        }

        /*ACTION BUTTON*/
        if((mouse_b & 1)&&(mouse_timer > 10))
        {
            mouse_timer = 0;
            play_sample(button,128,0,1000,0);

            /*CHANGE PRIORITY*/
            if(check_Cursor(mouse_x,mouse_y,0,0,36,500,82,20))
            {
                if(chosen_tower)
                    player->towers = priority_Change(player->towers,qtd_towers,MORE_HP,chosen_tower->type);
                else
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");
            }
            if(check_Cursor(mouse_x,mouse_y,0,0,36,520,82,20))
            {
                if(chosen_tower)
                    player->towers = priority_Change(player->towers,qtd_towers,LESS_HP,chosen_tower->type);
                else
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");
            }
            else if(check_Cursor(mouse_x,mouse_y,0,0,36,540,82,20))
            {

                if(chosen_tower)
                    player->towers = priority_Change(player->towers,qtd_towers,FAR,chosen_tower->type);
                else
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");
            }
            else if(check_Cursor(mouse_x,mouse_y,0,0,36,560,82,20))
            {
                if(chosen_tower)
                    player->towers = priority_Change(player->towers,qtd_towers,CLOSE,chosen_tower->type);
                else
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");
            }

            /*SELL TOWER*/
            else if(check_Cursor(mouse_x,mouse_y,0,0,732,533,802,585))
            {
                if(chosen_tower)
                {
                    player->cash += chosen_tower->cost/2;
                    ct_x = chosen_tower->x;
                    ct_y = chosen_tower->y;
                    if(chosen_tower->type == BEACON_TOWER)
                        player->towers = sold_beacon_influence(player->towers,qtd_towers,ct_x,ct_y);

                    chosen_tower = NULL;
                    draw_sprite(buffer,grass,ct_x,ct_y);
                    map[(ct_y - 20)/sand->w][(ct_x - 20)/sand->h] = TOWER_SECTION;
                    player->towers = sell_Tower(ct_x,ct_y,player->towers,&qtd_towers);

                }
                else
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");

                mode =  NONE;
            }

            /*UPGRADE TOWER*/
            else if(check_Cursor(mouse_x,mouse_y,0,0,732,466,802,517))
            {
                if(chosen_tower && (player->cash >= (chosen_tower->cost * 1.5)))
                {
                    if((chosen_tower->type == BEACON_TOWER) && (chosen_tower->rank >= 3))
                        textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "RANK MAXIMO ATINGIDO");
                    else
                    {
                        player->cash -= (chosen_tower->cost * 1.5);
                        stats->money_spent += (chosen_tower->cost * 1.5);
                        chosen_tower = increase_Stats(chosen_tower);
                        textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "TOWER LEVEL UP");
                    }
                }
                else if(!chosen_tower)
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "SELECIONE UMA TORRE");

                else if(player->cash < (chosen_tower->cost * 1.5))
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "DINHEIRO INSUFICIENTE");

//                    mode = NONE;
//                    chosen_tower = NULL;
            }

            else  /*BUILD TOWER*/
            {
                if(check_Cursor(mouse_x,mouse_y,0,0,622,218,696,296))
                {
                    mode = BUILD;
                    tower_cost = 80;
                    selected_tower = tower_images[0];
                    selected_tower_type = NORMAL_T;
                }
                if(check_Cursor(mouse_x,mouse_y,0,0,704,218,778,296))
                {
                    mode = BUILD;
                    tower_cost = 200;
                    selected_tower = tower_images[1];
                    selected_tower_type = SPLASH_T;
                }
                if(check_Cursor(mouse_x,mouse_y,0,0,704,326,778,404))
                {
                    mode = BUILD;
                    tower_cost = 140;
                    selected_tower = tower_images[2];
                    selected_tower_type = SLOW_T;
                }
                else if(check_Cursor(mouse_x,mouse_y,0,0,622,326,696,404))
                {
                    mode = BUILD;
                    tower_cost = 160;
                    selected_tower = tower_images[3];
                    selected_tower_type = POISON_T;
                }
                if(check_Cursor(mouse_x,mouse_y,0,0,622,434,696,512))
                {
                    mode = BUILD;
                    tower_cost = 400;
                    selected_tower = tower_images[4];
                    selected_tower_type = BEACON_T;
                }
            }

        }

        if(check_Cursor(cursor->x,cursor->y,0,0,678,1,100,20) && (mouse_b & 1))
        {
            remove_int(tick_Counter);
            pause = TRUE;
        }

        /*XO, YO DOS TILES*/
        Xo = 20+(((mouse_x-20) / grass->w) * grass->w);
        Yo = 20+(((mouse_y-20) / grass->h) * grass->h);

        /*HIGHLIGHT DO TILE QUANDO O CURSOR ESTÁ EM CIMA E COLOCAÇÃO DA TORRE*/
        if(check_Cursor(mouse_x,mouse_y,0,0,20,20, grass->w*16, grass->h*12))
        {
            rect(buffer, Xo, Yo, Xo + grass->w, Yo + grass->h, makecol(0, 0, 255));

            if(mouse_b & 1)
            {
                /*SE VC CLICAR NUMA SECAO QUE SEJA MAIOR QUE FINISH_SECTION, SELECIONA UMA TORRE*/
                if((map[Yo/grass->w][Xo/grass->h] > FINISH_SECTION)&& (mode != BUILD))
                {
                    mode = CHOOSE_TOWER;
                    chosen_tower = search_Tower(Xo,Yo,player->towers,qtd_towers);
                }

                /*SE CLICAR NUMA TOWER SECTION E N ESTIVER NO MODO BUILD*/
                if((map[Yo/grass->w][Xo/grass->h] == TOWER_SECTION) && (mode != BUILD))
                    mode = NONE;

                /*SE VC CLICAR NUMA TOWER SECTION, NO MODO BUILD, E TIVER DINHEIRO, COLOCA UMA TORRE*/
                else if((map[Yo/grass->w][Xo/grass->h] == TOWER_SECTION)&&(player->cash >= tower_cost))
                {
                    do
                    {
                        aux_towers = (Tower**) realloc(player->towers,(qtd_towers+1) * sizeof(Tower*));
                        if(!aux_towers)
                            printf("\nERRO DE REALOCACAO DAS TORRES");
                    }
                    while(!aux_towers);
                    player->towers = aux_towers;

                    /*CRIA UMA TORRE*/
                    if((selected_tower_type == BEACON_TOWER)||(selected_tower_type == POISON_TOWER)) //colocar todo tipo
                        play_sample(tower_sounds[selected_tower_type-4],128,0,1000,0);


                    player->towers[qtd_towers] = create_Tower(selected_tower_type,Xo,Yo,round);

                    qtd_towers++;
                    player->cash -= tower_cost;
                    stats->money_spent += tower_cost;

                    map[Yo/grass->w][Xo/grass->h] = selected_tower_type;


                    /*SE FOR BEACON, COLOCA AS BLESS NOS PROX*/
                    if(selected_tower_type == BEACON_TOWER)
                        player->towers = new_beacon_influence(player->towers,qtd_towers,Xo,Yo);
                    else /*SE NAO FOR BEACON, PROCURA SE TEM BEACON POR PERTO. SE TIVER, COLOCA A BLESS NELA*/
                        player->towers = near_beacon_influence(player->towers,qtd_towers,Xo,Yo);

                    /*MODO VIRA NONE*/
                    mode = NONE;

                }
                else if((player->cash < tower_cost)&&(mode == BUILD))  /*SE NAO TIVER DINHEIRO E ESTIVER NO MODO BUILD*/
                {
                    textprintf_centre_ex(buffer,font,300,210,makecol(255,255,129),1, "DINHEIRO INSUFICIENTE");
                    mode = NONE; /*PRINTA MSG DE ERRO NA TELA E MODO VIRA NONE*/
                }

                else if((map[Yo/grass->w][Xo/grass->h] == TOWER_SECTION) && (mode != BUILD))
                    mode = NONE; /*MODO VIRA NONE*/
            }

            if(mode == BUILD)
                draw_trans_sprite(buffer,selected_tower,Xo - selected_tower->w/2 + grass->w/2,Yo- selected_tower->h/2 + 5);
        }

        ///UPDATE

        /*FAZ O MOVIMENTO DO INIMIGO*/
        if(start_round == 1)
        {
            end = 0;
            if(first == 1)
            {
                aux2->status = IN_GAME;
                first = 0;
            }

            if(round->rest_vector[k] != 0 && ticks % round->rest_vector[k] == 0)
            {
                aux2 = aux2->prox;
                aux2->status = IN_GAME;

                k++;
            }
            aux = round->enemy_list;
            while(aux != NULL && flag == 0)
            {
                if(aux->status ==  IN_GAME || aux->status == WON || aux->status == LOST)
                {
                    start_round = 0;
                }
                else
                {
                    start_round = 1;
                    flag = 1;
                }
                aux = aux->prox;

            }

            flag = 0;
        }
        else
        {
            aux = round->enemy_list;
            while(aux != NULL && flag == 0)
            {
                if(aux->status == WON || aux->status == LOST)
                {
                    if(check_Cursor(20+8*36 - round_button->w/2, 20+6*36 - round_button->h/2, round_button->w, round_button->h, mouse_x, mouse_y, 0, 0) && mouse_b & 1)
                    {
                        start_round = 1;
                        game_music = (game_music + 1)%7;
                        switch(game_music)
                        {
                        case 0:
                            play_midi(theme1,1);
                            break;
                        case 1:
                            play_midi(theme2,1);
                            break;
                        case 2:
                            play_midi(theme3,1);
                            break;
                        case 3:
                            play_midi(theme4,1);
                            break;
                        case 4:
                            play_midi(theme5,1);
                            break;
                        case 5:
                            play_midi(theme6,1);
                            break;
                        case 6:
                            play_midi(theme7,1);
                            break;
                        }
                    }
                }
                else
                {
                    start_round = 0;
                    flag = 1;
                }

                aux = aux->prox;

            }
            if(flag == 0)
            {
                end = 1;
            }
            if(aux == NULL && start_round == 1 && round->prox != NULL)
            {
                round = round->prox;
                aux2 = round->enemy_list;

                first = 1;
                k = 0;
            }
            else if(check_EOR(aux2) && !round->prox)
                victory = TRUE;

            flag = 0;
        }

        /*ATUALIZA O PASSO DOS INIMIGOS*/
        //if(!pause){
            aux = round->enemy_list;
            aux = movement_Updater(&pause,aux,path_tiles,&player->lifes,&frame_atual,grass,movement_instructions,&view,tempo_troca,
                                   num_frame,j,buffer,frame_w,frame_h,enemy_images);
        //}

        /*DESENHA AS TORRES*/
        load_Towers(buffer,grass,tower_images,map);

        /*SHOTS*/
        flag =0;
        if(player->towers)
        {
            aux = round->enemy_list;
            //printf(""aux->x
            while(aux != NULL)
            {
                /*SE NAO TIVER ALVO OU SE O ATUAL ESTIVER FORA DA RANGE OU MORTO, TROCA DE ALVO*/
                for(i = 0; i < qtd_towers; i++)
                {
                    if(!player->towers[i]->target)
                        select_Target(player->towers[i],aux);

                    if(player->towers[i]->target)
                    {
                        /*SE TIVER ALVO, CHECA SE O ALVO AINDA ESTA NA RANGE E SE AINDA ESTA VIVO*/
//                        check_Target(player->towers[i], buffer);

                        /*CASO SIM*/
                        if(player->towers[i]->target)
                        {

                            if((player->towers[i]->type == NORMAL_TOWER) || (player->towers[i]->type == SPLASH_TOWER))
                            {
                                circle(buffer, player->towers[i]->target->x, player->towers[i]->target->y, 10, makecol(0,255,0));

                                if(player->towers[i]->shooted == 0)
                                {
                                    player->towers[i]->shot_list = insert_Shot(player->towers[i]->shot_list, player->towers[i]->damage, player->towers[i]->x, player->towers[i]->y,  player->towers[i]->target);
                                    player->towers[i]->shooted = 1;
                                    player->towers[i]->shot_list->ticks = ticks;
                                }
                                else if(player->towers[i]->shot_list->wait == 0 && player->towers[i]->shot_list->ticks < ticks)
                                {
//                                        printf("%d\n", ticks);
                                    if(shot_Follow(player->towers[i]->shot_list,player->towers[i]->target, dead, stats, &player->cash))
                                    {
                                        if(player->towers[i]->type == SPLASH_TOWER)
                                        {
                                            splash_effect(player->towers[i], round->enemy_list, player->towers[i]->shot_list, buffer);
                                        }
                                        player->towers[i]->shot_list->wait = 1;
                                        player->towers[i]->target = NULL;
//                                        printf("%d ", player->towers[i]->shot_list->ticks);
                                    }
                                    player->towers[i]->shot_list->ticks = ticks;
                                }
                                if(player->towers[i]->shot_list->wait == 1 && player->towers[i]->shot_list->ticks + player->towers[i]->rest < ticks)
                                    {
                                        player->towers[i]->shooted = 0;
                                        player->towers[i]->shot_list->ticks = ticks;
                                        player->towers[i]->shot_list->wait = 0;
                                    }
                            }
                            else if(player->towers[i]->type == POISON_TOWER)
                            {
                                poison_Effect(player->towers[i],round->enemy_list,buffer,dead2,stats);
                            }

                            else if(player->towers[i]->type == SLOW_TOWER)
                            {
                                if(flag == 1)
                                {
                                    reset(player->towers[i],buffer, round->enemy_list);
                                    flag = 0;
                                }
                                establish_Connection(player->towers[i],buffer, round->enemy_list);
                                flag = 1;
                            }
                        }
                    }
                }

                aux = aux->prox;
            }
            flag = 0;
        }

        for(i = 0; i < qtd_towers; i++)
        {
            if(player->towers[i]->shot_list != NULL && player->towers[i]->shot_list->wait == 0)
            {
                draw_sprite(buffer,bullet,player->towers[i]->shot_list->origin_x, player->towers[i]->shot_list->origin_y);
                //circlefill(buffer,player->towers[i]->shot_list->target_x, player->towers[i]->shot_list->target_y, 3, makecol(255,0,0));
                //circlefill(buffer,player->towers[i]->shot_list->origin_x, player->towers[i]->shot_list->origin_y, 3, makecol(0,0,255));
            }
        }

        /*ESCREVE OS ATRIBUTOS DO JOGADOR*/
        textprintf_ex(buffer, font,100 ,458, makecol(200, 190, 30),-1, "%d", player->cash);
        textprintf_ex(buffer, font,195 ,458, makecol(200, 190, 30),-1, "%d", player->lifes);
        textprintf_ex(buffer, font,285 ,458, makecol(200, 190, 30),-1, "%d", round->number);
        textprintf_ex(buffer, font,285 ,458, makecol(200, 190, 30),-1, " /%d", player->rounds);

        /*MENU DE PAUSE*/
        if(pause == TRUE)
            draw_sprite(buffer,game_menu,212,161);

        /*INTERAÇÕES COM GAME MENU*/
        if(pause == TRUE){
            if(check_Cursor(cursor->x,cursor->y,0,0,237,198,140,15)){// && (mouse_b & 1)){
                draw_sprite(buffer,help_pressed,237,198);
                if(mouse_b & 1)
                    show_help = 1;
            }
            if(check_Cursor(cursor->x,cursor->y,0,0,237,221,140,15)){
                if(mouse_b & 1)
                    exit_program = TRUE;
                draw_sprite(buffer,exit_pressed,237,221);
            }
            if(check_Cursor(cursor->x,cursor->y,0,0,237,269,140,15)){
                if(mouse_b & 1){
                    pause = FALSE;
                    install_int(tick_Counter,30);
                }
                draw_sprite(buffer,return_pressed,237,269);
            }
        }

        if(show_help == 1){
            draw_sprite(buffer,help1,0,0);
            if(check_Cursor(cursor->x,cursor->y,0,0,622,460,90,37)){
                draw_sprite(buffer,next,622,460);
                if(mouse_b & 1)
                    show_help = 2;
            }
        }
        if(show_help == 2)
        {
            draw_sprite(buffer,help2,0,0);
            if(check_Cursor(cursor->x,cursor->y,0,0,622,460,90,37)){
                draw_sprite(buffer,next,622,460);
                if((mouse_b & 1)&&(mouse_timer > 10))
                    show_help = 0;
            }
        }

        /*DESENHA BOTAO PARA O PROXIMO ROUND*/
        if((end == 1)&&(round->prox))
            draw_sprite(buffer, round_button, 20+8*36 - round_button->w/2, 20+6*36 - round_button->h/2);
        /*DESENHA O CURSOR*/
        draw_sprite(buffer,cursor->image,cursor->x,cursor->y);

//            textprintf_right_ex(buffer,font,SCREEN_W,0,makecol(255,255,255),0,"%d",ticks); //debug
        /*DESENHA O BUFFER NA TELA*/

        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
    }

    /*ESCREVE LOG DE SAIDA E MUDA DE TELA*/
    if(!player->lifes)
        fprintf(saida,"%s\n","O jogador perdeu!");
    if(victory)
        fprintf(saida,"%s\n","O jogador venceu!");

    fprintf(saida,"%d\n",round->number);
    fprintf(saida,"N: %d\nR: %d\nS: %d\nF: %d\nI: %d\n",stats->normal_killed,stats->resistant_killed,stats->strong_killed,stats->fast_killed,stats->imune_killed);
    fprintf(saida,"%d\n",stats->money_spent);
    fprintf(saida,"%d\n",player->cash);
    fprintf(saida,"%d\n",player->lifes);
    Round* aux_round = first_round;
    while(aux_round){ /*NORMAL TOWERS*/
        fprintf(saida,"%d ",aux_round->normal_built);
        aux_round = aux_round->prox;
    }
    fprintf(saida,"%c",'\n');
    aux_round = first_round;
    while(aux_round){   /*SPLASH*/
        fprintf(saida,"%d ",aux_round->splash_built);
        aux_round = aux_round->prox;
    }
    fprintf(saida,"%c",'\n');
    aux_round = first_round;
    while(aux_round){ /*SLOW*/
        fprintf(saida,"%d ",aux_round->slow_built);
        aux_round = aux_round->prox;
    }
    fprintf(saida,"%c",'\n');
    aux_round = first_round;
    while(aux_round){ /*POISON*/
        fprintf(saida,"%d ",aux_round->poison_built);
        aux_round = aux_round->prox;
    }
    fprintf(saida,"%c",'\n');
    aux_round = first_round;
    while(aux_round){ /*BEACON*/
        fprintf(saida,"%d ",aux_round->beacon_built);
        aux_round = aux_round->prox;
    }

    if(!player->lifes){
        exit_screen = TRUE;
        *screen_state = ENDING_LOSE;
    }
     if(victory){
        exit_screen = TRUE;
        *screen_state = ENDING_WIN;
    }

    ///FINALIZACAO
    destroy_midi(theme1);
    destroy_midi(theme2);
    destroy_midi(theme3);
    destroy_midi(theme4);
    destroy_midi(theme5);
    destroy_midi(theme6);
    destroy_midi(theme7);
    destroy_sample(button);
    destroy_sample(dead);
    destroy_sample(dead2);
    destroy_sample(tower_sounds[2]);
    destroy_sample(tower_sounds[4]);
    destroy_bitmap(grass);
    destroy_bitmap(sand);
    destroy_bitmap(menu);
    destroy_bitmap(cursor->image);
    for(i = 0; i < 5; i++){
        destroy_bitmap(tower_images[i]);
        destroy_bitmap(enemy_images[i]);
        destroy_bitmap(icon_images[i]);
    }
    destroy_bitmap(buffer);
    destroy_bitmap(base_saida);
    destroy_bitmap(base_entrada);
    destroy_bitmap(selection_menu);
    destroy_bitmap(priority);
    destroy_bitmap(round_button);
    destroy_bitmap(game_menu);
    destroy_bitmap(help_pressed);
    destroy_bitmap(exit_pressed);
    destroy_bitmap(return_pressed);
    destroy_bitmap(next);
    destroy_bitmap(help1);
    destroy_bitmap(help2);
    destroy_bitmap(bullet);

    free(start);
    free(finish);
    free(cursor);
    free(movement_instructions);

    fclose(saida);
}
