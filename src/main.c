//non usare il tuo main 
#define SDL_MAIN_HANDLED //altrimenti caga il cazzo e c'hai er doppio entrypoint 
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.h"
int main() {

    Game* game = game_new(640,480, "1945");
    game_loop(game);
    game_end(game);
    return 0;
}
