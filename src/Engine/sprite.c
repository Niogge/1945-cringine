#include "sprite.h"

Sprite* new_sprite(Texture* t){
    Sprite* txt = (Sprite*) malloc(sizeof(Sprite));
    txt->texture = t->txt;
    txt->renderFrame = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    txt->renderFrame->x = 0;
    txt->renderFrame->y = 0;
    txt->renderFrame->h = t->h;
    txt->renderFrame->w = t->w;
    txt->renderQuad = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    txt->renderQuad->x = 0;
    txt->renderQuad->y = 0;
    txt->renderQuad->h = t->h;
    txt->renderQuad->w = t->w;
    return txt;
}

void destroy_sprite(Sprite* sprite){
    free(sprite->renderFrame);
    free(sprite->renderQuad);
    free(sprite);
}
