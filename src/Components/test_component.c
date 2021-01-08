#include "test_component.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>

void test_init(component* self){
    printf("Hello\n");
}

void test_update(component* self,float dt){
   if(get_key(SDL_SCANCODE_P)){
       self->owner->rb->speed = 10;
   }
   else{
       self->owner->rb->speed = 10;
   }
}

void test_destructor(component* self){
    free(self->data);
    free(self);
}

void test_on_collision(component* self, GameObject* other){
    int p =  self->owner->position.x;
    printf("colliding %d\n", p );

}


component* new_test_component(){
    component* c = (component* )malloc(sizeof(component));
    c->init= test_init;
    c->update= test_update;
    test_data* td = (test_data*)malloc(sizeof(test_data));
    td->a = 1;
    td->b = 2;
    td->s = "cane";
    c->data = td;
    c->destructor = test_destructor;
    c->onCollision = test_on_collision;

    return c;
}

