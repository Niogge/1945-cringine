#include "lives_handler_component.h"
#include "managers.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>
void lives_handler_component_init(component* self){
}
void lives_handler_component_init_scene(component* self, scene *s ){
}
void lives_handler_component_update(component* self,float dt){
    lives_handler_component_data * data = self->data;
    movement_data * p_data = data->player_movement->data;
    data->l1->active = p_data->lives >=1? true:false;
    data->l2->active = p_data->lives >=2? true:false;
    data->l3->active = p_data->lives >=3? true:false;
}
void lives_handler_component_destructor(component* self){
    free(self);
}
void lives_handler_component_on_collision(component* self, GameObject* other){
}
component* new_lives_handler_component(component* player_m, scene* s){
    component* c = (component* )malloc(sizeof(component));
    c->init= lives_handler_component_init;
    c->init_scene= lives_handler_component_init_scene;
    c->update= lives_handler_component_update;
    lives_handler_component_data * data = (lives_handler_component_data*)malloc(sizeof(lives_handler_component_data));
    data->player_movement = player_m;
    data->l1 = new_gameobject_layer(vec2_new(20,430),get_texture("life"),FORE);
    add_scene_object(s,data->l1);
    data->l2 = new_gameobject_layer(vec2_new(52,430),get_texture("life"),FORE);
    add_scene_object(s,data->l2);
    data->l3 = new_gameobject_layer(vec2_new(82,430),get_texture("life"),FORE);
    add_scene_object(s,data->l3);
    /*fill in data*/
    c->data = data;
    c->destructor = lives_handler_component_destructor;
    c->onCollision = lives_handler_component_on_collision;
    return c;
}