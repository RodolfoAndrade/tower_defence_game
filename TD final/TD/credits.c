#include "credits.h"

void credits(int* screen_state)
{
    int exit_screen = FALSE;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* credits = load_bitmap("sprites/credits.bmp",NULL);
    BITMAP* back = load_bitmap("sprites/back.bmp",NULL);

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

    while(!exit_program && !exit_screen)
    {
        draw_sprite(buffer,credits,0,0);
        cursor->x = mouse_x;
        cursor->y = mouse_y;
        if((check_Cursor(cursor->x,cursor->y,0,0,696,542,93,46)) && (mouse_b & 1)){
            draw_sprite(buffer,back,696,542);
            play_sample(button,128,0,1000,0);
            exit_screen = TRUE;
            *screen_state = MAINMENU;
        }

        draw_sprite(buffer,cursor->image,cursor->x,cursor->y);
        draw_sprite(screen,buffer,0,0);
    }
    destroy_bitmap(credits);
    destroy_bitmap(back);
    destroy_bitmap(buffer);
    destroy_bitmap(cursor->image);
    free(cursor);
    destroy_sample(button);
}
