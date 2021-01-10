#include "game.h"
#include "scene.h"
#include "component-factory.h"
#include "vec2.h"
#include "aiv-vector.h"
#include "animator.h"
#include "rigidbody.h"
#include "managers.h"
#include <stdlib.h>
#include <stdio.h>

void LoadMedia(){
    load_texture("resources/player_plane.png", "player" ,65,65);
    load_texture("resources/bullet.png", "bullet" ,32,32);
    load_texture("resources/water.png", "water" ,188,192);
    load_texture("resources/enemy1_strip3.png", "Enemy_1" ,32,32);
    load_texture("resources/enemy2_strip3.png", "Enemy_2" ,32,32);
    load_texture("resources/enemy3_strip3.png", "Enemy_3" ,32,32);
    load_texture("resources/enemybullet1.png", "enemy_bullet" ,32,32);
    load_texture("resources/explosion.png", "explosion" ,65,65);
    load_texture("resources/bottom.png", "bottomUI" ,640,76);
    load_texture("resources/life.png", "life" ,32,32);
}
void scene1_ctor(scene* s){
    GameObject* go = new_gameobject(vec2_new(320.f,200.f), get_texture("player"));
    rigidbody* rbody = new_rb_with_collider(vec2_new(0,0),vec2_new(65,65));
    rbody->direction= vec2_new(1,0);
    rbody->speed = 0;
    rbody->collision_mask = ENEMY_MASK|BULLET_MASK;
    rbody->mask_self = PLAYER_MASK;
    attach_rigidbody(go,rbody);
    Animator* anim = new_animator();
    Clip* clip = new_clip(vec2_new(0,0), vec2_new(2,0), 65,65,3);
    add_clip(anim,clip);
    Clip* clip2 = new_clip(vec2_new(2,0), vec2_new(3,0), 65,65,3);
    add_clip(anim,clip2);
    set_animator(go,anim);
    component* player_movement = new_player_movement();
    attach_component(go,player_movement);
    player_movement->init(player_movement);

    component* shoot = new_player_shooting();
    attach_component(go, shoot);
    shoot->init_scene(shoot, s);

    add_scene_object(s,go);
    ///UI
    GameObject* ui = new_gameobject_layer(vec2_new(0,0), get_texture("bottomUI"), FORE);
    ui->pivot= vec2_new(0,0);
    ui->position = vec2_new(0,(float)(s->game->height - ui->texture->renderFrame->h));
    add_scene_object(s,ui);

    GameObject* lives = new_gameobject_no_texture(vec2_new(0,0));
    component* live_handler = new_lives_handler_component(player_movement,s);
    attach_component(lives, live_handler);
    add_scene_object(s, lives);
    

    //ENEMY_SPAWN
    GameObject* Enemy_Spawner = new_gameobject_no_texture(vec2_new(0,-100.f));
    component* spawn = new_enemy_spawner();
    attach_component(Enemy_Spawner,spawn);
    spawn->init_scene(spawn, s);
    add_scene_object(s,Enemy_Spawner);

    ///BACKGROUND STUFF
    GameObject* bg1 = new_gameobject_layer(vec2_new(0,0), get_texture("water"), BACKEST);
    bg1->pivot = vec2_new(0,0);
    bg1->texture->renderQuad->w = s->game->width;
    bg1->texture->renderQuad->h = s->game->height+10;
    component* FilippoMariaPerlini = new_background_component(vec2_new(0.f,(float)-s->game->height),vec2_new(0.f,(float)s->game->height),50.f);
    FilippoMariaPerlini->init(FilippoMariaPerlini);
    attach_component(bg1,FilippoMariaPerlini);
    add_scene_object(s,bg1);
    GameObject* bg2 = new_gameobject_layer(vec2_new(0,0), get_texture("water"), BACKEST);
    bg2->pivot = vec2_new(0,0);
    bg2->texture->renderQuad->w = s->game->width;
    bg2->texture->renderQuad->h = s->game->height+10;
    bg2->position.y = -s->game->height;
    component* EnricoMariaPerlini = new_background_component(vec2_new(0.f,(float)-s->game->height),vec2_new(0.f,(float)s->game->height),50.f);
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

    LoadMedia();
    scene * main_scene = new_scene(scene1_ctor,g);
    scene * second_scene = new_scene(scene2_ctor,g);

    register_scene(main_scene);
    register_scene(second_scene);


    return g;
}


void game_end(Game* game){
    destroy_inputmgr();
    printf("input mgr destroyed \n");
    destroy_scene_manager();
    printf("scene mgr destroyed \n"); 
    destroy_gfxmgr();
    printf("gfx mgr destroyed \n"); 
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    printf("image quit\n");

    SDL_DestroyRenderer(game->renderer);
    printf("render free \n");
    printf("window destroyed \n"); 
    SDL_Quit();
    printf("sdl closed \n"); 

    free(game);
    printf("game free\n ");
}

void game_loop(Game* game){
    int current_count= SDL_GetPerformanceCounter();
    int last_count = 0;
    float timer = 0;
    int close = 1;
    float speed = 1.f;
    float timer2= 0.f;
    SDL_Event* event = (SDL_Event*)malloc(sizeof(SDL_Event));
    while(close){
        last_count = current_count;
        current_count = SDL_GetPerformanceCounter();
        int freq = SDL_GetPerformanceFrequency();
        float delta_time= (float)(current_count-last_count)/((float)freq);
        int fps = (int)(1.f/delta_time);

        while(SDL_PollEvent(event)){
                if(event->type == SDL_QUIT){

                   close = 0;
                   break;
                }
           
        }

    /*    timer += delta_time;
        if(timer >= 1){
            char * title[100];
            sprintf_s(title,sizeof(title), "Deltatime: %.6f - Fps: %d", delta_time, fps); 
            SDL_SetWindowTitle(game->window, title);
            timer-= 1.f;
        }*/

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
        clean_up_scene(((scene*)current_scene()));
        
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
    free(event);

}
