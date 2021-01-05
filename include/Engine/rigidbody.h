#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include "vec2.h"

typedef enum {
    NULL_MASK = 0,
    PLAYER_MASK = 1,
    ENEMY_MASK = 2,
    BULLET_MASK = 4
}mask;
typedef struct{
    vec2 offset;
    vec2 size;
}box_collider;

typedef struct {
    vec2 pos; 
    vec2 direction;
    float speed;
    box_collider* coll;
    mask mask_self;
    mask collision_mask;
}rigidbody;


rigidbody* new_rb();
rigidbody* new_rb_with_collider(vec2 collider_offset, vec2 collider_size);
void rb_set_mask_self(rigidbody*rb, int selfMask);
void rb_set_mask_collision(rigidbody*rb, int collMask);
void destroy_rb(rigidbody *rb);
void destroy_collider(box_collider* box);
#endif