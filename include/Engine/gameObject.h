#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "aiv-vector.h"
#include "vec2.h"
#include "sprite.h"
#include "animator.h"
typedef struct{
    vec2 position;
    aiv_vector * components;
    Sprite* texture;
    Animator* animator;
}GameObject;

struct _component{
    GameObject* owner;
    void(*init)(struct _component *);
    void(*destructor)(struct _component *);
    void(*update)(struct _component *,float);
    void* data;
};
typedef struct _component component;


GameObject* new_gameobject(vec2 p, Texture* Texture);
GameObject* new_gameobject_no_texture(vec2 p);
void update_gameobject(GameObject* go, float delta_time);
void attach_component(GameObject* go, component * comp);
void set_animator(GameObject* go, Animator* a);
void destroy_gameobject(GameObject* go);
void detach_component(GameObject* go, component* c);

#endif