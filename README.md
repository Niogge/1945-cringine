# 1945-cringine
This is a small game I made as christmas project.
The challenge here was writing a small engine in C using SDL.
Even if the actual game is not completed, and even if there are some kinda picturesque architectural decisions, it was a challenging project with interesting results.
<br>
<br>

... and I think with some minor refactoring it could be usable for any type of game. 
<br>
<br>

The core structure of the engine works around 2 key concepts: Managers and Components. 
## Managers 
Managers are divided in 2 subclasses: 
1. Static managers (input, gfx, audio, scene) 
2. Scene specific managers (Draw, Update, Physics) 
<br>

the first category are cross scene managers that usually contain loaded resources or stuff, like input, that is identical across the scenes. 
their functions are called in the main game functions (init, update, destroy), IF they have it (eg: the only one with update is the input manager).<br>
<br>
<br>
the second one, instead, contains usually a list of gameobjects (scroll down for more infos) and, depending on their purpose, they access a specific
function or data structure inside the GameObject.<br> their functions are called inside the main game function, depending on which scene is the active one.<br>
Right now this leads to an instant change between scenes that are all loaded at startup.
ATM (12/01) this structure is missing features that give control to the loading and unloading of the scene. this is planned, if not for this repo i will 
update this with where to find them. 
<br><br>
## Components
Components are defined like this.<br>
```c
struct _component{
    GameObject* owner;
    void(*init)(struct _component *);
    void(*init_scene)(struct _component *, scene* s );
    void(*destructor)(struct _component *);
    void(*update)(struct _component *,float);
    void(*onCollision)(struct _component *,GameObject* other);
    void* data;
};
```
And an example component implementation will look like this:
```c
void my_component_init(component* self){
    /***/
}
void my_component_init_scene(component* self, scene *s ){
    /***/
}

void my_component_update(component* self,float dt){
    /***/
}
void my_component_destructor(component* self){
    /***/
}
void my_component_on_collision(component* self, GameObject* other){
    /***/
}
component* new_my_component(){
    component* c = (component* )malloc(sizeof(component));
    c->init= my_component_init;
    c->init_scene= my_component_init_scene;
    c->update= my_component_update;
    my_component_data * data = (my_component_data*)malloc(sizeof(my_component_data));
    /*fill in data*/
    c->data = data;
    c->destructor = my_component_destructor;
    c->onCollision = my_component_on_collision;
    return c;
}
```
This is nice because you can build your own game around the concept of factories (more later).<br><br>
Just create my_comp.h and my_comp.c, fill in the functions and define your game logic, then include my_comp.h inside "component-factory.h" and Bing Bam Boom Checkmate. <br><br>
After a component is created you have to attach to a gameobject with its specific function.<br>
Components functions, except init and init_scene, are called by managers or gameobject it self when they are needed, no need to worry about them just write the game logic! :)
<br>
<br>
But how can you create, say, a bullet?<br?>
### Factory Example
Take my bullet factory as an example:
```c
GameObject* bullet_factory(scene * s){
    //create the gameobject with texture and a position, basic stuff
    GameObject* bullet_go = new_gameobject(vec2_new(180.f,180.f), get_texture( "bullet"));
    //add the rigidbody, so it has a collider
    rigidbody* bullet_rigidbody = new_rb_with_collider(bullet_go->position, vec2_new(15,15)); 
    //collision masks 
    bullet_rigidbody->collision_mask = ENEMY_MASK | PLAYER_MASK | BULLET_MASK;
    //the mask of the bullet
    bullet_rigidbody->mask_self = BULLET_MASK; 
    //then attach the rigidbody
    attach_rigidbody(bullet_go, bullet_rigidbody); 

    component * bullet_c = new_bullet_behaviour(vec2_new(0,-1),300); //create the bullet behaviour
    attach_component(bullet_go, bullet_c); //attach it to the bullet Go 
    bullet_c->init(bullet_c); //initialize the bullet component 
    
    // add the gameobject to the scene, this will add the gameobject to the various scene specific managers 
    // (eg: if it has no texture --> not added to drawmgr)
    add_scene_object(s, bullet_go); 
    
    //this is because i needed the bullet pointer right away
    return bullet_go;
}
```
and, for reference, let's take a look to the bullet_behaviour component.

