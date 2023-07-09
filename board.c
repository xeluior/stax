#include "board.h"

game_board* board_init() {
    game_board* ret_ptr = (game_board*) malloc(sizeof(game_board));
    ret_ptr->count = 0;
    ret_ptr->rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    ret_ptr->rect->w = FIELD_W_PX;
    ret_ptr->rect->h = FIELD_H_PX;
    return ret_ptr;
}

void board_free(game_board* ptr) {
    if (ptr != NULL) {
        if (ptr->rect != NULL) free(ptr->rect);
        free(ptr);
    }
}
