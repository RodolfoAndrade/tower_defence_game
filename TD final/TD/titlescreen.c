#include "titlescreen.h"

void titlescreen(int* screen_state)
{
    int exit_screen = FALSE;
    float angle = - 27.0;
    int reverse = 0;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* titlescreen = load_bitmap("sprites/titlescreen.bmp",NULL);
    BITMAP* start = load_bitmap("sprites/start.bmp",NULL);

    ///SOUNDS
    SAMPLE* button = load_sample("sounds/button.wav");
    MIDI* titlescreen_bs = load_midi("sounds/titlescreen_bs.mid");

    ///GAME LOOP
    play_midi(titlescreen_bs,1);

    while(ticks > 0 && !exit_program && !exit_screen)
    {
        //INPUT
        if(key[KEY_ENTER])
        {
            play_sample(button,128,0,1000,0);
            exit_screen = TRUE;
            *screen_state = MAINMENU;
        }

        //UPDATE
        if(angle == -27)
            reverse = 1;
        else if(angle == 27)
            reverse = -1;
        if(angle <= 27)
            angle += (reverse * 3);

        //DRAW
        draw_sprite(buffer,titlescreen, 0, 0);

        if((angle > 14)||(angle < -14))
            draw_sprite(buffer, start, 303, 320);


        draw_sprite(screen, buffer, 0, 0);
        clear_to_color(buffer, makecol(255,255,255));


        ticks--;
    }


    ///FINALIZACAO
    destroy_bitmap(titlescreen);
    destroy_bitmap(start);
    destroy_bitmap(buffer);
}
