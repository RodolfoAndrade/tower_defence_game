#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include <allegro.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cursor.h"

volatile int exit_program;
void exit_Program();

volatile int ticks;
void tick_Counter();

enum { OPENINGSCREEN, TITLESCREEN, MAINMENU, GAMESCREEN, ENDING_WIN, ENDING_LOSE, CREDITS };


#endif // GLOBAL_H_INCLUDED
