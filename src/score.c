#include "score.h"
#include "SDL_clipboard.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

void update_score(score* self, int mag) {
    // check for underflow
    if (
        self->score < (unsigned int) -INT_MIN
        && mag < 0 
        && (unsigned int) -mag > self->score
       ) {
        self->score = 0;
    }
    // check for overflow
    else if (
            self->score > (UINT_MAX - (unsigned int) INT_MAX)
            && mag > 0
            && (unsigned int) mag > (UINT_MAX - self->score)
        ) {
        self->score = UINT_MAX;
    }
    else {
        self->score += mag;
    }

    // update variables
    sprintf(self->text, SCORE_FMT, self->score);
    SDL_FreeSurface(self->surface);
    self->surface = TTF_RenderUTF8_Solid(self->font, self->text, self->color);
}

void draw_score(score* self, SDL_Surface* dst) {
    SDL_BlitSurface(
            self->surface,
            NULL,
            dst,
            &self->rect
    );
}

score* init_score(void) {
    if (!TTF_WasInit()) TTF_Init();
    score* self = (score*) malloc(sizeof(score));

    self->score = 0;
    sprintf(self->text, SCORE_FMT, self->score);

    self->font  = TTF_OpenFont("resources/monogram.ttf", FONT_SIZE);

    self->color.r = 255;
    self->color.g = 255;
    self->color.b = 255;
    self->color.a = 255;

    self->surface = TTF_RenderUTF8_Solid(self->font, self->text, self->color);

    SDL_GetClipRect(self->surface, &self->rect);

    return self;
}

void free_score(score* self) {
    TTF_CloseFont(self->font);
    SDL_FreeSurface(self->surface);
    free(self);
    if (TTF_WasInit()) TTF_Quit();
}
