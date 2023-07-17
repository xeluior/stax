#include <time.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_filesystem.h"
#include "stax.h"
#include "SDL_ext.h"
#include "board.h"
#include "piece.h"
#include "ui_number.h"

// creates a flat j piece in the middle-top of the board
piece_t* create_piece(game_board* board, SDL_PixelFormat* format);

// tries to move the piece by magnitude, returning true on sucess or false if
// the piece would move out of the playing area or into another piece
bool checked_move(piece_t* self, SDL_Point mag, game_board* playfield);
bool checked_rotation(piece_t* piece, game_board* playfield);
void play_piece(game_board* board, piece_t* piece);

unsigned int gravity_from_level(int level);

const SDL_Point v_up = { 0, -CELL_H };
const SDL_Point v_down = { 0, CELL_H };
const SDL_Point v_left = { -CELL_W, 0 };
const SDL_Point v_right  = { CELL_W, 0 };

char* base_dir = NULL;

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

    if (TTF_Init() < 0) {
        fprintf(stderr, "%s\n", TTF_GetError());
        fprintf(stderr, "Fonts will not be displayed\n");
    }

    base_dir = SDL_GetBasePath();
    // End SDL Boilerplate

    // Begin game initialization
    SDL_Rect window_clip;
    SDL_GetClipRect(main_surface, &window_clip);

    ui_number* score = init_number("%010d", 0);
    ui_number* level = init_number("Lv%02d", 0);
    ui_number* lines = init_number("Lines%03d", 0);

    level->rect.y = score->rect.y + score->rect.h;
    lines->rect.y = level->rect.y + level->rect.h;

    const int line_scores[] = { 0, 40, 100, 300, 1200 };
    int das_delay = 0;
    int falling = true;
    int g_counter = 0;
    int gravity = gravity_from_level(level->number);

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
                            if (falling) checked_rotation(current_piece, board);
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
            g_counter %= gravity;
        }

        if (g_counter == 0
                || (
                    g_counter == gravity / SOFT_DROP_MULT
                    && keyboard_state[SDL_SCANCODE_DOWN]
                   )
           )
        {
            if (!checked_move(current_piece, v_down, board)) {
                int row_max = 0;
                int row_min = INT_MAX;
                for (int i = 0; i < PIECE_CELLS; i++) {
                    int row = (current_piece->cells[i]->rect.y - board->rect->y) / CELL_H;
                    if (row_max < row) row_max = row;
                    if (row_min > row) row_min = row;
                }
                if (!add_piece(board, current_piece)) {
                    exit = true;
                }
                else {
                    int cleared_rows = 0;
                    for (int row = row_min; row <= row_max; row++) {
                        if (clear_row(board, row)) {
                            cleared_rows += 1;
                        }
                    }
                    if (cleared_rows > 0) {
                        set_number(lines, lines->number + cleared_rows);
                        set_number(level, lines->number / LINES_PER_LEVEL);
                        set_number(score, score->number + line_scores[cleared_rows] * (level->number + 1));
                        gravity = gravity_from_level(level->number);
                    }

                    current_piece = next_piece;
                    next_piece = create_piece(board, main_surface->format);
                    play_piece(board, current_piece);
                }
            }
        }

        // draw the screen
        SDL_FillRect(main_surface, &window_clip, black);
        if (falling) {
            draw_board(board, main_surface);
            draw_piece(current_piece, main_surface);
            draw_piece(next_piece, main_surface);
        }
        draw_number(score, main_surface);
        draw_number(level, main_surface);
        draw_number(lines, main_surface);
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
    free_number(score);
    SDL_free(base_dir);
    SDL_FreeSurface(main_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

piece_t* create_piece(game_board* board, SDL_PixelFormat* format) {
    piece_t* piece = init_piece(format, rand() % PIECE_TYPES);
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

unsigned int gravity_from_level(int level) {
    if (level <= 8) {
        return 48 - level * 5;
    }
    else if (level <= 18) {
        return 9 - level / 3;
    }
    else if (level <= 28) {
        return 2;
    }
    else {
        return 1;
    }
}

