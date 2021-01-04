#include "gameObject.h"
#include "managers.h"
#include <stdio.h>
#include <SDL.h>
typedef struct{
    int a;
    int b;
    char* s;
} test_data;

void test_init(component* self){
    printf("Hello\n");
}

void test_update(component* self,float dt){
   if(get_key(input_manager, SDL_SCANCODE_P)){
       vec2 p = self->owner->position;
       vec2 p2 = vec2_new(p.x + 10.f*dt,p.y);
       self->owner->position = p2;
   }
}

void test_destructor(component* self){
    free(self->data);
    free(self);
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

    return c;
}

