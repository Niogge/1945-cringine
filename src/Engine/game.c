#include "game.h"
#include "component-factory.h"
#include "vec2.h"
#include "aiv-vector.h"
#include "scene.h"
#include "animator.h"
#include "rigidbody.h"
#include <stdlib.h>

Texture * textest;
void LoadMedia(Game* g){
    textest = load_texture(g->gfxMgr,"resources/player_plane.png", "player" ,65,65);
}
void scene1_ctor(scene* s){
    GameObject* go = new_gameobject(vec2_new(30.f,30.f), get_texture(s->game->gfxMgr,"player"));
    component* c = new_test_component();
    c->init(c);
    c->update(c,0.2f);
    attach_component(go,c);
    
    Animator* anim = new_animator();
    Clip* clip = new_clip(vec2_new(0,0), vec2_new(2,0), 65,65,30);
    add_clip(anim,clip);
    set_animator(go,anim);


    add_scene_object(s,go);



}
void scene2_ctor(scene* s){
    component* c = new_test_component();
    c->init(c);
    c->update(c,0.2f);
    GameObject* go = new_gameobject(vec2_new(40.f,40.f), get_texture(s->game->gfxMgr,"player"));
    attach_component(go,c);
    rigidbody* rbody = new_rb_with_collider(vec2_new(0,0),vec2_new(10,10));
    rbody->direction= vec2_new(1,0);
    rbody->speed = 10;
    attach_rigidbody(go, rbody);
    add_scene_object(s,go);
}
Game* game_new(int w, int h, const char * name){
    Game* g = (Game*) malloc(sizeof(Game));
    g->width = w;
    g->height = h;

    SDL_Init(SDL_INIT_VIDEO);
    g->window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,h,
        0
    );

    if(g->window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Could not create window: %s", SDL_GetError());
        return NULL;
    }

    g->renderer = SDL_CreateRenderer(g->window,-1,SDL_RENDERER_ACCELERATED);
    if(g->renderer == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Could not create renderer: %s", SDL_GetError());
        return NULL;
    }
    int flags=IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if((initted&flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        // handle error
    }
    g->gfxMgr = new_gfxmgr(g->renderer);
    input_manager = new_inputmgr();

    LoadMedia(g);
    g->scenes_vector = aiv_vector_new();
    scene * main_scene = new_scene(scene1_ctor,g);
    aiv_vector_add(g->scenes_vector,main_scene);
    scene * second_scene = new_scene(scene2_ctor,g);
    aiv_vector_add(g->scenes_vector,second_scene);
    g->scene_index = 0;



    return g;
}


void game_end(Game* game){
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void game_loop(Game* game){
    int current_count= SDL_GetPerformanceCounter();
    int last_count = 0;
    float timer = 0;
    int close = 1;
    float speed = 1.f;
    float timer2= 0.f;
    while(close){
        last_count = current_count;
        current_count = SDL_GetPerformanceCounter();
        int freq = SDL_GetPerformanceFrequency();
        float delta_time= (float)(current_count-last_count)/((float)freq);
        int fps = (int)(1.f/delta_time);

        SDL_Event event;
        while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){

                   close = 0;
                   break;
                }
           
        }

        timer += delta_time;
        if(timer >= 1){
            char * title[100];
            sprintf_s(title,sizeof(title), "Deltatime: %.6f - Fps: %d", delta_time, fps); 
            SDL_SetWindowTitle(game->window, title);
            timer-= 1.f;
        }

        SDL_RenderClear(game->renderer);
        // update(game->updMgr, delta_time);
        // draw(game->drawMgr);
        
        update_inputmgr(input_manager);


        update(((scene*)aiv_vector_at(game->scenes_vector, game->scene_index))->updMgr, delta_time);
        draw(((scene*)aiv_vector_at(game->scenes_vector, game->scene_index))->drawMgr);
        update_physicsmgr(((scene*)aiv_vector_at(game->scenes_vector, game->scene_index))->physMgr, delta_time);
        timer2+= delta_time;
        // if(timer2 >= 3){
        //     game->scene_index = game->scene_index ==1?0:1;
        //     timer2= 0.f;
        // }

        if(get_key(input_manager, SDL_SCANCODE_C)){
            game->scene_index = game->scene_index ==1?0:1;
            timer2= 0.f;
        }
        SDL_RenderPresent(game->renderer);
        

        // Uint8* state = SDL_GetKeyboardState(NULL);
        // if(state[SDL_SCANCODE_LEFT]){
        //     bmp_rect_frame.x += (-1.f*delta_time );
        // }
        // else if(state[SDL_SCANCODE_RIGHT]){
        //     bmp_rect_frame.x += (1.f*delta_time );
        // }
        // if(state[SDL_SCANCODE_UP]){
        //     bmp_rect_frame.y += (-1.f *delta_time );
        // }
        // else if(state[SDL_SCANCODE_DOWN]){
        //     bmp_rect_frame.y += (1.f *delta_time );
        // }
        // int mouse_x;
        // int mouse_y;
        // Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
        // if(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
        //     //do stuff
        // }
    }
}
