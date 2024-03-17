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
#ifndef _STAX_H
#define _STAX_H

#include <SDL.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

#define GAME_NAME "Stax"
#define VERSION "v0.1.0"
#define WIN_TITLE GAME_NAME " " VERSION
#define SCREEN_W 640
#define SCREEN_H 480
#define CELL_W 20
#define CELL_H 20
#define RECT_UNDEF 0
#define FIELD_W_CELLS 10
#define FIELD_H_CELLS 20
#define FIELD_W_PX FIELD_W_CELLS * CELL_W
#define FIELD_H_PX FIELD_H_CELLS * CELL_H
#define FPS 60
#define FRAME_MS 1000 / FPS
#define DAS_DELAY 10
#define PIECE_CELLS 4
#define PIECE_TYPES 7
#define PIECE_ROTATIONS 4
#define MAX_PIECES FIELD_W_CELLS * FIELD_H_CELLS / PIECE_CELLS
#define INITIAL_ROTATION 0
#define LINES_PER_LEVEL 10
#define SOFT_DROP_MULT 2

extern char* base_dir;

#endif

