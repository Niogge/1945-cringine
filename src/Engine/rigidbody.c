#include "rigidbody.h"
#include <stddef.h>
#include<stdlib.h>
rigidbody* new_rb(){
    rigidbody*rb = (rigidbody*)malloc(sizeof(rigidbody));
    rb->pos = vec2_new(0,0);
    rb->direction = vec2_new(0,0);
    rb->coll = NULL;
    rb->speed = 0.f;
    rb->mask_self = NULL_MASK;
    rb->collision_mask = NULL_MASK;
    return rb;
}
rigidbody* new_rb_with_collider(vec2 collider_offset, vec2 collider_size){
    rigidbody* rb = new_rb();
    rb->coll = (box_collider*) malloc(sizeof(box_collider));
    rb->coll->offset = collider_offset;
    rb->coll->size = collider_size;
    return rb;
}
void rb_set_mask_self(rigidbody*rb, int selfMask){
    rb->mask_self = selfMask;
}
void rb_set_mask_collision(rigidbody*rb, int collMask){
    rb->collision_mask = collMask;
}
void destroy_rb(rigidbody *rb){
    if(rb->coll!= NULL){
        destroy_collider(rb->coll);
    }
    free(rb);
}

void destroy_collider(box_collider* box){
    free(box);
}