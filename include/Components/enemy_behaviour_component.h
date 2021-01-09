#ifndef ENEMY_BEHAVIOUR_COMPONENT_H
#define ENEMY_BEHAVIOUR_COMPONENT_H
#include "gameObject.h"
#include "component-factory.h"
typedef struct{
    aiv_vector * pool;
    float shootCD;
    float shootCD_elapsed;
    vec2 shoot_offset;
} enemy_behaviour_component_data;
void enemy_behaviour_component_init(component* self);
void enemy_behaviour_component_init_scene(component* self, scene *s);
void enemy_behaviour_component_update(component* self,float dt);
void enemy_behaviour_component_destructor(component* self);
void enemy_behaviour_component_on_collision(component* self, GameObject* other);
component* new_enemy_behaviour_component();
#endif
