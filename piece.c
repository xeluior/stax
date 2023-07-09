#include "piece.h"
#include "SDL_ext.h"

bool move_piece(piece_t* piece, int mag, SDL_Rect* playfield) {
    Uint32 new_left = leftmost(piece->pips, PIECE_CELLS) + mag;
    Uint32 new_right = rightmost(piece->pips, PIECE_CELLS) + mag;
    if (new_left >= left(playfield)
            && new_right <= right(playfield)
        )
    {
        for (int i = 0; i < PIECE_CELLS; i++) {
            piece->pips[i].x += mag;
        }
    }
    return leftmost(piece->pips, PIECE_CELLS) == new_left;
}

bool piece_intersect(piece_t* piece_a, piece_t* piece_b) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        for (int j = 0; j < PIECE_CELLS; j++) {
            if (SDL_HasIntersection(&piece_a->pips[i], &piece_b->pips[j])) {
                return true;
            }
        }
    }
    return false;
}

