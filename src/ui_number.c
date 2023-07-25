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
#include "ui_number.h"
#include "SDL_clipboard.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "stax.h"

#define FONT_PATH_REL "/resources/monogram.ttf"

TTF_Font* ui_font = NULL;
unsigned int instances = 0;

void set_number(ui_number* self, int new) {
    self->number = new;
    sprintf(self->text, self->fmt, self->number);
    SDL_FreeSurface(self->surface);
    self->surface = TTF_RenderUTF8_Solid(ui_font, self->text, self->color);
}

void draw_number(ui_number* self, SDL_Surface* dst) {
    SDL_BlitSurface(
            self->surface,
            NULL,
            dst,
            &self->rect
    );
}

ui_number* init_number(char* fmt, int number) {
    if (!TTF_WasInit()) TTF_Init();
    ui_number* self = malloc(sizeof(ui_number));
    size_t fmt_len = strnlen(fmt, FMT_LEN) + 1;
    self->fmt = calloc(fmt_len, sizeof(char));
    strncpy(self->fmt, fmt, fmt_len);

    self->number = number;
    sprintf(self->text, self->fmt, self->number);

    if (ui_font == NULL) {
        int path_len = strlen(base_dir) + strlen(FONT_PATH_REL) + 1;
        char* font_path = calloc(path_len, sizeof(char));

        strcpy(font_path, base_dir);
        strcat(font_path, FONT_PATH_REL);

        printf("loading fonts from %s\n", font_path);

        ui_font = TTF_OpenFont(font_path, FONT_SIZE);
    }

    self->color.r = 255;
    self->color.g = 255;
    self->color.b = 255;
    self->color.a = 255;

    self->surface = TTF_RenderUTF8_Solid(ui_font, self->text, self->color);

    SDL_GetClipRect(self->surface, &self->rect);

    instances += 1;
    return self;
}

void free_number(ui_number* self) {
    SDL_FreeSurface(self->surface);
    free(self->fmt);
    free(self);

    instances -= 1;
    if (TTF_WasInit() && instances == 0) {
        TTF_CloseFont(ui_font);
        ui_font = NULL;
        TTF_Quit();
    }
}
