#include "my_component.h"
#include "managers.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>

void my_component_init(component* self){
    /***/
}
void my_component_init_scene(component* self, scene *s ){
    /***/
}

void my_component_update(component* self,float dt){
    /***/
}
void my_component_destructor(component* self){
    /***/
}
void my_component_on_collision(component* self, GameObject* other){
    /***/
}
component* new_my_component(){
    component* c = (component* )malloc(sizeof(component));
    c->init= my_component_init;
    c->init_scene= my_component_init_scene;
    c->update= my_component_update;
    my_component_data * data = (my_component_data*)malloc(sizeof(my_component_data));
    /*fill in data*/
    c->data = data;
    c->destructor = my_component_destructor;
    c->onCollision = my_component_on_collision;
    return c;
}