#include "scene.h"
scene* new_scene(void(*builder)(scene* s), Game* game){
    scene* s = (scene*) malloc(sizeof(scene)); 
    s->game = game;
    s->drawMgr = new_drawmgr(game->renderer);
    s->updMgr = new_updmgr();
    s->physMgr = new_physicsmgr();
    s->sceneObjects = aiv_vector_new();
    builder(s);
    return s;
}
void add_scene_object(scene* scene, GameObject * go){
    aiv_vector_add(scene->sceneObjects,go);
    register_updmgr(scene->updMgr,go);
    if(go->texture != NULL){
        register_drawmgr(scene->drawMgr, go);
    }
    if(go->rb != NULL){
        register_physicsmgr(scene->physMgr, go);
    }
    
}
void remove_scene_object(scene* scene, GameObject * go){
    int index =-1;
    for (int i = 0; i < scene->sceneObjects->__count; i++)
    {
        if(scene->sceneObjects->__items[i] == go){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(scene->sceneObjects,index);
    }

    unregister_updmgr(scene->updMgr,go);
    unregister_drawmgr(scene->drawMgr,go);
}
void destroy_scene(scene* scene){
    aiv_vector_foreach(scene->sceneObjects,destroy_gameobject);
    aiv_vector_destroy(scene->sceneObjects);
    destroy_updmgr(scene->updMgr);
    destroy_drawmgr(scene->drawMgr);
    destroy_physicsmgr(scene->physMgr);
}