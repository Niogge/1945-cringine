#ifndef LIVES_HANDLER_COMPONENT_H
#define LIVES_HANDLER_COMPONENT_H
#include "gameObject.h"
#include "component-factory.h"
typedef struct{
    component* player_movement;
    GameObject* l1;
    GameObject* l2;
    GameObject* l3;
} lives_handler_component_data;
void lives_handler_component_init(component* self);
void lives_handler_component_init_scene(component* self, scene *s);
void lives_handler_component_update(component* self,float dt);
void lives_handler_component_destructor(component* self);
void lives_handler_component_on_collision(component* self, GameObject* other);
component* new_lives_handler_component(component* pm, scene*s);
#endif