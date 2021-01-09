#include "background_component.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>

void background_init(component * self){

}
void background_update(component* self,float dt){
    if(self->owner->position.y >= ((bkg_data*)self->data)->max_pos.y){
        self->owner->position.y = ((bkg_data*)self->data)->reset_position.y;
    }
    else{
        self->owner->position.y += ((bkg_data*)self->data)->scroll_speed*dt;
    }
}

void background_destructor(component* self){
    free(self->data);
    free(self);
}

void background_collision(component* self, GameObject* other){

}

component* new_background_component(vec2 r_pos, vec2 m_pos, float s_speed){
    component* c = (component*)malloc(sizeof(component));
    c->init = background_init;
    c->update = background_update;
    c->destructor =background_destructor;
    c->onCollision = background_collision;
    c->data = malloc(sizeof(bkg_data));
    ((bkg_data*)c->data)->max_pos= m_pos;
    ((bkg_data*)c->data)->reset_position= r_pos;
    ((bkg_data*)c->data)->scroll_speed= s_speed;
    return c;
}