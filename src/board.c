#include "board.h"
#include "SDL_ext.h"

game_board* board_init(void) {
    game_board* self = (game_board*) malloc(sizeof(game_board));
    self->count = 0;
    self->rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    self->rect->w = FIELD_W_PX;
    self->rect->h = FIELD_H_PX;
    self->outline = outline_rect(self->rect);
    return self;
}

void board_free(game_board* self) {
    if (self != NULL) {
        if (self->rect != NULL) free(self->rect);
        for (int i = 0; i < self->count; i++) {
            free(self->pieces[i]);
        }
        free(self->outline);
        free(self);
    }
}

void update_outline(game_board* self) {
    free(self->outline);
    self->outline = outline_rect(self->rect);
}
