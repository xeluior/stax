#include <SDL2/SDL.h>
#include <limits.h>
#include <stdio.h>

#define GAME_NAME "Stax"
#define VERSION "v0.1.0"
#define WIN_TITLE GAME_NAME " " VERSION
#define SCREEN_W 640
#define SCREEN_H 480
#define CELL_W 20
#define CELL_H 20
#define RECT_UNDEF 0
#define FIELD_W_CELLS 10
#define FIELD_H_CELLS 20
#define FIELD_W_PX FIELD_W_CELLS * CELL_W
#define FIELD_H_PX FIELD_H_CELLS * CELL_H
#define FPS 240
#define FRAME_MS 1000 / FPS
#define DAS_DELAY 10
#define PIECE_CELLS 4
#define MAX_PIECES FIELD_W_CELLS * FIELD_H_CELLS / PIECE_CELLS

#define true 1
#define false 0
#define bool int

SDL_Surface* main_surface;

Uint32 gravity = 16;
bool falling = true;
Uint32 g_counter = 0;

SDL_Rect playfield_rect = {
    RECT_UNDEF,
    RECT_UNDEF,
    FIELD_W_PX,
    FIELD_H_PX
};

typedef struct piece {
    SDL_Rect pips[PIECE_CELLS];
    Uint32 color;
} piece_t;

struct dropped_pieces {
    int count;
    piece_t pieces[MAX_PIECES];
} dropped_pieces;

// Sets the x and y of child such that the center of child is placed as the
// center of parent
void center(SDL_Rect* parent, SDL_Rect* child) {
    child->x = (parent->w / 2) - (child->w / 2);
    child->y = (parent->h / 2) - (child->h / 2);
}

// Returns a pointer to an array of 4 SDL_Rects each of which is only 1 pixel
// wide or tall arranged such that each one follows one of the edges of the
// input rect. There is one pixel of overlap on each of the corners. The result
// should be freed when it is no longer needed.
SDL_Rect* outline_rect(SDL_Rect* rect) {
    SDL_Rect* ret_ptr = (SDL_Rect*) malloc(sizeof(SDL_Rect)*4);
    ret_ptr[0].x = rect->x;
    ret_ptr[0].y = rect->y;
    ret_ptr[0].w = rect->w;
    ret_ptr[0].h = 1;

    ret_ptr[1].x = rect->x;
    ret_ptr[1].y = rect->y;
    ret_ptr[1].w = 1;
    ret_ptr[1].h = rect->h;

    ret_ptr[2].x = rect->x;
    ret_ptr[2].y = rect->y + rect->h;
    ret_ptr[2].w = rect->w;
    ret_ptr[2].h = 1;

    ret_ptr[3].x = rect->x + rect->w;
    ret_ptr[3].y = rect->y;
    ret_ptr[3].w = 1;
    ret_ptr[3].h = rect->h;
    return ret_ptr;
}

Uint32 bottom(SDL_Rect* rect) {
    return rect->y + rect->h;
}

Uint32 left(SDL_Rect* rect) {
    return rect->x;
}

Uint32 right(SDL_Rect* rect) {
    return rect->x + rect->w;
}

Uint32 top(SDL_Rect* rect) {
    return rect->y;
}

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

Uint32 leftmost(SDL_Rect* rects, int count) {
    Uint32 leftmost = INT_MAX;
    for (int i = 0; i < count; i++) {
        if (left(&rects[i]) < leftmost) {
            leftmost = left(&rects[i]);
        }
    }
    return leftmost;
}

Uint32 rightmost(SDL_Rect* rects, int count) {
    Uint32 rightmost = 0;
    for (int i = 0; i < count; i++) {
        if (right(&rects[i]) > rightmost) {
            rightmost = right(&rects[i]);
        }
    }
    return rightmost;
}

Uint32 topmost(SDL_Rect* rects, int count) {
    Uint32 topmost = 0;
    for (int i = 0; i < count; i++) {
        if (top(&rects[i]) > topmost) {
            topmost = top(&rects[i]);
        }
    }
    return topmost;
}

Uint32 bottomost(SDL_Rect* rects, int count) {
    Uint32 bottomost = 0;
    for (int i = 0; i < count; i++) {
        if (bottom(&rects[i]) > bottomost) {
            bottomost = bottom(&rects[i]);
        }
    }
    return bottomost;
}

// attempts to move the piece along the X axis by mag. Will not move the piece
// if it would place it outside of the playfield rect
void move(SDL_Rect* piece, Uint32 mag) {
    Uint32 new_x = piece->x + mag;
    if (new_x >= left(&playfield_rect)
        && new_x + piece->w <= right(&playfield_rect))
    {
        piece->x = new_x;
    }
}

bool move_piece(piece_t* piece, Uint32 mag) {
    Uint32 new_left = leftmost(piece->pips, PIECE_CELLS) + mag;
    Uint32 new_right = rightmost(piece->pips, PIECE_CELLS) + mag;
    if (new_left >= left(&playfield_rect) && new_right <= right(&playfield_rect)) {
        for (int i = 0; i < PIECE_CELLS; i++) {
            piece->pips[i].x += mag;
        }
    }
    return leftmost(piece->pips, PIECE_CELLS) == new_left;
}

bool piece_intersect(piece_t* piece_a, piece_t* piece_b) {
    for (int i = 0; i < PIECE_CELLS; i++) {
        for (int j = 0; j < PIECE_CELLS; j++) {
            if (SDL_HasIntersection(&piece_a->pips[i], &piece_b->pips[j])) {
                return true;
            }
        }
    }
    return false;
}

bool drop_piece(piece_t* piece, Uint32 mag) {
    // try drop piece
    for (int i = 0; i < PIECE_CELLS; i++) {
        piece->pips[i].y += mag;
    }
    Uint32 new_top = topmost(piece->pips, PIECE_CELLS);
    Uint32 new_bottom = bottomost(piece->pips, PIECE_CELLS);

    // check for collisions
    bool in_playfield = (new_top >= top(&playfield_rect)) && (new_bottom <= bottom(&playfield_rect));
    bool piece_collision = false;
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
    Uint32 red   = SDL_MapRGB(main_surface->format, 255, 0, 0);
    Uint32 black = SDL_MapRGB(main_surface->format, 0, 0, 0);

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    // start game
    SDL_ShowWindow(main_window);
    bool exit = false;
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
                move_piece(current_piece, -CELL_W);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            if (das_delay == 0) {
                move_piece(current_piece, CELL_W);
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
