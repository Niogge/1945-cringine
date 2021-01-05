#include "managers.h"
#include <stdio.h>
#include <string.h>
GraphicsManager* new_gfxmgr(SDL_Renderer* renderTarget){
    GraphicsManager * g = (GraphicsManager*) malloc(sizeof(GraphicsManager));
    g->loadedTextures = aiv_dict_new();
    g->target = renderTarget;
    return g; 
}
Texture* load_texture(GraphicsManager* gfxMgr,const char* path, const char * texture_key , int w, int h){
    SDL_Surface* tempTexture = IMG_Load(path);
	if (tempTexture != NULL)
	{
		SDL_Texture * newTexture = SDL_CreateTextureFromSurface(gfxMgr->target, tempTexture);
		Texture* tex = (Texture*) malloc(sizeof(Texture));
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", texture_key, SDL_GetError());
			return NULL;
		}
		else
		{
			tex->txt = newTexture;
			tex->h =h;
			tex->w = w;
            char* key = texture_key;
            aiv_dict_put(gfxMgr->loadedTextures,key,(uint)strlen(key),tex);
		}
		SDL_FreeSurface(tempTexture);
		return tex;
	}
	else
	{
		printf("Unable to load %s image %s! SDL_image Error: %s\n",texture_key, path, IMG_GetError());
		return NULL;
	}
}

Texture* get_texture(GraphicsManager* gfxMgr, const char * texture_key ){
    char * key = texture_key;
    return (Texture*)aiv_dict_get(gfxMgr->loadedTextures,key, (uint)strlen(key));
}

///////////////UPDMGR

UpdateManager* new_updmgr(){
    UpdateManager * u = (UpdateManager*)malloc(sizeof(UpdateManager));
    u->registered_objects = aiv_vector_new();
    return u;
}
void register_updmgr( UpdateManager * u,GameObject* go){
    aiv_vector_add(u->registered_objects,go);
}
void unregister_updmgr(UpdateManager * u,GameObject* go){
    int index =-1;
    for (int i = 0; i < u->registered_objects->__count; i++)
    {
        if(u->registered_objects->__items[i] == go){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(u->registered_objects,index);
    }
}

void update(UpdateManager* u, float delta_time){
    aiv_vector_foreach_dt(u->registered_objects, update_gameobject,delta_time);
   
}

void destroy_updmgr(UpdateManager * u){
    aiv_vector_destroy(u->registered_objects);
    free(u);
}

///////////////DRAWMGR
DrawManager* new_drawmgr(SDL_Renderer* renderer){
    DrawManager* d = (DrawManager*)malloc(sizeof(DrawManager));
    d->target = renderer;
    d->registered_objects = aiv_vector_new();
    return d;
}

void register_drawmgr(DrawManager* d ,GameObject* go){
    aiv_vector_add(d->registered_objects, go);
}


void unregister_drawmgr(DrawManager* d ,GameObject* go){
int index =-1;
    for (int i = 0; i < d->registered_objects->__count; i++)
    {
        if(d->registered_objects->__items[i] == go){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(d->registered_objects,index);
    }
}

void draw(DrawManager* d){
    for (uint i = 0; i < d->registered_objects->__count; i++)
    {
        void* elem = d->registered_objects->__items[i];

        Sprite* s = ((GameObject*) elem)->texture;
    
        SDL_RenderCopy(d->target,s->texture,s->renderFrame,s->renderQuad);
    }
    
}
void destroy_drawmgr(DrawManager* d ){
    aiv_vector_destroy(d->registered_objects);
    free(d);
}


///////// INPUT MGR
InputManager* new_inputmgr(){
    InputManager* i = (InputManager*)malloc(sizeof(InputManager));
    i->_keyStates = NULL;
}
void update_inputmgr(InputManager * mgr){
    mgr->_keyStates= SDL_GetKeyboardState(0);
}
boolean get_key(InputManager* mgr,SDL_Scancode key){
    if (mgr->_keyStates != 0)
	{
		return mgr->_keyStates[key];
	}
	return false;
}
void destroy_inputmgr(InputManager* mgr){
    free(mgr->_keyStates);
    free(mgr);
}


//////PHYSICS MGR
PhysicsManager* new_physicsmgr(){
    PhysicsManager* phy = (PhysicsManager*)malloc(sizeof(PhysicsManager));
    phy->registered_objects = aiv_vector_new();
    return phy;
}
void register_physicsmgr(PhysicsManager* phy ,GameObject* go){
    aiv_vector_add(phy->registered_objects, go);
}
void unregister_physicsmgr(PhysicsManager* phy ,GameObject* go){
    int index =-1;
    for (int i = 0; i < phy->registered_objects->__count; i++)
    {
        if(phy->registered_objects->__items[i] == go){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(phy->registered_objects,index);
    }
    
}
void update_physicsmgr(PhysicsManager* phy, float delta_time){
    for (uint i = 0; i < phy->registered_objects->__count; i++)
    {
        GameObject* elem =(GameObject*) phy->registered_objects->__items[i];
        _rb_update(elem, delta_time);
        _collision_check(phy, elem);


    }
}
void _collision_check(PhysicsManager* phy, GameObject* go){
    for (uint i = 0; i < phy->registered_objects->__count; i++)
    {
        GameObject* elem =(GameObject*) phy->registered_objects->__items[i];
        
        if(elem == go){ continue;}
        if((elem->rb->mask_self & go->rb->collision_mask) >0 ){
            //TODO: Check collision;
        }
    }
}
void _rb_update(GameObject* go, float delta_time){
    go->rb->pos.x = go->rb->pos.x + go->rb->direction.x*go->rb->speed*delta_time;
    go->rb->pos.y = go->rb->pos.y + go->rb->direction.y*go->rb->speed*delta_time;
    go->position = go->rb->pos;
}

void destroy_physicsmgr(PhysicsManager* phy ){
    aiv_vector_destroy(phy->registered_objects);
    free(phy);
}