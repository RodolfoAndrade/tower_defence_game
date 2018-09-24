#include "mainmenu.h"

void mainmenu(int* screen_state)
{
    int exit_screen = FALSE;

    ///BITMAPS
    /*GAME*/
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* mainmenu = load_bitmap("sprites/mainmenu.bmp",NULL);
    BITMAP* pressed1 = load_bitmap("sprites/pressed1.bmp",NULL);
    BITMAP* pressed2 = load_bitmap("sprites/pressed2.bmp",NULL);
    BITMAP* pressed3 = load_bitmap("sprites/pressed3.bmp",NULL);
    BITMAP* pressed4 = load_bitmap("sprites/pressed4.bmp",NULL);
    Cursor* cursor_aux = (Cursor*) malloc(sizeof(Cursor));
    if(!cursor_aux)
    {
        printf("\nERRO DE ALOCACAO 1\n");
        exit(1);
    }
    Cursor* cursor = cursor_aux;
    cursor->image = load_bitmap("sprites/cursor_image.bmp",NULL);

    ///SOUNDS
    SAMPLE* button = load_sample("sounds/button.wav");

    ///GAME LOOP
    //set_trans_blender(255,255,255,100);
    while(!exit_program && !exit_screen)
    {
        cursor->x = mouse_x;
        cursor->y = mouse_y;

        //INPUT
        if(key[KEY_ESC])
            exit_Program();

        draw_sprite(buffer,mainmenu,0,0);

        if(check_Cursor(cursor->x,cursor->y,0,0,312,247,200,25) == TRUE)
        {
            rect(buffer, 312,247,512,272, makecol(255,255,0));
            if(mouse_b & 1)
            {
                draw_sprite(buffer,pressed1,312,247);
                play_sample(button,128,0,1000,0);
                exit_screen = TRUE;
                *screen_state = GAMESCREEN;
            }
        }
        else if(check_Cursor(cursor->x,cursor->y,0,0,312,287,200,25) == TRUE)
        {
            rect(buffer, 312,284,512,309, makecol(255,255,0));
            if(mouse_b & 1)
            {
                draw_sprite(buffer,pressed2,312,284);
                play_sample(button,128,0,1000,0);
                exit_screen = TRUE;
                *screen_state = OPENINGSCREEN;
            }
        }
        else if(check_Cursor(cursor->x,cursor->y,0,0,312,320,200,25) == TRUE)
        {
            rect(buffer, 312,320,512,345, makecol(255,255,0));
            if(mouse_b & 1)
            {
                draw_sprite(buffer,pressed3,312,320);
                play_sample(button,128,0,1000,0);
                exit_screen = TRUE;
                *screen_state = CREDITS;
            }
        }
        else if(check_Cursor(cursor->x,cursor->y,0,0,312,357,200,25) == TRUE)
        {
            rect(buffer, 312,357,512,383, makecol(255,255,0));
            if(mouse_b & 1)
            {
                draw_sprite(buffer,pressed4,312,357);
                play_sample(button,128,0,1000,0);
                exit_program = TRUE;
            }
        }
            //exit_program = TRUE;


        //UPDATE


        //DRAW
        draw_sprite(buffer,cursor->image,cursor->x,cursor->y);
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);

        ticks--;
    }


    ///FINALIZACAO
    destroy_bitmap(buffer);
    destroy_bitmap(pressed1);
    destroy_bitmap(pressed2);
    destroy_bitmap(pressed3);
    destroy_bitmap(pressed4);
    destroy_bitmap(mainmenu);
    destroy_bitmap(cursor->image);
    free(cursor);
    destroy_sample(button);
}
