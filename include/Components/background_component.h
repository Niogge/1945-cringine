#ifndef BACKGROUND_COMPONENT_H
#define BACKGROUND_COMPONENT_H

#include "gameObject.h"
#include "component-factory.h"


typedef struct{
    vec2 reset_position;
    vec2 max_pos;
    float scroll_speed;
}bkg_data;
void background_init(component * self);
void background_update(component* self,float dt);

void background_destructor(component* self);

void background_collision(component* self, GameObject* other);

component* new_background_component(vec2 r_pos, vec2 m_pos,float speed);
#endif