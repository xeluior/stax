#ifndef _BOARD_H
#define _BOARD_H

#include <SDL2/SDL_rect.h>
#include "stax.h"
#include "piece.h"

typedef struct game_board {
    int count;
    piece_t pieces[MAX_PIECES];
    SDL_Rect* rect;
} game_board;

// initializes all the components of the game board to their default values
// based on the constants in stax.h. Returns a pointer that should be freed
// with board_free()
game_board* board_init(void);

// frees all the resources associated with a game_board object
void board_free(game_board*);

#endif

