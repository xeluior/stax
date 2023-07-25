/*
 * Stax: a classic falling block puzzle game
 * Copyright (C) 2023 Robert Gingras
 * 
 * Stax is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Stax is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Stax. If not, see <https://www.gnu.org/licenses/>. 
 */
#include <limits.h>
#include "SDL_ext.h"

int bottom(SDL_Rect* rect) {
    return rect->y + rect->h;
}

int left(SDL_Rect* rect) {
    return rect->x;
}

int right(SDL_Rect* rect) {
    return rect->x + rect->w;
}

int top(SDL_Rect* rect) {
    return rect->y;
}

int leftmost(SDL_Rect* rects, int count) {
    int leftmost = INT_MAX;
    for (int i = 0; i < count; i++) {
        if (left(&rects[i]) < leftmost) {
            leftmost = left(&rects[i]);
        }
    }
    return leftmost;
}

int rightmost(SDL_Rect* rects, int count) {
    int rightmost = 0;
    for (int i = 0; i < count; i++) {
        if (right(&rects[i]) > rightmost) {
            rightmost = right(&rects[i]);
        }
    }
    return rightmost;
}

int topmost(SDL_Rect* rects, int count) {
    int topmost = INT_MAX;
    for (int i = 0; i < count; i++) {
        if (top(&rects[i]) < topmost) {
            topmost = top(&rects[i]);
        }
    }
    return topmost;
}

int bottomost(SDL_Rect* rects, int count) {
    int bottomost = 0;
    for (int i = 0; i < count; i++) {
        if (bottom(&rects[i]) > bottomost) {
            bottomost = bottom(&rects[i]);
        }
    }
    return bottomost;
}

void center(SDL_Rect* parent, SDL_Rect* child) {
    child->x = (parent->w / 2) - (child->w / 2);
    child->y = (parent->h / 2) - (child->h / 2);
}

SDL_Rect* outline_rect(SDL_Rect* rect) {
    SDL_Rect* ret_ptr = calloc(4, sizeof(SDL_Rect));
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

bool inside(SDL_Rect* inners, int count, SDL_Rect* outer) {
    return (leftmost(inners, count) >= left(outer))
        && (rightmost(inners, count) <= right(outer))
        && (topmost(inners, count) >= top(outer))
        && (bottomost(inners, count) <= bottom(outer));
}

bool bucketted(SDL_Rect* inners, int count, SDL_Rect* outer) {
    return (leftmost(inners, count) >= left(outer))
        && (rightmost(inners, count) <= right(outer))
        && (bottomost(inners, count) <= bottom(outer));
}

SDL_Point invert_point(SDL_Point point) {
    SDL_Point ret = { -point.x, -point.y };
    return ret;
}

