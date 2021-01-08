#include"bullet_behaviour_component.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>

void bullet_behaviour_init(component * self){
    self->owner->rb->direction = vec2_new(0,-1);
    self->owner->rb->speed = 300;
}

void bullet_behaviour_update(component* self,float dt){
    
}
void bullet_behaviour_destructor(component* self){
    free(self);
}

void bullet_behaviour_collision(component* self, GameObject* other){
    self->owner->active = false;
}

component* new_bullet_behaviour(){
    component* c = (component*)malloc(sizeof(component));
    c->init = bullet_behaviour_init;
    c->update = bullet_behaviour_update;
    c->destructor =bullet_behaviour_destructor;
    c->onCollision = bullet_behaviour_collision;

    return c;
}