#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "aiv-vector.h"
#include "vec2.h"
#include "sprite.h"
#include "animator.h"
#include "rigidbody.h"

typedef struct{
    enum drawLayers layer;
    vec2 position;
    vec2 pivot; 
    aiv_vector * components;
    Sprite* texture;
    Animator* animator;
    rigidbody* rb;
    boolean active;
}GameObject;

typedef struct _s scene;
struct _component{
    GameObject* owner;
    void(*init)(struct _component *);
    void(*init_scene)(struct _component *, scene* s );
    void(*destructor)(struct _component *);
    void(*update)(struct _component *,float);
    void(*onCollision)(struct _component *,GameObject* other);
    void* data;
};
typedef struct _component component;

GameObject* new_gameobject(vec2 p, Texture* Texture);
GameObject* new_gameobject_layer(vec2 p, Texture* Texture, enum drawLayers layer);
GameObject* new_gameobject_no_texture(vec2 p);
void update_gameobject(GameObject* go, float delta_time);
void collision_handle(GameObject* go, GameObject* other);
void attach_component(GameObject* go, component * comp);
void attach_rigidbody(GameObject* go, rigidbody * rb);
void set_animator(GameObject* go, Animator* a);
void destroy_gameobject(GameObject* go);
void detach_component(GameObject* go, component* c);

#endif