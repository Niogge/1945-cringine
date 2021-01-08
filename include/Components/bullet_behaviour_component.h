#ifndef BULLET_BEHAVIOUR_COMPONENT_H
#define BULLET_BEHAVIOUR_COMPONENT_H
#include "gameObject.h"
#include "component-factory.h"

void bullet_behaviour_init(component * self);
void bullet_behaviour_update(component* self,float dt);
void bullet_behaviour_destructor(component* self);

void bullet_behaviour_collision(component* self, GameObject* other);

component* new_bullet_behaviour();

#endif