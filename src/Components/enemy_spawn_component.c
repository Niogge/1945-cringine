#include "enemy_spawn_component.h"
#include "managers.h"
#include "scene.h"
#include <time.h>
#include <stdio.h>
#include <SDL.h>


GameObject* enemy_factory(scene* s){
    GameObject* enemy_go = new_gameobject(vec2_new(0,0),get_texture("Enemy_1"));
    rigidbody* enemy_rb = new_rb_with_collider(enemy_go->position, vec2_new(32,32));
    enemy_rb->collision_mask = BULLET_MASK | PLAYER_MASK;
    enemy_rb->mask_self = ENEMY_MASK;
    enemy_rb->direction = vec2_new(0,1);
    enemy_rb->speed = 100;
    attach_rigidbody(enemy_go, enemy_rb);
    //ADD ENEMY COMPONENTS
    component * enemy_behaviour = new_enemy_behaviour_component();
    attach_component(enemy_go, enemy_behaviour);
    enemy_behaviour->init_scene(enemy_behaviour,s);

    add_scene_object(s, enemy_go);
    return enemy_go;
}

void enemy_spawner_init(component * self, scene*s){
    srand(time(0));
    for (int i = 0; i < 20; i++)
    {
        GameObject* en_go = enemy_factory(s);
        en_go->active = false;
        aiv_vector_add(((enemy_spawner_data*)self->data)->enemy_pool, en_go);

    }
}
void enemy_spawner_update(component* self,float dt){
    if(((enemy_spawner_data*)self->data)->spawnCD_elapsed >=((enemy_spawner_data*)self->data)->spawnCD ){
            GameObject* enemy = (GameObject*) aiv_vector_pop(((enemy_spawner_data*)self->data)->enemy_pool);
            int minX =((enemy_spawner_data*)self->data)->XRange.x; 
            int maxX = ((((enemy_spawner_data*)self->data)->XRange.y - ((enemy_spawner_data*)self->data)->XRange.x));
            float Xpos =minX+ (rand()%maxX);         
            vec2 enemypos = vec2_new(Xpos,-50);
            enemy->position = enemypos;
            enemy->rb->pos = enemypos;
            enemy->active = true;
            aiv_vector_add(((enemy_spawner_data*)self->data)->enemy_pool, enemy);
            ((enemy_spawner_data*)self->data)->spawnCD_elapsed = 0;
             ((enemy_spawner_data*)self->data)->spawnCD -= ((enemy_spawner_data*)self->data)->spawnCD/100.f;
    }
    else{
        ((enemy_spawner_data*)self->data)->spawnCD_elapsed += dt;
    }
}

void enemy_spawner_destructor(component* self){
    for (int i = 0; i < ((enemy_spawner_data*)self->data)->enemy_pool->__count; i++)
    {
        destroy_gameobject(((enemy_spawner_data*)self->data)->enemy_pool->__items[i]);
    }
    aiv_vector_destroy(((enemy_spawner_data*)self->data)->enemy_pool);
    free(self->data);
    free(self);
}

void enemy_spawner_collision(component* self, GameObject* other){

}

component* new_enemy_spawner(){
    component* c = (component*)malloc(sizeof(component));
    c->init = NULL;
    c->init_scene = enemy_spawner_init;
    c->update = enemy_spawner_update;
    c->destructor =enemy_spawner_destructor;
    c->onCollision = enemy_spawner_collision;
    c->data = malloc(sizeof(shooting_data));
    ((enemy_spawner_data*)c->data)->enemy_pool = aiv_vector_new();
    ((enemy_spawner_data*)c->data)->spawnCD = 2.f;
    ((enemy_spawner_data*)c->data)->spawnCD_elapsed = 0.5f;
    ((enemy_spawner_data*)c->data)->XRange = vec2_new(15,600);
    return c;
}