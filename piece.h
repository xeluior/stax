#ifndef _PIECE_H
#define _PIECE_H

#include <SDL2/SDL_rect.h>
#include "stax.h"

typedef struct piece {
    SDL_Rect pips[PIECE_CELLS];
    int color;
} piece_t;

// checks if piece_a has any pips that insersect with any pips of
// piece_b. O(n^2) n = PIECE_CELLS
bool piece_intersect(piece_t* piece_a, piece_t* piece_b);

// modifies the postition of each pip of self by the offsets defined in vector
void move_piece(piece_t* self, SDL_Point* vector);

#endif

