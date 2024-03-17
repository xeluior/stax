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
#ifndef _UI_NUMBER
#define _UI_NUMBER

#include <SDL_ttf.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <limits.h>
#include <string.h>

#define FONT_SIZE 36
#define FMT_LEN CHAR_MAX

typedef struct ui_number {
    int number;
    char text[10];
    char* fmt;
    SDL_Color color;
    SDL_Rect rect;
    SDL_Surface* surface;
} ui_number;

// modifies score by mag, bounded by uint32 limits
ui_number* init_number(char* fmt, int number);
void set_number(ui_number* self, int mag);
void draw_number(ui_number* self, SDL_Surface* dst);
void free_number(ui_number* self);

#endif

