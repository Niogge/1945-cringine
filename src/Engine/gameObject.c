#include "gameObject.h"
GameObject* new_gameobject(vec2 pos, Texture* Texture){
    GameObject* go = (GameObject*)malloc(sizeof(GameObject));
    go->position = pos;
    go->components = aiv_vector_new();
    go->texture = new_sprite(Texture);
    go->animator = NULL;
    go->pivot = vec2_new(0.5,0.5);
    go->rb = NULL;
    go->active = true;
    return go;
}
GameObject* new_gameobject_no_texture(vec2 pos){
    GameObject* go = (GameObject*)malloc(sizeof(GameObject));
    go->position = pos;
    go->components = aiv_vector_new();
    go->texture = NULL;
    go->animator = NULL;
    go->rb = NULL;
    go->active = true;

    go->pivot = vec2_new(0.5,0.5);
    return go;
}

void set_animator(GameObject* go, Animator* a){
    go->animator = a;
}

void updater(void * elem, float delta_time){
    ((component*)elem)->update(elem, delta_time);
}
void collision_handle(GameObject* go, GameObject* other);

void update_gameobject(GameObject* go, float delta_time){
    if(go->active){
        if(go->texture != NULL){
            go->texture->renderQuad->x = go->position.x-go->pivot.x*go->texture->renderQuad->w;
            go->texture->renderQuad->y = go->position.y-go->pivot.y*go->texture->renderQuad->h;
            if(go->animator != NULL){
                vec2 v = update_animator(go->animator, delta_time);
                go->texture->renderFrame->x = v.x;
                go->texture->renderFrame->y = v.y;
            }
        }

    }
    aiv_vector_foreach_dt(go->components, updater, delta_time);
}

void collision_handle(GameObject* go, GameObject* other){
    for (int i = 0; i < go->components->__count; i++)
    {
        component * c = go->components->__items[i];
        if(c->onCollision != NULL){
            c->onCollision(c,other);
        }
    }
    
}


void attach_component(GameObject* go, component * comp){
    aiv_vector_add(go->components, (void*) comp);
    comp->owner = go;
}


void detach_component(GameObject* go, component* c){
    int index =-1;
    for (int i = 0; i < go->components->__count; i++)
    {
        if(go->components->__items[i] == c){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(go->components,index);
    }
    c->destructor(c);
}
void attach_rigidbody(GameObject* go, rigidbody * rb){
    go->rb = rb;
    rb->pos = go->position;
}
void component_mass_destroyer_heavy_metal_bravo_ultra_dragon(void* elem){
    ((component*) elem)->destructor(elem);
}
void destroy_gameobject(GameObject* go){
    
    aiv_vector_foreach(go->components, component_mass_destroyer_heavy_metal_bravo_ultra_dragon);
    
}
