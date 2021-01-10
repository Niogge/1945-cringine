#include"bullet_behaviour_component.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>

void bullet_behaviour_init(component * self){
    self->owner->rb->direction = ((bullet_behaviour_data*)self->data)->dir;
    self->owner->rb->speed = ((bullet_behaviour_data*)self->data)->speed;
}

void bullet_behaviour_update(component* self,float dt){
    bullet_behaviour_data* data = self->data;
    data->life -= dt;
    if(data->life <= 0){
        data->life= data->max_life;
        self->owner->active = false;
    }
}
void bullet_behaviour_destructor(component* self){
    free(self);
}

void bullet_behaviour_collision(component* self, GameObject* other){
    self->owner->active = false;
}

component* new_bullet_behaviour(vec2 direction, float speed){
    component* c = (component*)malloc(sizeof(component));
    c->init = bullet_behaviour_init;
    c->update = bullet_behaviour_update;
    bullet_behaviour_data* data= (bullet_behaviour_data*)malloc(sizeof(bullet_behaviour_data));
    data->dir = direction;
    data->speed = speed;
    data->life = 5;
    data->max_life = 5;
    c->data = data;
    c->destructor =bullet_behaviour_destructor;
    c->onCollision = bullet_behaviour_collision;

    return c;
}