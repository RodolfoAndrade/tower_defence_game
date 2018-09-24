#include "ending_lose.h"

void ending_lose(int* screen_state)
{
    int exit_screen = FALSE;
    int lose_music = rand() % 2;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* endscreen_lose = load_bitmap("sprites/endscreen_lose.bmp",NULL);
    ///SOUNDS
    MIDI* orc_lose = load_midi("sounds/orc_lose.mid");
    MIDI* human_lose = load_midi("sounds/human_lose.mid");


    switch(lose_music){
        case 0: play_midi(orc_lose,1); break;
        case 1: play_midi(human_lose,1); break;
    }

    while(ticks > 0 && !exit_program && !exit_screen)
    {
        draw_sprite(buffer,endscreen_lose,0,0);
        draw_sprite(screen,buffer,0,0);
        if(key[KEY_ESC])
            exit_program = TRUE;
    }

    destroy_bitmap(buffer);
    destroy_bitmap(endscreen_lose);
    destroy_midi(orc_lose);
    destroy_midi(human_lose);

}
