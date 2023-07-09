#ifndef _SDL_EXT_H
#define _SDL_EXT_H
#include <SDL2/SDL_rect.h>

// returns the the x or y value of the given side of the rect. O(c)
int bottom(SDL_Rect* rect);
int left(SDL_Rect* rect);
int right(SDL_Rect* rect);
int top(SDL_Rect* rect);

// returns the x or y value that is furthest in the array of rects
// O(n) n = count
int leftmost(SDL_Rect* rects, int count);
int rightmost(SDL_Rect* rects, int count);
int topmost(SDL_Rect* rects, int count);
int bottomost(SDL_Rect* rects, int count);

// Sets the x and y of child such that the center of child is placed as
// the center of parent. O(c)
void center(SDL_Rect* parent, SDL_Rect* child);

// Returns a pointer to an array of 4 SDL_Rects each of which is only 1
// pixel wide or tall arranged such that each one follows one of the
// edges of the input rect. There is one pixel of overlap on each of
// the corners. The result should be freed when it is no longer needed.
// O(c)
SDL_Rect* outline_rect(SDL_Rect* rect);

#endif

