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
    
}Clip;

Animator* new_animator();
Clip* new_clip(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps);
int add_clip(Animator* animator, Clip* clip);
vec2 update_animator(Animator* a, float delta_time);
void destroy_animator(Animator* a);
void destroy_clip(Clip*c);

#endif