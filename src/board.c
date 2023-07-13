#include "board.h"
#include "SDL_ext.h"

game_board* board_init(void) {
    game_board* self = (game_board*) malloc(sizeof(game_board));
    self->rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    self->rect->w = FIELD_W_PX;
    self->rect->h = FIELD_H_PX;
    self->outline = outline_rect(self->rect);
    for (int row = 0; row < FIELD_H_CELLS; row++) {
        for (int col = 0; col < FIELD_W_CELLS; col++) {
            self->cells[row][col] = NULL;
        }
    }
    return self;
}

void board_free(game_board* self) {
    if (self != NULL) {
        if (self->rect != NULL) free(self->rect);
        for (int i = 0; i < FIELD_H_CELLS; i++) {
            for (int j = 0; j < FIELD_W_CELLS; j++) {
                if (self->cells[i][j] != NULL) free(self->cells[i][j]);
            }
        }
        free(self->outline);
        free(self);
    }
}

void update_outline(game_board* self) {
    free(self->outline);
    self->outline = outline_rect(self->rect);
}

void draw_board(game_board* self, SDL_Surface* surface) {
    SDL_FillRects(surface, self->outline, 4, SDL_MapRGB(surface->format, 255, 255, 255));
    for (int row = 0; row < FIELD_H_CELLS; row++) {
        for (int col = 0; col < FIELD_W_CELLS; col++) {
            cell* c = self->cells[row][col];
            if (c != NULL) {
                SDL_FillRect(surface, &c->rect, c->color);
            }
        }
    }
}

bool add_piece(game_board* self, piece_t* piece) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        int col = (piece->cells[i]->rect.x - self->rect->x) / CELL_W;
        int row = (piece->cells[i]->rect.y - self->rect->y) / CELL_H;

        if (
            col < 0
            || col >= FIELD_W_CELLS 
            || row < 0
            || row >= FIELD_H_CELLS
            || self->cells[row][col] != NULL
        ) {
            return false;
        }
        self->cells[row][col] = piece->cells[i];
    }
    free(piece);
    return true;
}

bool valid_position(game_board* self, piece_t* piece) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        int col = (piece->cells[i]->rect.x - self->rect->x) / CELL_W;
        int row = (piece->cells[i]->rect.y - self->rect->y) / CELL_H;
        if (left(&piece->cells[i]->rect) < left(self->rect)
               || right(&piece->cells[i]->rect) > right(self->rect)
               || bottom(&piece->cells[i]->rect) > bottom(self->rect)
               || self->cells[row][col] != NULL
           ) {
            return false;
        }
    }
    return true;
}

bool row_complete(game_board* self, int row) {
    for (int col = 0; col < FIELD_W_CELLS; col++) {
        if (self->cells[row][col] == NULL) return false;
    }
    return true;
}

bool clear_row(game_board* self, int row) {
    if (!row_complete(self, row)) return false;

    for (int col = 0; col < FIELD_W_CELLS; col++) {
        free(self->cells[row][col]);
    }

    for (int i = row; i > 0; i--) {
        for (int j = 0; j < FIELD_W_CELLS; j++) {
            self->cells[i][j] = self->cells[i-1][j];
            if (self->cells[i][j] != NULL) self->cells[i][j]->rect.y += CELL_H;
        }
    }

    for (int i = 0; i < FIELD_W_CELLS; i++) {
        self->cells[0][i] = NULL;
    }
    return true;
}

