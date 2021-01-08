#ifndef GAME_H
#define GAME_H
#include "gameObject.h"
#include "managers.h"
#include "aiv-common.h"
#include <SDL.h>


typedef struct{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height; 

}Game;


Game* game_new(int w, int h, const char * name);

void game_end(Game* game);

void game_loop(Game* game);
#endif;