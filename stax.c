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
#define FPS 60
#define FRAME_MS 1000 / FPS
#define DAS_DELAY 10

#define true 1
#define false 0
#define loop while(true)

Uint32 gravity = 48;
Uint32 g_counter = 0;

SDL_Rect playfield_rect = {
    RECT_UNDEF,
    RECT_UNDEF,
    FIELD_W_PX,
    FIELD_H_PX
};

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

    center(&window_clip, &playfield_rect);
    SDL_Rect* playfield = outline_rect(&playfield_rect);

    SDL_Rect current_piece = {
        FIELD_W_CELLS / 2 * CELL_W + left(&playfield_rect),
        top(&playfield_rect),
        CELL_W,
        CELL_H
    };

    Uint32 das_delay = 0;
    // End game initialization

    // define colors
    Uint32 white = SDL_MapRGB(main_surface->format, 255, 255, 255);
    Uint32 red   = SDL_MapRGB(main_surface->format, 255, 0, 0);
    Uint32 black = SDL_MapRGB(main_surface->format, 0, 0, 0);

    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    // start game
    SDL_ShowWindow(main_window);
    loop {
        Uint64 start_of_frame = SDL_GetTicks64();
        SDL_FillRect(main_surface, &window_clip, black);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    goto exit;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            goto exit;
                    }
            }
        }
        if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_C]) {
            goto exit;
        }

        if (keyboard_state[SDL_SCANCODE_LEFT]) {
            if (das_delay == 0) {
                move(&current_piece, -CELL_W);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            if (das_delay == 0) {
                move(&current_piece, CELL_W);
                das_delay = DAS_DELAY;
            }
            else {
                das_delay--;
            }
        }
        else {
            das_delay = 0;
        }

        g_counter += 1;
        if (g_counter >= gravity) {
            g_counter = 0;
            current_piece.y += CELL_H;
        }
        if (bottom(&current_piece) == bottom(&playfield_rect)) {
            gravity = INT_MAX;
        }
        SDL_FillRect(main_surface, &current_piece, red);
        SDL_FillRects(main_surface, playfield, 4, white);
        SDL_UpdateWindowSurface(main_window);

        Uint64 time_since_start = SDL_GetTicks64() - start_of_frame;
        SDL_Delay(FRAME_MS - time_since_start);
    }

exit:
    free(playfield);
    SDL_FreeSurface(main_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}
