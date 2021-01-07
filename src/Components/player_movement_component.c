#include "gameObject.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>

typedef struct{

    vec2 last_pos;
}movement_data;


void player_movement_init(component * self){
    printf("player movement initalized\n");
    self->owner->rb->speed = 50;
}

void player_movement_update(component* self,float dt){
    vec2 dir = vec2_new(0,0);
    if(get_key(SDL_SCANCODE_UP) || get_key(SDL_SCANCODE_W)){
        dir.y = -1;
    }
    else if(get_key(SDL_SCANCODE_DOWN) || get_key(SDL_SCANCODE_S)){
        dir.y = 1;
    }
    else{
        dir.y = 0;
    }

    if(get_key(SDL_SCANCODE_RIGHT) || get_key(SDL_SCANCODE_D)){
        dir.x = 1;
    }
    else if(get_key(SDL_SCANCODE_LEFT) || get_key(SDL_SCANCODE_A)){
        dir.x=-1;
    }
    else{
        dir.x = 0;
    }
    dir = vec2_norm(&dir);
    self->owner->rb->direction = dir;
    ((movement_data*)self->data)->last_pos = self->owner->position;
}

void player_movement_destructor(component* self){
    free(self);
}

void player_movement_collision(component* self, GameObject* other){
    self->owner->position = ((movement_data*)self->data)->last_pos;
    self->owner->rb->pos = ((movement_data*)self->data)->last_pos;
    self->owner->rb->direction = vec2_new(0,0);
}

component* new_player_movement(){
    component* c = (component*)malloc(sizeof(component));
    c->init = player_movement_init;
    c->update = player_movement_update;
    c->data = (movement_data*)malloc(sizeof(movement_data));
    ((movement_data*)c->data)->last_pos = vec2_new(0,0);
    c->destructor =player_movement_destructor;
    c->onCollision = player_movement_collision;

    return c;
}