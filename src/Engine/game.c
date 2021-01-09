#include "game.h"
#include "scene.h"
#include "component-factory.h"
#include "vec2.h"
#include "aiv-vector.h"
#include "animator.h"
#include "rigidbody.h"
#include <stdlib.h>

Texture * textest;
void LoadMedia(Game* g){
    textest = load_texture("resources/player_plane.png", "player" ,65,65);
    textest = load_texture("resources/bullet.png", "bullet" ,32,32);
    textest = load_texture("resources/water.png", "water" ,188,192);
}
void scene1_ctor(scene* s){
    GameObject* go = new_gameobject(vec2_new(30.f,30.f), get_texture("player"));
    rigidbody* rbody = new_rb_with_collider(vec2_new(0,0),vec2_new(65,65));
    rbody->direction= vec2_new(1,0);
    rbody->speed = 0;
    rbody->collision_mask = ENEMY_MASK|BULLET_MASK;
    rbody->mask_self = PLAYER_MASK;
    attach_rigidbody(go,rbody);
    Animator* anim = new_animator();
    Clip* clip = new_clip(vec2_new(0,0), vec2_new(2,0), 65,65,30);
    add_clip(anim,clip);
    set_animator(go,anim);

    component* c = new_player_movement();
    attach_component(go,c);
    c->init(c);

    component* shoot = new_player_shooting();
    attach_component(go, shoot);
    shoot->init_scene(shoot, s);

    add_scene_object(s,go);

    GameObject* go2 = new_gameobject_layer(vec2_new(180.f,30.f), get_texture("player"), BACK);
    rigidbody* rbody2 = new_rb_with_collider(vec2_new(0,0),vec2_new(65,65));
    rbody2->direction= vec2_new(1,0);
    rbody2->speed = 0;
    rbody2->collision_mask = PLAYER_MASK|BULLET_MASK;
    rbody2->mask_self = ENEMY_MASK;
    attach_rigidbody(go2, rbody2);
    Animator* anim2 = new_animator();
    Clip* clip2 = new_clip(vec2_new(0,0), vec2_new(2,0), 65,65,30);
    add_clip(anim2,clip2);
    set_animator(go2,anim2);

    add_scene_object(s,go2);

    GameObject* bullet_go = new_gameobject(vec2_new(180.f,180.f), get_texture( "bullet"));
    rigidbody* bullet_rigidbody = new_rb_with_collider(bullet_go->position, vec2_new(32,32));
    bullet_rigidbody->direction = vec2_new(0,0);
    bullet_rigidbody->speed=0;
    bullet_rigidbody->collision_mask = ENEMY_MASK | PLAYER_MASK;
    bullet_rigidbody->mask_self = BULLET_MASK;
    attach_rigidbody(bullet_go, bullet_rigidbody);

    component * bullet_c = new_bullet_behaviour();
    attach_component(bullet_go, bullet_c);
    bullet_c->init(bullet_c);

    add_scene_object(s, bullet_go);



    ///BACKGROUND STUFF
    GameObject* bg1 = new_gameobject_layer(vec2_new(0,0), get_texture("water"), BACKEST);
    bg1->pivot = vec2_new(0,0);
    bg1->texture->renderQuad->w = s->game->width;
    bg1->texture->renderQuad->h = s->game->height+10;
    component* FilippoMariaPerlini = new_background_component(vec2_new(0.f,-s->game->height),vec2_new(0.f,s->game->height),50.f);
    FilippoMariaPerlini->init(FilippoMariaPerlini);
    attach_component(bg1,FilippoMariaPerlini);
    add_scene_object(s,bg1);
    GameObject* bg2 = new_gameobject_layer(vec2_new(0,0), get_texture("water"), BACKEST);
    bg2->pivot = vec2_new(0,0);
    bg2->texture->renderQuad->w = s->game->width;
    bg2->texture->renderQuad->h = s->game->height+10;
    bg2->position.y = -s->game->height;
    component* EnricoMariaPerlini = new_background_component(vec2_new(0.f,-s->game->height),vec2_new(0.f,s->game->height),50.f);
    EnricoMariaPerlini->init(EnricoMariaPerlini);
    attach_component(bg2,EnricoMariaPerlini);
    add_scene_object(s,bg2);
    //go2->active = false;

}
void scene2_ctor(scene* s){
    component* c = new_test_component();
    GameObject* go = new_gameobject(vec2_new(40.f,40.f), get_texture("player"));
    attach_component(go,c);
    c->init(c);
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
    init_gfxmgr(g->renderer);
    init_inputmgr();
    init_scene_manager();

    LoadMedia(g);
    scene * main_scene = new_scene(scene1_ctor,g);
    scene * second_scene = new_scene(scene2_ctor,g);

    register_scene(main_scene);
    register_scene(second_scene);


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
        
        update_inputmgr();


        update(((scene*)current_scene())->updMgr, delta_time);
        draw(((scene*)current_scene())->drawMgr);
        update_physicsmgr(((scene*)current_scene())->physMgr, delta_time);
        timer2+= delta_time;
        // if(timer2 >= 3){
        //     game->scene_index = game->scene_index ==1?0:1;
        //     timer2= 0.f;
        // }

        if(get_key(SDL_SCANCODE_C) && timer2>0.5f){
            go_to_scene(current_scene_index()==1?0:1);
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
