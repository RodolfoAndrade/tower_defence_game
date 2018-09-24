#include "global.h"
#include "initialization.h"
#include "openingscreen.h"
#include "titlescreen.h"
#include "mainmenu.h"
#include "gamescreen.h"
#include "ending_win.h"
#include "ending_lose.h"
#include "credits.h"

int main()
{
    int screen_state;
    init(&screen_state);
    srand(time(NULL));

    while(!exit_program)
    {
        if(screen_state == OPENINGSCREEN)
            openingscreen(&screen_state);

        else if(screen_state == TITLESCREEN)
            titlescreen(&screen_state);

        else if(screen_state == MAINMENU)
            mainmenu(&screen_state);

        else if(screen_state == GAMESCREEN)
            gamescreen(&screen_state);

        else if(screen_state == ENDING_WIN)
            ending_win(&screen_state);

        else if(screen_state == ENDING_LOSE)
            ending_lose(&screen_state);

        else if(screen_state == CREDITS)
            credits(&screen_state);


    }

    return 0;
}
END_OF_MAIN();
