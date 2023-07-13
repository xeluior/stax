#include <time.h>

#include "stax.h"
#include "SDL_ext.h"
#include "board.h"
#include "piece.h"

// creates a flat j piece in the middle-top of the board
piece_t* create_piece(game_board* board, SDL_PixelFormat* format);

// tries to move the piece by magnitude, returning true on sucess or false if
// the piece would move out of the playing area or into another piece
bool checked_move(piece_t* self, SDL_Point mag, game_board* playfield);
bool checked_rotation(piece_t* piece, game_board* playfield);
void play_piece(game_board* board, piece_t* piece);

int gravity = 16;
int falling = true;
int g_counter = 0;

SDL_Point v_up = { 0, -CELL_H };
SDL_Point v_down = { 0, CELL_H };
SDL_Point v_left = { -CELL_W, 0 };
SDL_Point v_right  = { CELL_W, 0 };

int main() {
    // Begin SDL Boilerplate
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
            WIN_TITLE,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE
    );
    if (window == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Surface* main_surface = SDL_GetWindowSurface(window);
    if (main_surface == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    // End SDL Boilerplate

    // Begin game initialization
    SDL_Rect window_clip;
    SDL_GetClipRect(main_surface, &window_clip);

    int das_delay = 0;
    game_board* board = board_init();
    center(&window_clip, board->rect);
    update_outline(board);

    Uint32 black = SDL_MapRGB(main_surface->format, 0, 0, 0);
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    srand(time(NULL));

    piece_t* current_piece = create_piece(board, main_surface->format);
    piece_t* next_piece = create_piece(board, main_surface->format);
    play_piece(board, current_piece);
    // End game initialization

    // start game
    SDL_ShowWindow(window);
    int exit = false;
    while(!exit) {
        unsigned long start_of_frame = SDL_GetTicks64();
        SDL_FillRect(main_surface, &window_clip, black);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit = true;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            falling = !falling;
                            break;
                        case SDLK_UP:
                            checked_rotation(current_piece, board);
                            break;
                    }
            }
        }
        if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_C]) {
            exit = true;
        }
        
        // move the piece left || right
        if (keyboard_state[SDL_SCANCODE_LEFT]) {
            if (das_delay == 0) {
                checked_move(current_piece, v_left, board);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            if (das_delay == 0) {
                checked_move(current_piece, v_right, board);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else {
            das_delay = 0;
        }

        // drop the piece
        if (falling) {
            g_counter += 1;
            g_counter %= gravity * 2;
        }

        if (g_counter == 0
                || (g_counter == gravity && keyboard_state[SDL_SCANCODE_DOWN])
            )
        {
            if (!checked_move(current_piece, v_down, board)) {
                if (!add_piece(board, current_piece)) {
                    exit = true;
                }
                else {
                    current_piece = next_piece;
                    next_piece = create_piece(board, main_surface->format);
                    play_piece(board, current_piece);
                    for (int row = 0; row < FIELD_H_CELLS; row++) {
                        clear_row(board, row);
                    }
                }
            }
        }

        // draw the screen
        draw_board(board, main_surface);
        draw_piece(current_piece, main_surface);
        draw_piece(next_piece, main_surface);
        SDL_UpdateWindowSurface(window);

        // sync the framerate
        unsigned long time_since_start = SDL_GetTicks64() - start_of_frame;
        if (time_since_start > FRAME_MS) {
            time_since_start = FRAME_MS;
        }
        SDL_Delay(FRAME_MS - time_since_start);
    }

    // exit
    if (current_piece != NULL) free(current_piece);
    if (next_piece != NULL) free(next_piece);
    board_free(board);
    SDL_FreeSurface(main_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

piece_t* create_piece(game_board* board, SDL_PixelFormat* format) {
    piece_t* piece = init_piece(format, rand() % 7);
    SDL_Point v = {
        right(board->rect) + CELL_W,
        board->rect->h / 2 + top(board->rect)
    };
    move_piece(piece, &v);
    return piece;
}

void play_piece(game_board* board, piece_t* piece) {
    SDL_Point v = {
        board->rect->w / 2 - piece->bound.x - CELL_W * 2 + board->rect->x,
        top(board->rect) - piece->bound.y
    };
    move_piece(piece, &v);
}

bool checked_rotation(piece_t* piece, game_board* playfield) {
    // try the rotation
    rotate_piece(piece, CLOCKWISE);

    // undo if nessecary
    if (!valid_position(playfield, piece)) {
        rotate_piece(piece, COUNTERCLOCKWISE);
        return false;
    }
    return true;
}

bool checked_move(piece_t* piece, SDL_Point mag, game_board* playfield) {
    // try move piece
    move_piece(piece, &mag);

    // undo the move if nessecary
    SDL_Point v_inverse = invert_point(mag);
    if (!valid_position(playfield, piece)) {
        move_piece(piece, &v_inverse);
        return false;
    }
    return true;
}

