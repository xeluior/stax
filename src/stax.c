#include "stax.h"
#include "SDL_ext.h"
#include "board.h"

// creates a flat j piece in the middle-top of the board
piece_t* create_j_piece(game_board* board, SDL_PixelFormat* format);

// tries to move the piece by magnitude, returning true on sucess or false if
// the piece would move out of the playing area or into another piece
bool checked_move(piece_t* self, SDL_Point mag, game_board* playfield);

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

    SDL_Window* main_window = SDL_CreateWindow(
            WIN_TITLE,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE
    );
    if (main_window == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    if (main_surface == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        SDL_DestroyWindow(main_window);
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
    SDL_Rect* playfield = outline_rect(board->rect);

    piece_t* current_piece = create_j_piece(board, main_surface->format);
    // End game initialization

    // define colors
    Uint32 white = SDL_MapRGB(main_surface->format, 255, 255, 255);
    Uint32 black = SDL_MapRGB(main_surface->format, 0, 0, 0);

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    // start game
    SDL_ShowWindow(main_window);
    int exit = false;
    while(!exit) {
        Uint64 start_of_frame = SDL_GetTicks64();
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
                board->pieces[board->count++] = *current_piece;
                free(current_piece);
                if (board->count == MAX_PIECES) {
                    exit = true;
                }
                else {
                    current_piece = create_j_piece(board, main_surface->format);
                }
            }
        }

        // draw the screen
        for (int i = 0; i < board->count; i++) {
            SDL_FillRects(
                    main_surface,
                    board->pieces[i].pips,
                    PIECE_CELLS,
                    board->pieces[i].color
            );
        }
        SDL_FillRects(main_surface, current_piece->pips, PIECE_CELLS, current_piece->color);
        SDL_FillRects(main_surface, playfield, 4, white);
        SDL_UpdateWindowSurface(main_window);

        // sync the framerate
        Uint64 time_since_start = SDL_GetTicks64() - start_of_frame;
        if (time_since_start > FRAME_MS) {
            time_since_start = FRAME_MS;
        }
        SDL_Delay(FRAME_MS - time_since_start);
    }

    // exit
    if (current_piece != NULL) free(current_piece);
    board_free(board);
    free(playfield);
    SDL_FreeSurface(main_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}

piece_t* create_j_piece(game_board* board, SDL_PixelFormat* format) {
    piece_t* ret_ptr = (piece_t*) malloc(sizeof(piece_t));
    ret_ptr->color = SDL_MapRGB(format, 0, 0, 255);
    ret_ptr->pips[0].x = FIELD_W_CELLS / 2 * CELL_W + left(board->rect);
    ret_ptr->pips[0].y = top(board->rect);
    ret_ptr->pips[0].w = CELL_W;
    ret_ptr->pips[0].h = CELL_H;

    ret_ptr->pips[1].x = ret_ptr->pips[0].x;
    ret_ptr->pips[1].y = ret_ptr->pips[0].y + CELL_H;
    ret_ptr->pips[1].w = CELL_W;
    ret_ptr->pips[1].h = CELL_H;

    ret_ptr->pips[2].x = ret_ptr->pips[1].x + CELL_W;
    ret_ptr->pips[2].y = ret_ptr->pips[1].y;
    ret_ptr->pips[2].w = CELL_W;
    ret_ptr->pips[2].h = CELL_H;

    ret_ptr->pips[3].x = ret_ptr->pips[2].x + CELL_W;
    ret_ptr->pips[3].y = ret_ptr->pips[2].y;
    ret_ptr->pips[3].w = CELL_W;
    ret_ptr->pips[3].h = CELL_H;
    
    return ret_ptr;
}

bool checked_move(piece_t* self, SDL_Point mag, game_board* playfield) {
    // try move piece
    move_piece(self, &mag);

    // check for collisions
    bool in_playfield = inside(self->pips, PIECE_CELLS, playfield->rect);
    bool piece_collision = false;
    for (int i = 0; i < playfield->count; i++) {
        if (piece_intersect(self, &playfield->pieces[i])) {
            piece_collision = true;
            break;
        }
    }

    // undo the move if nessecary
    SDL_Point v_inverse = invert_point(mag);
    if (piece_collision || !in_playfield) {
        move_piece(self, &v_inverse);
        return false;
    }
    return true;
}

