#include "ending_win.h"

void ending_win(int* screen_state)
{
    int exit_screen = FALSE;
    int win_music = rand() % 4;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* endscreen_win = load_bitmap("sprites/endscreen_win.bmp",NULL);
    ///SOUNDS
    MIDI* orc_win1 = load_midi("sounds/orc_win1.mid");
    MIDI* orc_win2 = load_midi("sounds/orc_win2.mid");
    MIDI* human_win1 = load_midi("sounds/human_win1.mid");
    MIDI* human_win2 = load_midi("sounds/human_win2.mid");

    switch(win_music)
    {
        case 0: play_midi(orc_win1,1); break;
        case 1: play_midi(orc_win2,1); break;
        case 2: play_midi(human_win1,1); break;
        case 3: play_midi(human_win2,1); break;
    }

    while(ticks > 0 && !exit_program && !exit_screen){
        draw_sprite(buffer,endscreen_win,0,0);
        draw_sprite(screen,buffer,0,0);
        if(key[KEY_ESC])
            exit_program = TRUE;
    }

    destroy_bitmap(buffer);
    destroy_bitmap(endscreen_win);
    destroy_midi(human_win1);
    destroy_midi(human_win2);
    destroy_midi(orc_win2);
    destroy_midi(orc_win1);
}
