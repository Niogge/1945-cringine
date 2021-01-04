#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL.h>
typedef struct{
    SDL_Texture* txt;
    int w;
    int h;
}Texture;

typedef struct {
    SDL_Texture* texture;
    SDL_Rect* renderFrame;
    SDL_Rect* renderQuad; 
}Sprite; 

Sprite* new_sprite(Texture* t);

void destroy_sprite(Sprite* txt);

#endif