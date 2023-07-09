#include "stax.h"
#include "SDL_ext.h"
#include "piece.h"

#define true 1
#define false 0

SDL_Surface* main_surface;

int gravity = 16;
int falling = true;
int g_counter = 0;

SDL_Rect playfield_rect = {
    RECT_UNDEF,
    RECT_UNDEF,
    FIELD_W_PX,
    FIELD_H_PX
};

struct dropped_pieces {
    int count;
    piece_t pieces[MAX_PIECES];
} dropped_pieces;

piece_t* create_j_piece() {
    piece_t* ret_ptr = (piece_t*) malloc(sizeof(piece_t));
    ret_ptr->color = SDL_MapRGB(main_surface->format, 0, 0, 255);
    ret_ptr->pips[0].x = FIELD_W_CELLS / 2 * CELL_W + left(&playfield_rect);
    ret_ptr->pips[0].y = top(&playfield_rect);
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

int drop_piece(piece_t* piece, Uint32 mag) {
    // try drop piece
    for (int i = 0; i < PIECE_CELLS; i++) {
        piece->pips[i].y += mag;
    }
    Uint32 new_top = topmost(piece->pips, PIECE_CELLS);
    Uint32 new_bottom = bottomost(piece->pips, PIECE_CELLS);

    // check for collisions
    int in_playfield = (new_top >= top(&playfield_rect)) && (new_bottom <= bottom(&playfield_rect));
    int piece_collision = false;
    for (int i = 0; i < dropped_pieces.count; i++) {
        if (piece_intersect(piece, &dropped_pieces.pieces[i])) {
            piece_collision = true;
            break;
        }
    }

    if (in_playfield && !piece_collision) {
        return true;
    }

    // undo drop
    for (int i = 0; i < PIECE_CELLS; i++) {
        piece->pips[i].y -= mag;
    }
    return false;
}

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

    main_surface = SDL_GetWindowSurface(main_window);
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

    center(&window_clip, &playfield_rect);
    SDL_Rect* playfield = outline_rect(&playfield_rect);

    piece_t* current_piece = create_j_piece();

    Uint32 das_delay = 0;
    dropped_pieces.count = 0;
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
                move_piece(current_piece, -CELL_W, &playfield_rect);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            if (das_delay == 0) {
                move_piece(current_piece, CELL_W, &playfield_rect);
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
            if (!drop_piece(current_piece, CELL_H)) {
                dropped_pieces.pieces[dropped_pieces.count++] = *current_piece;
                free(current_piece);
                if (dropped_pieces.count == MAX_PIECES) {
                    exit = true;
                }
                else {
                    current_piece = create_j_piece();
                }
            }
        }

        // draw the screen
        for (int i = 0; i < dropped_pieces.count; i++) {
            SDL_FillRects(
                    main_surface,
                    dropped_pieces.pieces[i].pips,
                    PIECE_CELLS,
                    dropped_pieces.pieces[i].color
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
    free(playfield);
    SDL_FreeSurface(main_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}
