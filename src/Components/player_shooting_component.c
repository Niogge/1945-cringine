#include "player_shooting_component.h"
#include "managers.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>

GameObject* bullet_factory(scene * s){
    GameObject* bullet_go = new_gameobject(vec2_new(180.f,180.f), get_texture( "bullet"));
    rigidbody* bullet_rigidbody = new_rb_with_collider(bullet_go->position, vec2_new(32,32));
    bullet_rigidbody->collision_mask = ENEMY_MASK | PLAYER_MASK;
    bullet_rigidbody->mask_self = BULLET_MASK;
    attach_rigidbody(bullet_go, bullet_rigidbody);

    component * bullet_c = new_bullet_behaviour();
    attach_component(bullet_go, bullet_c);
    bullet_c->init(bullet_c);

    add_scene_object(s, bullet_go);
    return bullet_go;
}


void player_shooting_init(component * self,scene* s){
    for (int i = 0; i < 50; i++)
    {
        GameObject* bullet_go = bullet_factory(s);
        bullet_go->active = false;
        aiv_vector_add(((shooting_data*)self->data)->pool, bullet_go);

    }
    
}


void player_shooting_update(component* self,float dt){

    if(((shooting_data*)self->data)->shootCD_elapsed>= ((shooting_data*)self->data)->shootCD){
        if(get_key(SDL_SCANCODE_SPACE)){
            GameObject* bullet = (GameObject*) aiv_vector_pop(((shooting_data*)self->data)->pool);
            vec2 bullpos = vec2_sum(&self->owner->position, &((shooting_data*)self->data)->shoot_offset );
            bullet->position = bullpos;
            bullet->rb->pos = bullpos;
            bullet->active = true;
            aiv_vector_add(((shooting_data*)self->data)->pool, bullet);
            ((shooting_data*)self->data)->shootCD_elapsed = 0;
        }
    }
    else{
        ((shooting_data*)self->data)->shootCD_elapsed += dt;
    }
}
void player_shooting_destructor(component* self){
    free(self);
}

void player_shooting_collision(component* self, GameObject* other){
    //self->owner->active = false;
}

component* new_player_shooting(){
    component* c = (component*)malloc(sizeof(component));
    c->init = NULL;
    c->init_scene = player_shooting_init;
    c->update = player_shooting_update;
    c->destructor =player_shooting_destructor;
    c->onCollision = player_shooting_collision;
    c->data = malloc(sizeof(shooting_data));
    ((shooting_data*)c->data)->pool = aiv_vector_new();
    ((shooting_data*)c->data)->shootCD = 0.1f;
    ((shooting_data*)c->data)->shootCD_elapsed = 0.5f;
    ((shooting_data*)c->data)->shoot_offset = vec2_new(0,-50);

    return c;
}