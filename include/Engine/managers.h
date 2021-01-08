#ifndef MANAGERS_H
#define MANAGERS_H
#include "gameObject.h"
#include <SDL.h>
#include "aiv-dictionary.h"
#include "aiv-vector.h"
#include <SDL_image.h>
typedef struct{
    SDL_Renderer* target;
    aiv_dict* loadedTextures;

}GraphicsManager; 
void init_gfxmgr(SDL_Renderer* renderTarget);
Texture* load_texture( const char* path, const char* texture_key, int neww, int newh);
Texture* get_texture(const char* texture_key );

typedef struct{
    aiv_vector* registered_objects;
}UpdateManager; 
UpdateManager* new_updmgr();
void register_updmgr( UpdateManager * u,GameObject* go);
void unregister_updmgr( UpdateManager * u,GameObject* go);
void update(UpdateManager* u, float delta_time);
void destroy_updmgr(UpdateManager * u);

enum drawLayers{FORE,PLAY,BACK,BACKEST};
typedef struct{
    SDL_Renderer* target;
    aiv_vector* registered_objects_backest;
    aiv_vector* registered_objects_back;
    aiv_vector* registered_objects_play;
    aiv_vector* registered_objects_fore;
}DrawManager; 
DrawManager* new_drawmgr(SDL_Renderer* renderer);
void register_drawmgr(DrawManager* d ,GameObject* go,enum drawLayers layer);
void unregister_drawmgr(DrawManager* d ,GameObject* go,enum drawLayers layer);
void draw(DrawManager* d);
void destroy_drawmgr(DrawManager* d );




typedef struct{
    Uint8* _keyStates;
}InputManager;


new_inputmgr();
void update_inputmgr();
boolean get_key(SDL_Scancode key);
void destroy_inputmgr();


typedef struct{
    aiv_vector* registered_objects;
}PhysicsManager;
PhysicsManager* new_physicsmgr();
void register_physicsmgr(PhysicsManager* d ,GameObject* go);
void unregister_physicsmgr(PhysicsManager* d ,GameObject* go);
void update_physicsmgr(PhysicsManager* d,float delta_time);
void _collision_check(PhysicsManager* phy);
void _rb_update(GameObject* go, float delta_time);
void destroy_physicsmgr(PhysicsManager* d );



typedef struct{
    aiv_vector* scenes_vector;
    uint scene_index;
}SceneManager;
init_scene_manager();
void * current_scene();
void * get_scene_at(int i);
int register_scene(void * to_register);
int current_scene_index();
void go_to_scene(int i);
void destroy_scene_manager();

#endif