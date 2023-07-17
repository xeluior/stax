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
    ui_number* self = (ui_number*) malloc(sizeof(ui_number));
    size_t fmt_len = strnlen(fmt, FMT_LEN) + 1;
    self->fmt = (char*) malloc(sizeof(char) * fmt_len);
    strncpy(self->fmt, fmt, fmt_len);

    self->number = number;
    sprintf(self->text, self->fmt, self->number);

    if (ui_font == NULL) {
        int path_len = strlen(base_dir) + strlen(FONT_PATH_REL) + 1;
        char* font_path = (char*) malloc(sizeof(char) * path_len);
        memset(font_path, 0, path_len);

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
