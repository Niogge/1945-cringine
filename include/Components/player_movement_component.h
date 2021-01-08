#ifndef PLAYER_MOVEMENT_COMPONENT_H
#define PLAYER_MOVEMENT_COMPONENT_H

#include "gameObject.h"
#include "component-factory.h"

typedef struct{
    vec2 last_pos;
}movement_data;


void player_movement_init(component * self);
void player_movement_update(component* self,float dt);

void player_movement_destructor(component* self);

void player_movement_collision(component* self, GameObject* other);

component* new_player_movement();

#endif