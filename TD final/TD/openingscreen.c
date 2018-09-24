#include "openingscreen.h"
#include "animations.h"

void openingscreen(int* screen_state)
{
    int exit_screen = FALSE;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* human_crest = load_bitmap("sprites/human_crest.bmp",NULL);
    BITMAP* orc_crest = load_bitmap("sprites/orc_crest.bmp",NULL);
    BITMAP* nightelf_crest = load_bitmap("sprites/nightelf_crest.bmp",NULL);
    BITMAP* scourge_crest = load_bitmap("sprites/scourge_crest.bmp",NULL);
    BITMAP* wallpaper = load_bitmap("sprites/wallpaper.bmp",NULL);

    ///SOUNDS
    MIDI* intro = load_midi("sounds/intro.mid");
    SAMPLE* button = load_sample("sounds/button.wav");

    play_midi(intro,1);

    while(ticks > 0 && !exit_program && !exit_screen)
    {
        rectfill(buffer,0,0,SCREEN_W,SCREEN_H,0);
        if(ticks < 40)
            rectfill(buffer,0,0,SCREEN_W,SCREEN_H,0);
        else if(ticks == 40)
            highcolor_fade_in(human_crest,9,(SCREEN_W-human_crest->w)/2,(SCREEN_H-human_crest->h)/2);
        else if(ticks < 160)
            draw_sprite(buffer,human_crest,(SCREEN_W-human_crest->w)/2,(SCREEN_H-human_crest->h)/2);
        else if(ticks == 160){
            highcolor_fade_out(9);
            highcolor_fade_in(orc_crest,9,(SCREEN_W-orc_crest->w)/2,(SCREEN_H-orc_crest->h)/2);
        }
        else if(ticks < 280)
            draw_sprite(buffer,orc_crest,(SCREEN_W-orc_crest->w)/2,(SCREEN_H-orc_crest->h)/2);
        else if(ticks == 280){
            highcolor_fade_out(9);
            highcolor_fade_in(nightelf_crest,9,(SCREEN_W-nightelf_crest->w)/2,(SCREEN_H-nightelf_crest->h)/2);
        }
        else if(ticks < 400)
            draw_sprite(buffer,nightelf_crest,(SCREEN_W-nightelf_crest->w)/2,(SCREEN_H-nightelf_crest->h)/2);
        else if(ticks == 400){
            highcolor_fade_out(9);
            highcolor_fade_in(scourge_crest,9,(SCREEN_W-scourge_crest->w)/2,(SCREEN_H-scourge_crest->h)/2);
        }
        else if(ticks < 520)
            draw_sprite(buffer,scourge_crest,(SCREEN_W-scourge_crest->w)/2,(SCREEN_H-scourge_crest->h)/2);
        else if(ticks == 520){
            highcolor_fade_out(9);
            highcolor_fade_in(wallpaper,9,(SCREEN_W-wallpaper->w)/2,(SCREEN_H-wallpaper->h)/2);
        }
        else if(ticks < 701)
            draw_sprite(buffer,wallpaper,(SCREEN_W-wallpaper->w)/2,(SCREEN_H-wallpaper->h)/2);
        else if(ticks == 701){
            exit_screen = TRUE;
            *screen_state = TITLESCREEN;
        }

        //INPUT
        if(key[KEY_ESC])
        {
            play_sample(button,128,0,1000,0);
            exit_screen = TRUE;
            *screen_state = TITLESCREEN;
        }

        //DRAW
        draw_sprite(screen, buffer, 0, 0);
        clear_to_color(buffer, makecol(255,255,255));
    }

    ///FINALIZACAO
    destroy_bitmap(wallpaper);
    destroy_bitmap(human_crest);
    destroy_bitmap(orc_crest);
    destroy_bitmap(nightelf_crest);
    destroy_bitmap(scourge_crest);
    destroy_bitmap(buffer);

    destroy_midi(intro);
    destroy_sample(button);
}

