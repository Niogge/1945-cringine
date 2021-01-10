#ifndef PLAYER_SHOOTING_COMPONENT_H
#define PLAYER_SHOOTING_COMPONENT_H

#include "gameObject.h"
#include "component-factory.h"

typedef struct{
    aiv_vector * pool;
    float shootCD;
    float shootCD_elapsed;
    vec2 shoot_offset;
}shooting_data;

void player_shooting_init(component * self, scene* s);

void player_shooting_update(component* self,float dt);
void player_shooting_destructor(component* self);

void player_shooting_collision(component* self, GameObject* other);

component* new_player_shooting();

#endif