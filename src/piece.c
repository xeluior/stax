/*
 * Stax: a classic falling block puzzle game
 * Copyright (C) 2023 Robert Gingras
 * 
 * Stax is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Stax is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Stax. If not, see <https://www.gnu.org/licenses/>. 
 */
#include "piece.h"
#include "SDL_ext.h"
#include "stax.h"

SDL_Point piece_rotations[PIECE_TYPES][PIECE_ROTATIONS][PIECE_CELLS] = { // refereced by piece_rotation[PIECE_TYPE_ENUM_VAL][ROTATION_STATE][PIP]
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
        self->cells[i]->rect.x += vector->x;
        self->cells[i]->rect.y += vector->y;
    }
}

void rotate_piece(piece_t* self, rotation_dir direction) {
    switch (direction) {
        case CLOCKWISE:
            self->rotation += 1;
            self->rotation %= PIECE_ROTATIONS;
            break;
        case COUNTERCLOCKWISE:
            self->rotation -= 1;
            while (self->rotation < 0) {
                self->rotation += PIECE_ROTATIONS;
            }
            break;
    }
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->cells[i]->rect.x = piece_rotations[self->type][self->rotation][i].x * CELL_W + self->bound.x;
        self->cells[i]->rect.y = piece_rotations[self->type][self->rotation][i].y * CELL_H + self->bound.y;
    }
}

bool piece_intersect(piece_t* piece_a, piece_t* piece_b) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        for (int j = 0; j < PIECE_CELLS; j++) {
            if (SDL_HasIntersection(&piece_a->cells[i]->rect, &piece_b->cells[j]->rect)) {
                return true;
            }
        }
    }
    return false;
}

piece_t* init_piece(SDL_PixelFormat* format, piece_type type) {
    piece_t* self = malloc(sizeof(piece_t));
    self->type = type;
    self->rotation = INITIAL_ROTATION;
    self->bound.x = 0;
    self->bound.y = 0;
    for (int i = 0; i < PIECE_CELLS; i++) {
        self->cells[i] = malloc(sizeof(cell));
        self->cells[i]->rect.w = CELL_W;
        self->cells[i]->rect.h = CELL_H;
        self->cells[i]->rect.x = piece_rotations[type][INITIAL_ROTATION][i].x * CELL_W;
        self->cells[i]->rect.y = piece_rotations[type][INITIAL_ROTATION][i].y * CELL_H;
        switch (type) {
            case I_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 0, 255, 255); //cyan
                break;
            case J_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 0, 0, 255); //blue
                break;
            case L_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 255, 128, 0); //orange
                break;
            case S_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 0, 255, 0); //green
                break;
            case Z_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 255, 0, 0); //red
                break;
            case T_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 255, 0, 255); //purple
                break;
            case O_PIECE:
                self->cells[i]->color = SDL_MapRGB(format, 255, 255, 0); //yellow
        }
    }
    return self;
}

void draw_piece(piece_t* self, SDL_Surface* surface) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        SDL_FillRect(surface, &self->cells[i]->rect, self->cells[i]->color);
    }
}

