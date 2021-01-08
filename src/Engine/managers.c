#include "managers.h"
#include <stdio.h>
#include <string.h>

static GraphicsManager* gfxmgr;
void init_gfxmgr(SDL_Renderer* renderTarget){
    gfxmgr = (GraphicsManager*) malloc(sizeof(GraphicsManager));
    gfxmgr->loadedTextures = aiv_dict_new();
    gfxmgr->target = renderTarget;

}
Texture* load_texture(const char* path, const char * texture_key , int w, int h){
    SDL_Surface* tempTexture = IMG_Load(path);
	if (tempTexture != NULL)
	{
		SDL_Texture * newTexture = SDL_CreateTextureFromSurface(gfxmgr->target, tempTexture);
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
            aiv_dict_put(gfxmgr->loadedTextures,key,(uint)strlen(key),tex);
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

Texture* get_texture(const char * texture_key ){
    char * key = texture_key;
    return (Texture*)aiv_dict_get(gfxmgr->loadedTextures,key, (uint)strlen(key));
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
    d->registered_objects_fore = aiv_vector_new();
    d->registered_objects_play = aiv_vector_new();
    d->registered_objects_back = aiv_vector_new();
    d->registered_objects_backest= aiv_vector_new();
    return d;
}

void register_drawmgr(DrawManager* d ,GameObject* go,enum drawLayers layer){
    switch(layer){
        case FORE:
            aiv_vector_add(d->registered_objects_fore, go);
            break;
        case PLAY:
            aiv_vector_add(d->registered_objects_play, go);
            break;
        case BACK:
            aiv_vector_add(d->registered_objects_back, go);
            break;
        case BACKEST:
            aiv_vector_add(d->registered_objects_backest, go);
            break;
        default:
            break;
    }
}


void unregister_drawmgr(DrawManager* d ,GameObject* go, enum drawLayers layer){
    aiv_vector* layer_vector = NULL;
    switch(layer){
        case FORE:
            layer_vector= d->registered_objects_fore;
            break;
        case PLAY:
            layer_vector= d->registered_objects_fore;
            break;
        case BACK:
            layer_vector= d->registered_objects_fore;
            break;
        case BACKEST:
            layer_vector= d->registered_objects_backest;
            break;
        default:
            return;
    }

    int index =-1;
    for (int i = 0; i < layer_vector->__count; i++)
    {
        if(layer_vector->__items[i] == go){
            index = i;
            break;
        }
    }
    if(index != -1){
        aiv_vector_remove_at(layer_vector,index);
    }
}
void INTERNAL_draw(DrawManager* d,aiv_vector * layer_vector){
    for (uint i = 0; i < layer_vector->__count; i++)
    {
        void* elem = layer_vector->__items[i];

        if(((GameObject*) elem)->active){

            Sprite* s = ((GameObject*) elem)->texture;
        
            SDL_RenderCopy(d->target,s->texture,s->renderFrame,s->renderQuad);
        }
    }
}
void draw(DrawManager* d){
    INTERNAL_draw(d, d->registered_objects_backest);
    INTERNAL_draw(d, d->registered_objects_back);
    INTERNAL_draw(d, d->registered_objects_play);
    INTERNAL_draw(d, d->registered_objects_fore);
    
}
void destroy_drawmgr(DrawManager* d ){
    aiv_vector_destroy(d->registered_objects_backest);
    aiv_vector_destroy(d->registered_objects_back);
    aiv_vector_destroy(d->registered_objects_play);
    aiv_vector_destroy(d->registered_objects_fore);
    free(d);
}


///////// INPUT MGR

static InputManager * _input_manager;
void init_inputmgr(){
    _input_manager = (InputManager*)malloc(sizeof(InputManager));
    _input_manager->_keyStates = NULL;
}
void update_inputmgr(){
    _input_manager->_keyStates= SDL_GetKeyboardState(0);
}
boolean get_key(SDL_Scancode key){
    if (_input_manager->_keyStates != 0)
	{
		return _input_manager->_keyStates[key];
	}
	return false;
}
void destroy_inputmgr(){
    free(_input_manager->_keyStates);
    free(_input_manager);
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
        if(elem->active){
            _rb_update(elem, delta_time);

        }


    }
    _collision_check(phy);
}
boolean boxbox_coll(GameObject* A, GameObject* B){
    int Ax = A->position.x - A->rb->coll->size.x * A->pivot.x;
    int Ay = A->position.y - A->rb->coll->size.y * A->pivot.y;
    int Aw = A->rb->coll->size.x;
    int Ah = A->rb->coll->size.y;


    int Bx = B->position.x - B->rb->coll->size.x * B->pivot.x;
    int By = B->position.y - B->rb->coll->size.y * B->pivot.y;
    int Bw = B->rb->coll->size.x;
    int Bh = B->rb->coll->size.y;


    if(Ax<Bx + Bw &&
        Ax+Aw > Bx &&
        Ay< By + Bh &&
        Ay+Ah>By){
            return true;
        }
    return false;
}
void _collision_check(PhysicsManager* phy){
    GameObject * go;
    for (uint j = 0; j < phy->registered_objects->__count; j++)
    {
        /* code */
        go = (GameObject*) phy->registered_objects->__items[j];
        if(go->active == false){
            continue;
        }
        for (uint i = j+1; i < phy->registered_objects->__count; i++)
        {
            GameObject* elem =(GameObject*) phy->registered_objects->__items[i];
            if(elem->active == false){
                continue;
            }
            if(elem == go){ continue;}
            if((elem->rb->mask_self & go->rb->collision_mask) >0 ){
                //TODO: Check collision;
                if(boxbox_coll(elem,go) || boxbox_coll(go, elem) ){
                    collision_handle(elem,go);
                    collision_handle(go,elem);
                }
            }
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

////////////////SCENE MANAGER
static SceneManager* scene_mgr;
init_scene_manager(){
    scene_mgr = (SceneManager*)malloc(sizeof(SceneManager));
    scene_mgr->scenes_vector = aiv_vector_new();
    scene_mgr->scene_index = 0;
}
void * current_scene(){
    return aiv_vector_at(scene_mgr->scenes_vector,scene_mgr->scene_index);
}
int register_scene(void * to_register){
    aiv_vector_add(scene_mgr->scenes_vector,to_register);
    return aiv_vector_size(scene_mgr->scenes_vector)-1;
}
void * get_scene_at(int i ){
    return aiv_vector_at(scene_mgr->scenes_vector,i);
}
int current_scene_index(){
    return scene_mgr->scene_index;
}
void go_to_scene(int i){
    scene_mgr->scene_index = i;
}
void destroy_scene_manager(){
    aiv_vector_destroy(scene_mgr->scenes_vector);
    free(scene_mgr);
}
