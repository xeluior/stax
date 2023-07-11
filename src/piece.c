#include "piece.h"
#include "SDL_ext.h"

void init_i(piece_t*);
void init_j(piece_t*);
void init_l(piece_t*);
void init_s(piece_t*);
void init_z(piece_t*);
void init_t(piece_t*);
void init_o(piece_t*);

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

piece_t* init_piece(SDL_PixelFormat* format, char type) {
    piece_t* self = (piece_t*) malloc(sizeof(piece_t));
    self->pips[0].x = RECT_UNDEF;
    self->pips[0].y = RECT_UNDEF;
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->pips[i].x = CELL_W;
        self->pips[i].y = CELL_H;
    }
    switch (type) {
        case 'i':
            init_i(self);
            break;
        case 'j':
            init_j(self);
            self->color = SDL_MapRGB(format, 0, 0, 255);
            break;
    }
    return self;
}

void init_i(piece_t* piece) {
    for (int i = 1; i < PIECE_CELLS; i++) {
        piece->pips[i].x = piece->pips[i-1].x + CELL_W;
        piece->pips[i].y = piece->pips[i-1].y;
    }
}

void init_j(piece_t* piece) {
    piece->pips[1].x = piece->pips[0].x;
    piece->pips[1].y = piece->pips[0].y + CELL_H;
    for (int i = 2; i < PIECE_CELLS; i++) {
        piece->pips[i].x = piece->pips[i-1].x + CELL_W;
        piece->pips[i].y = piece->pips[i-1].y;
    }
}
