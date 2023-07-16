#include "ui_number.h"
#include "SDL_clipboard.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

void set_number(ui_number* self, int new) {
    self->number = new;
    sprintf(self->text, self->fmt, self->number);
    SDL_FreeSurface(self->surface);
    self->surface = TTF_RenderUTF8_Solid(self->font, self->text, self->color);
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

    self->font  = TTF_OpenFont("resources/monogram.ttf", FONT_SIZE);

    self->color.r = 255;
    self->color.g = 255;
    self->color.b = 255;
    self->color.a = 255;

    self->surface = TTF_RenderUTF8_Solid(self->font, self->text, self->color);

    SDL_GetClipRect(self->surface, &self->rect);

    return self;
}

void free_number(ui_number* self) {
    TTF_CloseFont(self->font);
    SDL_FreeSurface(self->surface);
    free(self->fmt);
    free(self);
    if (TTF_WasInit()) TTF_Quit();
}
