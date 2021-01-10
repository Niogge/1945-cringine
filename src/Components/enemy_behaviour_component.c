#include "enemy_behaviour_component.h"
#include "managers.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>

GameObject* enemy_bullet_factory(scene * s){
    GameObject* bullet_go = new_gameobject(vec2_new(180.f,180.f), get_texture( "enemy_bullet"));
    rigidbody* bullet_rigidbody = new_rb_with_collider(bullet_go->position, vec2_new(15,15));
    bullet_rigidbody->collision_mask = ENEMY_MASK | PLAYER_MASK | BULLET_MASK;
    bullet_rigidbody->mask_self = BULLET_MASK;
    attach_rigidbody(bullet_go, bullet_rigidbody);

    component * bullet_c = new_bullet_behaviour(vec2_new(0,1), 300);
    attach_component(bullet_go, bullet_c);
    bullet_c->init(bullet_c);

    add_scene_object(s, bullet_go);
    return bullet_go;
}


void enemy_behaviour_component_init(component* self){
}
void self_deactivate(void * go){
    ((GameObject*)go)->active =false;
}
void enemy_behaviour_component_init_scene(component* self, scene *s ){
    for (int i = 0; i < 10; i++)
    {
        GameObject* bullet_go = enemy_bullet_factory(s);
        bullet_go->active = false;
        aiv_vector_add(((enemy_behaviour_component_data*)self->data)->pool, bullet_go);

    }
    GameObject* expl = new_gameobject(self->owner->position,get_texture("explosion"));
    Animator * anim = new_animator();
    Clip* clip = new_clip_no_loop(vec2_new(0,0), vec2_new(6,0),65,65,10,self_deactivate);
    add_clip(anim,clip);
    set_animator(expl,anim);
    add_scene_object(s, expl);
    expl->active = false;
    enemy_behaviour_component_data* data = self->data;
    data->death_particle = expl;
}
void enemy_behaviour_component_update(component* self,float dt){
    if(((enemy_behaviour_component_data*)self->data)->shootCD_elapsed>= ((enemy_behaviour_component_data*)self->data)->shootCD){
        GameObject* bullet = (GameObject*) aiv_vector_pop(((enemy_behaviour_component_data*)self->data)->pool);
        vec2 bullpos = vec2_sum(&self->owner->position, &((enemy_behaviour_component_data*)self->data)->shoot_offset );
        bullet->position = bullpos;
        bullet->rb->pos = bullpos;
        bullet->active = true;
        aiv_vector_add(((enemy_behaviour_component_data*)self->data)->pool, bullet);
        ((enemy_behaviour_component_data*)self->data)->shootCD_elapsed = 0;
        
    }
    else{
        ((enemy_behaviour_component_data*)self->data)->shootCD_elapsed += dt;
    }
}
void enemy_behaviour_component_destructor(component* self){
    enemy_behaviour_component_data * data = self->data;

    //destroy_gameobject(data->death_particle);
    aiv_vector_destroy(data->pool);
    free(self);
}

void enemy_behaviour_component_on_collision(component* self, GameObject* other){
    self->owner->active = false;
    enemy_behaviour_component_data *data = self->data;
    data->death_particle->position = self->owner->position;
    data->death_particle->active =true;

    play_chunk(-1,1,1);
}
component* new_enemy_behaviour_component(){
    component* c = (component* )malloc(sizeof(component));
    c->init= enemy_behaviour_component_init;
    c->init_scene= enemy_behaviour_component_init_scene;
    c->update= enemy_behaviour_component_update;
    enemy_behaviour_component_data * data = (enemy_behaviour_component_data*)malloc(sizeof(enemy_behaviour_component_data));
    data->pool = aiv_vector_new();
    data->shootCD = 0.3f;
    data->shootCD_elapsed = 0.f;
    data->shoot_offset = vec2_new(0,50);
 
    /*fill in data*/
    c->data = data;
    c->destructor = enemy_behaviour_component_destructor;
    c->onCollision = enemy_behaviour_component_on_collision;
    return c;
}