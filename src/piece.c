#include "piece.h"
#include "SDL_ext.h"
#include "stax.h"

SDL_Point piece_rotations[7][4][4] = { // refereced by piece_rotation[PIECE_TYPE_ENUM_VAL][ROTATION_STATE][PIP]
    { // I_PIECE
        {
            // flat
            { 0, 2 },
            { 1, 2 },
            { 2, 2 },
            { 3, 2 }
        },
        {
            // vertical
            { 2, 0 },
            { 2, 1 },
            { 2, 2 },
            { 2, 3 }
        },
        {
            // flat
            { 0, 2 },
            { 1, 2 },
            { 2, 2 },
            { 3, 2 }
        },
        {
            // vertical
            { 2, 0 },
            { 2, 1 },
            { 2, 2 },
            { 2, 3 }
        }
    }, // end I_PIECE
    {  // O_PIECE
        {
            // only rotation
            { 0, 0 },
            { 0, 1 },
            { 1, 0 },
            { 1, 1 }
        },
        {
            // only rotation
            { 0, 0 },
            { 0, 1 },
            { 1, 0 },
            { 1, 1 }
        },
        {
            // only rotation
            { 0, 0 },
            { 0, 1 },
            { 1, 0 },
            { 1, 1 }
        },
        {
            // only rotation
            { 0, 0 },
            { 0, 1 },
            { 1, 0 },
            { 1, 1 }
        }
    }, // end O_PIECE
    {  // S_PIECE
        {
            // flat
            { 0, 2 },
            { 1, 2 },
            { 1, 1 },
            { 2, 1 }   
        },
        {
            // vertical
            { 1, 0 },
            { 1, 1 },
            { 2, 1 },
            { 2, 2 }
        },
        {
            // flat
            { 0, 2 },
            { 1, 2 },
            { 1, 1 },
            { 2, 1 }   
        },
        {
            // vertical
            { 1, 0 },
            { 1, 1 },
            { 2, 1 },
            { 2, 2 }
        }
    }, // end S_PIECE
    {  // Z_PIECE
        {
            // flat
            { 0, 1 },
            { 1, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        {
            // vertical
            { 2, 0 },
            { 2, 1 },
            { 1, 1 },
            { 1, 2 }
        },
        {
            // flat
            { 0, 1 },
            { 1, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        {
            // vertical
            { 2, 0 },
            { 2, 1 },
            { 1, 1 },
            { 1, 2 }
        }
    }, // end Z_PIECE
    {  // J_PIECE
        {
            // down
            { 0, 1 },
            { 1, 1 },
            { 2, 1 },
            { 2, 2 }
        },
        {
            // left
            { 1, 0 },
            { 1, 1 },
            { 1, 2 },
            { 0, 2 }
        },
        {
            // up
            { 2, 1 },
            { 1, 1 },
            { 0, 1 },
            { 0, 0 }
        },
        {
            // right
            { 1, 2 },
            { 1, 1 },
            { 1, 0 },
            { 2, 0 }
        }
    }, // end J_PIECE
    {  // L_PIECE
        {
            // down
            { 0, 2 },
            { 0, 1 },
            { 1, 1 },
            { 2, 1 }
        },
        {
            // left
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 1, 2 }
        },
        {
            // up
            { 2, 0 },
            { 0, 1 },
            { 1, 1 },
            { 2, 1 }
        },
        {
            // right
            { 2, 2 },
            { 1, 2 },
            { 1, 1 },
            { 1, 0 }
        }
    }, // end L_PIECE
    {  // T_PIECE
        {
            // down
            { 0, 1 },
            { 1, 1 },
            { 1, 2 },
            { 2, 1 }
        },
        {
            // left
            { 1, 0 },
            { 1, 1 },
            { 0, 1 },
            { 1, 2 }
        },
        {
            // up
            { 2, 1 },
            { 1, 1 },
            { 1, 0 },
            { 0, 1 }
        },
        {
            // right
            { 1, 2 },
            { 1, 1 },
            { 2, 1 },
            { 1, 0 }
        }
    } // end T_PIECE
};

void move_piece(piece_t* self, SDL_Point* vector) {
    self->bound.x += vector->x;
    self->bound.y += vector->y;
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->pips[i].x += vector->x;
        self->pips[i].y += vector->y;
    }
}

void rotate_piece(piece_t* self) {
    self->rotation = (self->rotation + 1) % 4;
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->pips[i].x = piece_rotations[self->type][self->rotation][i].x * CELL_W + self->bound.x;
        self->pips[i].y = piece_rotations[self->type][self->rotation][i].y * CELL_H + self->bound.y;
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

piece_t* init_piece(SDL_PixelFormat* format, piece_type type) {
    piece_t* self = (piece_t*) malloc(sizeof(piece_t));
    self->type = type;
    self->rotation = INITIAL_ROTATION;
    self->bound.x = 0;
    self->bound.y = 0;
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->pips[i].w = CELL_W;
        self->pips[i].h = CELL_H;
        self->pips[i].x = piece_rotations[type][INITIAL_ROTATION][i].x * CELL_W;
        self->pips[i].y = piece_rotations[type][INITIAL_ROTATION][i].y * CELL_H;
    }
    switch (type) {
        case I_PIECE:
            self->color = SDL_MapRGB(format, 0, 255, 255); //cyan
            break;
        case J_PIECE:
            self->color = SDL_MapRGB(format, 0, 0, 255); //blue
            break;
        case L_PIECE:
            self->color = SDL_MapRGB(format, 255, 128, 0); //orange
            break;
        case S_PIECE:
            self->color = SDL_MapRGB(format, 0, 255, 0); //green
            break;
        case Z_PIECE:
            self->color = SDL_MapRGB(format, 255, 0, 0); //red
            break;
        case T_PIECE:
            self->color = SDL_MapRGB(format, 128, 0, 128); //purple
            break;
        case O_PIECE:
            self->color = SDL_MapRGB(format, 255, 255, 0); //yellow
    }
    return self;
}

