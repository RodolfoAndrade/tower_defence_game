#include "initialization.h"

void init(int* screen_state)
{
    allegro_init();
    install_timer();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_window_title("Warcraft II Tower Defense");

    exit_program = FALSE;
    LOCK_FUNCTION(exit_Program);
    LOCK_VARIABLE(exit_program);
    set_close_button_callback(exit_Program);

    ticks = 0;
    LOCK_FUNCTION(tick_Counter);
    LOCK_VARIABLE(ticks);
    install_int_ex(tick_Counter, MSEC_TO_TIMER(30));

    *screen_state = OPENINGSCREEN; //trocar pra opening
}
