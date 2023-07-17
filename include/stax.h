#ifndef _STAX_H
#define _STAX_H

#include <SDL2/SDL.h>
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

