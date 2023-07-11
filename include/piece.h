#ifndef _PIECE_H
#define _PIECE_H

#include <SDL2/SDL_rect.h>
#include "stax.h"

typedef enum piece_types {
    I_PIECE,
    O_PIECE,
    S_PIECE,
    Z_PIECE,
    J_PIECE,
    L_PIECE,
    T_PIECE
} piece_type;

typedef struct piece {
    SDL_Rect pips[PIECE_CELLS];
    SDL_Point bound;
    int rotation;
    unsigned int color;
    piece_type type;
} piece_t;

// checks if piece_a has any pips that insersect with any pips of
// piece_b. O(n^2) n = PIECE_CELLS
bool piece_intersect(piece_t* piece_a, piece_t* piece_b);

// rotates the piece to the next step in the rotation matrix, wrapping if nessecary
void rotate_piece(piece_t* self);

// modifies the postition of each pip of self by the offsets defined in vector
void move_piece(piece_t* self, SDL_Point* vector);

// initializes a piece of the given letter type, using the given pixel format
// for its color
piece_t* init_piece(SDL_PixelFormat* format, piece_type type);

#endif

