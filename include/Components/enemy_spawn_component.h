#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H
#include "gameObject.h"
#include "component-factory.h"

typedef struct{
    float spawnCD;
    float spawnCD_elapsed;
    aiv_vector* enemy_pool;
    vec2 XRange;
}enemy_spawner_data;


void enemy_spawner_init(component * self, scene *s);
void enemy_spawner_update(component* self,float dt);

void enemy_spawner_destructor(component* self);

void enemy_spawner_collision(component* self, GameObject* other);

component* new_enemy_spawner();



#endif