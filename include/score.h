#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <limits.h>

#define FONT_SIZE 36
#define SCORE_FMT "%010u"

typedef struct score {
    unsigned int score;
    char text[10];
    TTF_Font* font;
    SDL_Color color;
    SDL_Rect rect;
    SDL_Surface* surface;
} score;

// modifies score by mag, bounded by uint32 limits
score* init_score(void);
void update_score(score* self, int mag);
void draw_score(score* self, SDL_Surface* dst);
void free_score(score* self);

