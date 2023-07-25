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
#ifndef _BOARD_H
#define _BOARD_H

#include <SDL2/SDL_rect.h>
#include "stax.h"
#include "piece.h"
#include "SDL_ext.h"

typedef struct game_board {
    cell* cells[FIELD_H_CELLS][FIELD_W_CELLS];
    SDL_Rect* rect;
    SDL_Rect* outline;
} game_board;

// initializes all the components of the game board to their default values
// based on the constants in stax.h. Returns a pointer that should be freed
// with board_free()
game_board* board_init(void);

// frees all the resources associated with a game_board object
void board_free(game_board*);

// regenerates the outline rects to match the main rect position, should be
// called any time rect's position changes. O(c)
void update_outline(game_board* self);

// draws the board outline in white and all cells in the matrix in their own
// color. Expects the cell color uses the same pixel format of the surface
// this will draw onto. O(FIELD_W_CELLS * FIELD_H_CELLS)
void draw_board(game_board* self, SDL_Surface* surface);

// adds the piece's cells to the board matrix and frees the now unneeded piece
// information. returns false if trying to add the piece to an occupied square
// or outside the matrix and does not free the piece. O(PIECE_CELLS)
bool add_piece(game_board* self, piece_t* piece);

// checks that the piece is both in or above the game board, and that it does
// not intersect with any existing cells in the board. O(PIECE_CELLS)
bool valid_position(game_board* self, piece_t* piece);

// clears the given row if it is complete and returns false otherwise. Moves
// all other rows down.
bool clear_row(game_board* self, int row);

#endif

