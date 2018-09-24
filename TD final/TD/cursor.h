#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED
#include "global.h"

typedef struct cursor
{
    int x;
    int y;
    BITMAP* image;
} Cursor;

int check_Cursor(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif // CURSOR_H_INCLUDED
