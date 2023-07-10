#include "piece.h"
#include "SDL_ext.h"

void move_piece(piece_t* self, SDL_Point* vector) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->pips[i].x += vector->x;
        self->pips[i].y += vector->y;
    }
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

