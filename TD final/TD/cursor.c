#include "cursor.h"

int check_Cursor(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    if( (x1 >= x2 + w2) || (y1 >= y2 + h2) || (x2 > x1 + w1) || (y2 > y1 + h1) )

        return FALSE;

    else

        return TRUE;
}
