#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <limits.h>
#include <string.h>

#define FONT_SIZE 36
#define FMT_LEN CHAR_MAX

typedef struct ui_number {
    int number;
    char text[10];
    char* fmt;
    TTF_Font* font;
    SDL_Color color;
    SDL_Rect rect;
    SDL_Surface* surface;
} ui_number;

// modifies score by mag, bounded by uint32 limits
ui_number* init_number(char* fmt, int number);
void set_number(ui_number* self, int mag);
void draw_number(ui_number* self, SDL_Surface* dst);
void free_number(ui_number* self);

