#include "animator.h"

Animator* new_animator(){
    Animator* a = (Animator*)malloc(sizeof(Animator));
    a->animations = aiv_vector_new();
    a->current_animation = 0;
}
Clip* new_clip(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps){
    Clip*c = (Clip*) malloc(sizeof(Clip));
    c->clipLength =((end.x - start.x + 1)*(end.y-start.y+1));
    c->currentFrame=0;
    c->currentFrameTime = 0.f;
    c->secondsPerFrame = 1./fps;
    c->rectPositions = (vec2*)malloc(c->clipLength* sizeof(vec2));
    int currTile = 0;
    for (int i = start.y; i <=end.y; i++)
    {
        for (int j = start.x; j <= end.x; j++)
        {
            c->rectPositions[currTile] = vec2_new(j*frameWidth, i*frameHeight);
            currTile++;
        }
        
    }
    return c;
}

vec2 update_animator(Animator* a, float delta_time){
    Clip * currentClip = (Clip*)aiv_vector_at(a->animations, a->current_animation);
    currentClip->currentFrameTime += delta_time;
    if(currentClip->currentFrameTime >= currentClip->secondsPerFrame){
        currentClip->currentFrame= (currentClip->currentFrame+1)%currentClip->clipLength;
        currentClip->currentFrameTime = 0;
    }
    return currentClip->rectPositions[currentClip->currentFrame];
}
int add_clip(Animator* animator, Clip* clip){
    aiv_vector_add(animator->animations,clip);
    return animator->animations->__count -1;
}
void destroy_animator(Animator* a){
    for (int i = 0; i < a->animations->__count; i++)
    {
        destroy_clip(a->animations->__items[i]);
    }
    free(a);
}
void destroy_clip(Clip*c){
    for (int i = 0; i < c->clipLength; i++)
    {
        free(c->rectPositions);
    }
    free(c);
}