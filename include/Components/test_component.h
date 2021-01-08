#ifndef TEST_COMPONENT_H
#define TEST_COMPONENT_H


#include "gameObject.h"
#include "component-factory.h"
typedef struct{
    int a;
    int b;
    char* s;
} test_data;

void test_init(component* self);

void test_update(component* self,float dt);

void test_destructor(component* self);

void test_on_collision(component* self, GameObject* other);

component* new_test_component();

#endif