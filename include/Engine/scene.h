#ifndef SCENE_H
#define SCENE_H
#include "game.h"
#include "managers.h"
#include "aiv-vector.h"
typedef struct{
    Game* game;
    aiv_vector* sceneObjects;
    UpdateManager* updMgr;
    DrawManager* drawMgr;
}scene;

scene* new_scene(void(*builder)(scene* s), Game* game);
void add_scene_object(scene* scene, GameObject * go);
void remove_scene_object(scene* scene, GameObject * go);

void destroy_scene(scene* scene);
#endif