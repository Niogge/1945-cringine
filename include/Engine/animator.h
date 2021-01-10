#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "aiv-vector.h"
#include "vec2.h"
typedef struct{
    aiv_vector* animations;
    int current_animation;    
}Animator;
typedef struct{
    int clipLength;
    vec2* rectPositions;
    int currentFrame;
    float secondsPerFrame;
    float currentFrameTime;
    boolean loop;
    void (*onEnd)(void * go);
}Clip;

Animator* new_animator();
Clip* new_clip(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps);
Clip* new_clip_no_loop(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps, void(*func)(void*));
int add_clip(Animator* animator, Clip* clip);
vec2 update_animator(Animator* a, float delta_time, void* go);
void set_clip(Animator *a, int clip_index);
void destroy_animator(Animator* a);
void destroy_clip(Clip*c);

#endif