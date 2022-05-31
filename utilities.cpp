#include "utilities.h"

void swap(Piece& a, Piece& b)
{
    Piece temp = a;
    a = b;
    b = temp;
}