```c
void bullet_behaviour_init(component * self){
    self->owner->rb->direction = ((bullet_behaviour_data*)self->data)->dir;
    self->owner->rb->speed = ((bullet_behaviour_data*)self->data)->speed;
}

void bullet_behaviour_update(component* self,float dt){
    bullet_behaviour_data* data = self->data;
    data->life -= dt;
    if(data->life <= 0){
        data->life= data->max_life;
        self->owner->active = false;
    }
}
void bullet_behaviour_destructor(component* self){
    free(self->data);
    free(self);
}

void bullet_behaviour_collision(component* self, GameObject* other){
    self->owner->active = false;
}

component* new_bullet_behaviour(vec2 direction, float speed){
    component* c = (component*)malloc(sizeof(component));
    c->init = bullet_behaviour_init;
    c->update = bullet_behaviour_update;
    bullet_behaviour_data* data= (bullet_behaviour_data*)malloc(sizeof(bullet_behaviour_data));
    data->dir = direction;
    data->speed = speed;
    data->life = 5;
    data->max_life = 5;
    c->data = data;
    c->destructor =bullet_behaviour_destructor;
    c->onCollision = bullet_behaviour_collision;

    return c;
}

```
as you can see, bullet_behaviour has no init_scene, this was long before the snippet.

### The Component Snippet
inside the .vscode folder there is the .code-snippets file. <br>
if you open up a new file and write hcomp and hit autocomplete, it will give you the H file for a component, with function names depending on filename (and include guards too!).<br>
there is also ccomp for the .c file.<br><br>
For example, if i'm writing a new component, say Ultimate_Big_Brain_Component.h( and .c), the .h will look like this after the hcomp snippet:
```c
#ifndef ULTIMATE_BIG_BRAIN_COMPONENT_H
#define ULTIMATE_BIG_BRAIN_COMPONENT_H
#include "gameObject.h"
#include "component-factory.h"
typedef struct{
    int a;
    int b;
    char* s;
} Ultimate_Big_Brain_Component_data;
void Ultimate_Big_Brain_Component_init(component* self);
void Ultimate_Big_Brain_Component_init_scene(component* self, scene *s);
void Ultimate_Big_Brain_Component_update(component* self,float dt);
void Ultimate_Big_Brain_Component_destructor(component* self);
void Ultimate_Big_Brain_Component_on_collision(component* self, GameObject* other);
component* new_Ultimate_Big_Brain_Component();
#endif
```
and the .c:
```c
#include "Ultimate_Big_Brain_Component.h"
#include "managers.h"
#include "scene.h"
#include <stdio.h>
#include <SDL.h>
void Ultimate_Big_Brain_Component_init(component* self){
}
void Ultimate_Big_Brain_Component_init_scene(component* self, scene *s ){
}
void Ultimate_Big_Brain_Component_update(component* self,float dt){
}
void Ultimate_Big_Brain_Component_destructor(component* self){
}
void Ultimate_Big_Brain_Component_on_collision(component* self, GameObject* other){
}
component* new_Ultimate_Big_Brain_Component(){
    component* c = (component* )malloc(sizeof(component));
    c->init= Ultimate_Big_Brain_Component_init;
    c->init_scene= Ultimate_Big_Brain_Component_init_scene;
    c->update= Ultimate_Big_Brain_Component_update;
    Ultimate_Big_Brain_Component_data * data = (Ultimate_Big_Brain_Component_data*)malloc(sizeof(Ultimate_Big_Brain_Component_data));
    /*fill in data*/
    c->data = data;
    c->destructor = Ultimate_Big_Brain_Component_destructor;
    c->onCollision = Ultimate_Big_Brain_Component_on_collision;
    return c;
}
```
just leave the function blank if you don't need it, or keep it whatever i'm not your supervisor. 
## Other major stuff
### aka: things I should address more deeply in the future. 
You can add your ost using audio manager, it has 3 functions, easy to use.<br>
Font is missing, first 2 sentences of the Afterword will explain why.<br>
A good scene management is missing, again, Afterword.<br>
Rigidbody is AABB rect only (dude, no rotation + lazyness = no SAT, there's a c++ repo of mine with SAT, you can find it here: https://github.com/Niogge/YetAnotherGLCrap ) <br> 
I cannot tell you how you should structure your game code, because it still need some decoupling, but for sure in the afterword i will address some updates that i'm probably going to integrate in future project based on this engine.

## Afterword
I mean, there's a leak in this code that causes crashes. <br>
I'm working on it at the time of writing (12/01).<br>
Anyway, IF and WHEN i fix it, will update this repo, even if i fix in a different project.<br>
Thanks to Bord VBN for sponsoring this repo.<br><br>
You are a special human being if you took the time to read this readme, thanks. xoxo. 
![lov u guys](https://1b-f.s3.eu-west-1.amazonaws.com/a/88336-3A6E877D-4609-4AC8-B837-6A73C8DA0A89-0-1501203773.gif)
