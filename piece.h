#ifndef _PIECE_H
#define _PIECE_H

#include <SDL2/SDL_rect.h>
#include "stax.h"

typedef struct piece {
    SDL_Rect pips[PIECE_CELLS];
    int color;
} piece_t;

// attempts to move the piece mag on the x axis binding it within the
// playfield. O(n) n = PIECE_CELLS
bool move_piece(piece_t* piece, int mag, SDL_Rect* playfield);

// checks if piece_a has any pips that insersect with any pips of
// piece_b. O(n^2) n = PIECE_CELLS
bool piece_intersect(piece_t* piece_a, piece_t* piece_b);

#endif

