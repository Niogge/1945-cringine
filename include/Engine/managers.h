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
GraphicsManager* new_gfxmgr(SDL_Renderer* renderTarget);
Texture* load_texture(GraphicsManager* gfxMgr, const char* path, const char* texture_key, int neww, int newh);
Texture* get_texture(GraphicsManager* gfxMgr,const char* texture_key );

typedef struct{
    aiv_vector* registered_objects;
}UpdateManager; 
UpdateManager* new_updmgr();
void register_updmgr( UpdateManager * u,GameObject* go);
void unregister_updmgr( UpdateManager * u,GameObject* go);
void update(UpdateManager* u, float delta_time);
void destroy_updmgr(UpdateManager * u);


typedef struct{
    SDL_Renderer* target;
    aiv_vector* registered_objects;
}DrawManager; 
DrawManager* new_drawmgr(SDL_Renderer* renderer);
void register_drawmgr(DrawManager* d ,GameObject* go);
void unregister_drawmgr(DrawManager* d ,GameObject* go);
void draw(DrawManager* d);
void destroy_drawmgr(DrawManager* d );




typedef struct{
    Uint8* _keyStates;
}InputManager;

InputManager * input_manager;


InputManager* new_inputmgr();
void update_inputmgr(InputManager * mgr);
boolean get_key(InputManager* mgr,SDL_Scancode key);
void destroy_inputmgr(InputManager* mgr);


#endif