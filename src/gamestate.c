#include "DEC2020.h"

t_gamestate *mx_restart_gamestate(t_gamestate *game){
    //t_enemy *enemy= game->enemies;
    while (game->enemies)
        game->enemies = mx_del_enemy(game->enemies);

    game->wave_number = 0;
    game->player->caterpillar = 0;
    game->player->caterpillar_time = 0.0;
    game->enemies_spawn_time = 0;
    game->player = mx_init_player();
    game->last_time = SDL_GetTicks();
    //printf("killcount : %d\n", game->kill_count);
    //game->kill_count = 0;
    game->input_name = false;
    return game;
}

t_gamestate *mx_init_gamestate(SDL_Renderer *renderer){
    t_gamestate *game = (t_gamestate *)malloc(sizeof(t_gamestate));
    game->last_time = SDL_GetTicks();
    game->textures = mx_init_textures(renderer);
    game->backgroundSound = NULL;
    game->player = mx_init_player();
    game->enemies = NULL;
    game->enemies_spawn_time = ENEMIES_SPAWN_SPEED - 3000;
    game->wave_number = 0;
    game->kill_count = 0;
    game->player->caterpillar = 0;
    game->player->caterpillar_time = 0.0;
    game->input_name = false;

    game->sounds = (t_ingameSound *)malloc(sizeof(t_ingameSound));
    game->sounds->track = (Mix_Chunk **)malloc(sizeof(Mix_Chunk *) * SOUNDS_AMOUNT);
    game->sounds->channel = (int *)malloc(sizeof(int) * SOUNDS_AMOUNT);
    game->sounds->pause = (bool *)malloc(sizeof(bool) * SOUNDS_AMOUNT);

    game->sounds->track[TANK_MOVE] = Mix_LoadWAV("resource/Sounds/tank_move.wav");
    game->sounds->track[TANK_SHOT] = Mix_LoadWAV("resource/Sounds/tank_shot.wav");
    game->sounds->track[TANK_DEATH] = Mix_LoadWAV("resource/Sounds/tank_death.wav");
    game->sounds->track[MONSTER_TALK1] = Mix_LoadWAV("resource/Sounds/monster_talk1.wav");
    game->sounds->track[MONSTER_TALK2] = Mix_LoadWAV("resource/Sounds/monster_talk2.wav");
    game->sounds->track[MONSTER_HIT1] = Mix_LoadWAV("resource/Sounds/monster_hit1.wav");
    game->sounds->track[MONSTER_HIT2] = Mix_LoadWAV("resource/Sounds/monster_hit2.wav");
    game->sounds->track[MONSTER_HIT3] = Mix_LoadWAV("resource/Sounds/monster_hit3.wav");
    game->sounds->track[MONSTER_DEATH1] = Mix_LoadWAV("resource/Sounds/monster_death1.wav");
    game->sounds->track[MONSTER_DEATH2] = Mix_LoadWAV("resource/Sounds/monster_death2.wav");
    game->sounds->track[BULLET_HIT] = Mix_LoadWAV("resource/Sounds/bullet_hit.wav");
    game->sounds->channel[TANK_SHOT] = Mix_PlayChannel(0, game->sounds->track[TANK_SHOT], -1);
    game->sounds->channel[TANK_MOVE] = Mix_PlayChannel(1, game->sounds->track[TANK_MOVE], -1);
    for (int i = 0; i < SOUNDS_AMOUNT; i++) {
        game->sounds->pause[i] = 1;
        //game->sounds->channel[i] = Mix_PlayChannel(-1, game->sounds->track[i], -1);
    }
    Mix_Pause(-1);

    mx_set_start_scene(game);
    return game;
}

void mx_free_gamestate(t_gamestate *game){
    mx_free_textures(game->textures);
    if (game->enemies)
        free(game->enemies);
    free(game);
    game = NULL;
}

void mx_set_start_scene(t_gamestate *game){
    game->scene = START_STATE;
    if (game->backgroundSound) {
        Mix_FreeMusic(game->backgroundSound);
        game->backgroundSound = NULL;
    }
    game->backgroundSound = Mix_LoadMUS("resource/Sounds/battleThemeA.mp3");
    Mix_PlayMusic(game->backgroundSound, -1);
}

void mx_set_battle_scene(t_gamestate *game){

    game->scene = GAME_STATE;
    if (game->backgroundSound) {
        Mix_FreeMusic(game->backgroundSound);
        game->backgroundSound = NULL;
    }
    game->backgroundSound = Mix_LoadMUS("resource/Sounds/Rise_of_spirit.mp3");
    Mix_PlayMusic(game->backgroundSound, -1);
}

void mx_set_gameover_scene(t_gamestate *game){

    game->scene = GAMEOVER_STATE;
    if (game->backgroundSound) {
        Mix_FreeMusic(game->backgroundSound);
        game->backgroundSound = NULL;
    }
    game->backgroundSound = Mix_LoadMUS("resource/Sounds/gameover.mp3");
    Mix_PlayMusic(game->backgroundSound, -1);

}
