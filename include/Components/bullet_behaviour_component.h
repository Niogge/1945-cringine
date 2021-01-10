#ifndef BULLET_BEHAVIOUR_COMPONENT_H
#define BULLET_BEHAVIOUR_COMPONENT_H
#include "gameObject.h"
#include "component-factory.h"
typedef struct{
    vec2 dir;
    float speed;
    float life;
    float max_life;
}bullet_behaviour_data;
void bullet_behaviour_init(component * self);
void bullet_behaviour_update(component* self,float dt);
void bullet_behaviour_destructor(component* self);

void bullet_behaviour_collision(component* self, GameObject* other);

component* new_bullet_behaviour(vec2 direction, float speed);

#endif