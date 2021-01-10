#include "animator.h"
#include "gameObject.h"
Animator* new_animator(){
    Animator* a = (Animator*)malloc(sizeof(Animator));
    a->animations = aiv_vector_new();
    a->current_animation = 0;
    return a;
}
Clip* new_clip(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps){
    Clip*c = (Clip*) malloc(sizeof(Clip));
    c->clipLength =(int)((end.x - start.x + 1)*(end.y-start.y+1));
    c->currentFrame=0;
    c->currentFrameTime = 0.f;
    c->secondsPerFrame = 1.f/fps;
    c->rectPositions = (vec2*)malloc(c->clipLength* sizeof(vec2));
    int currTile = 0;
    for (int i = start.y; i <=end.y; i++)
    {
        for (int j = start.x; j <= end.x; j++)
        {
            c->rectPositions[currTile] = vec2_new((float)j*frameWidth, (float) i*frameHeight);
            currTile++;
        }
        
    }
    c->loop = true;
    return c;
}
Clip* new_clip_no_loop(vec2 start, vec2 end, int frameWidth, int frameHeight, int fps, void(*OnEnd)(void*)){
    Clip* c = new_clip(start,end,frameWidth,frameHeight,fps);
    c->loop =false;
    c->onEnd = OnEnd;
    return c;
}
vec2 update_animator(Animator* a, float delta_time,void * go){
    Clip * currentClip = (Clip*)aiv_vector_at(a->animations, a->current_animation);
    currentClip->currentFrameTime += delta_time;
    if(currentClip->currentFrameTime >= currentClip->secondsPerFrame){
        
        if(currentClip->loop){
            currentClip->currentFrame= (currentClip->currentFrame+1)%currentClip->clipLength;
            currentClip->currentFrameTime = 0;
        }
        else{
            if(currentClip->currentFrame +1 >= currentClip->clipLength){
                currentClip->onEnd(go);
                currentClip->currentFrameTime = 0;
            }
            else{
                currentClip->currentFrame+=1;
                currentClip->currentFrameTime =0;
            }
        }
        
    }
    return currentClip->rectPositions[currentClip->currentFrame];
}
int add_clip(Animator* animator, Clip* clip){
    aiv_vector_add(animator->animations,clip);
    return animator->animations->__count -1;
}
void destroy_animator(Animator* a){
    for (uint i = 0; i < a->animations->__count; i++)
    {
        destroy_clip(a->animations->__items[i]);
    }
    free(a);
}
void set_clip(Animator *a, int clip_index){
    Clip * currentClip = (Clip*)aiv_vector_at(a->animations, a->current_animation);
    currentClip->currentFrameTime=0;
    currentClip->currentFrame = 0;
    a->current_animation = clip_index;
}
void destroy_clip(Clip*c){
 
    free(c->rectPositions);
    
    free(c);
}