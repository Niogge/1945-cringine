#include "player_movement_component.h"
#include "managers.h"
#include "animator.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>

void player_movement_init(component * self){
    printf("player movement initalized\n");
    self->owner->rb->speed = 100;
}

void player_movement_update(component* self,float dt){

    movement_data* data = ((movement_data*)self->data);

    vec2 dir = vec2_new(0,0);
    if(get_key(SDL_SCANCODE_UP) || get_key(SDL_SCANCODE_W)){
        dir.y = -1;
    }
    else if(get_key(SDL_SCANCODE_DOWN) || get_key(SDL_SCANCODE_S)){
        dir.y = 1;
    }
    else{
        dir.y = 0;
    }

    if(get_key(SDL_SCANCODE_RIGHT) || get_key(SDL_SCANCODE_D)){
        dir.x = 1;
    }
    else if(get_key(SDL_SCANCODE_LEFT) || get_key(SDL_SCANCODE_A)){
        dir.x=-1;
    }
    else{
        dir.x = 0;
    }
    dir = vec2_norm(&dir);
    self->owner->rb->direction = dir;
    data->last_pos = self->owner->position;



    if(!data->can_hit){
        data->hitCD_elapsed += dt;
        if(data->hitCD_elapsed> data->hitCD){
            data->can_hit = true;
            set_clip(self->owner->animator, 0 );
        }
    }
}

void player_movement_destructor(component* self){
    free(self->data);
    free(self);
}

void self_destruct(void * go){
    destroy_gameobject(go);
}
void player_death(GameObject* go){
    GameObject* expl = new_gameobject(go->position,get_texture("explosion"));
    Animator * anim = new_animator();
    Clip* clip = new_clip_no_loop(vec2_new(0,0), vec2_new(6,0),65,65,10,self_destruct);
    add_clip(anim,clip);
    set_animator(expl,anim);
    add_scene_object(current_scene(), expl);


    play_chunk(-1,1,1);

}
void player_movement_collision(component* self, GameObject* other){
    // self->owner->position = ((movement_data*)self->data)->last_pos;
    // self->owner->rb->pos = ((movement_data*)self->data)->last_pos;
    // self->owner->rb->direction = vec2_new(0,0);
    movement_data* data = ((movement_data*)self->data);

    if(data->can_hit){
        data->can_hit=false;
        data->hitCD_elapsed =0;
       set_clip(self->owner->animator,1);
        data->lives--;
        if(data->lives<=0){
            player_death(self->owner);
            self->owner->active = false;
        }
    }

}


component* new_player_movement(){
    component* c = (component*)malloc(sizeof(component));
    c->init = player_movement_init;
    c->update = player_movement_update;
    movement_data* data = (movement_data*)malloc(sizeof(movement_data));
    data->last_pos = vec2_new(0,0);
    data->can_hit = true;
    data->hitCD = 1.5f;
    data->hitCD_elapsed = 0.f;
    data->lives = 3;
    c->data = data;
    c->destructor =player_movement_destructor;
    c->onCollision = player_movement_collision;

    return c;
}