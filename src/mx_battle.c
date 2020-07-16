#include "DEC2020.h"

#define TANK_WIDTH 64
#define TANK_HEIGHT 64
#define TOWER_WIDTH 86
#define TOWER_HEIGHT 86


void mx_add_enemies(t_gamestate *game) {
    srand(time(0));
    //unsigned int n = 10 * (int) log(game->wave_number);
    uint n = 10 + 2 * game->wave_number++;
    //uint n = 1;
    for (unsigned int i = 0; i < n; i++) {

        t_position pos;
        int r = rand() % 50;
        if (r < 16) {
            pos.x = (r + 1) * W_WIDTH / 17;
            pos.y = -i * ENEMY_SIZE_1;
        } else if (r < 25) {
            pos.x = W_WIDTH + i * ENEMY_SIZE_1;
            pos.y = (r-15) * W_HEIGHT / 10;
        } else if (r < 41) {
            pos.x = (r - 24) * W_WIDTH / 17;
            pos.y = W_HEIGHT + i * ENEMY_SIZE_1;
        } else {
            pos.x = -i * ENEMY_SIZE_1;
            pos.y = (r - 40) * W_HEIGHT / 10;
        }
        mx_spawn_enemy(game, rand() % 3, pos);
    }

}

void mx_draw_enemies(SDL_Renderer *renderer, t_gamestate * game) {
    //write(2,"start_draw_enemy",16);
    t_enemy *enemy = game->enemies;

    while (enemy != NULL) {
        SDL_Rect rect = { enemy->pos.x - enemy->size / 2,
                          enemy->pos.y - enemy->size / 2,
                          enemy->size,
                          enemy->size
        };
        SDL_Texture *tex = mx_get_texture(game, enemy->sprites_deskr[enemy->cur_sprite]);
        if(!enemy->dead)
            enemy->pos.angle = mx_find_angle(enemy->pos.p_x,
                                            enemy->pos.p_y,
                                            game->player->pos.p_x,
                                            game->player->pos.p_y);

        SDL_RenderCopyEx(renderer,
            tex,
            NULL,
            &rect,
            enemy->pos.angle,
            NULL,
            SDL_FLIP_NONE
        );
        enemy = enemy->next;
    }
}

void mx_draw_bullets(SDL_Renderer *renderer, t_gamestate * game) {
    t_bullet *bullet = game->player->cur_weapon->bullets_live;

    while (bullet != NULL) {
        SDL_Rect rect = { bullet->pos.x - bullet->size / 2,
                          bullet->pos.y - bullet->size / 2,
                          bullet->size,
                          bullet->size
        };
        SDL_Texture *tex;
        switch (bullet->type) {
            default:
                tex = mx_get_texture(game, TEX_BULLET1);
                break;
        }

        SDL_RenderCopyEx(renderer,
            tex,
            NULL,
            &rect,
            bullet->pos.angle,
            NULL,
            SDL_FLIP_NONE
        );
        bullet = bullet->next;
    }
}

void mx_draw_battle_scene(SDL_Renderer *renderer, t_gamestate *game) {
    t_position mouse_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    SDL_Rect background_rect = {0, 0, W_WIDTH, W_HEIGHT};
    SDL_Rect body_rect = {game->player->pos.x - TANK_WIDTH/2,
                          game->player->pos.y - TANK_HEIGHT/2,
                          TANK_WIDTH,
                          TANK_HEIGHT
    };
    SDL_Rect tower_rect = {game->player->pos.x - TOWER_WIDTH/2,
                           game->player->pos.y - TOWER_HEIGHT/2,
                           TOWER_WIDTH,
                           TOWER_HEIGHT
    };
    game->player->tower_angle = mx_find_angle(game->player->pos.p_x,
                                      game->player->pos.p_y,
                                      mouse_pos.x,
                                      mouse_pos.y);

    SDL_RenderCopy(renderer, mx_get_texture(game, TEX_BACKGROUND_BATTLE), NULL, &background_rect);
    SDL_RenderCopyEx(renderer,
                mx_get_texture(game, (game->player->caterpillar? TEX_TANK3_1: TEX_TANK3_0)),
                NULL,
                &body_rect,
                game->player->pos.angle,
                NULL,
                SDL_FLIP_NONE
    );
    SDL_RenderCopyEx(renderer,
                mx_get_texture(game, TEX_TOWER3_3),
                NULL,
                &tower_rect,
                game->player->tower_angle,
                NULL,
                SDL_FLIP_NONE
    );
}

void mx_set_cat_sound(t_gamestate *game, int move){
    if (move) {
        if (game->sounds->pause[TANK_MOVE]) {
            Mix_Resume(game->sounds->channel[TANK_MOVE]);
        }
    }
    else if (!game->sounds->pause[TANK_MOVE]) {
        game->sounds->pause[TANK_MOVE] = 1;
        Mix_Pause(game->sounds->channel[TANK_MOVE]);
    }
}

int mx_battle(SDL_Renderer *renderer, t_gamestate *game) {
    unsigned int time = SDL_GetTicks() - game->last_time;
    SDL_Event event;
    int move = 0;
    int rotate = 0;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_W])
        move += 1;
    if (keystate[SDL_SCANCODE_S])
        move -= 1;
    if (keystate[SDL_SCANCODE_D])
        rotate += 1;
    if (keystate[SDL_SCANCODE_A])
        rotate -= 1;
    while (SDL_PollEvent(&event)) {
        if (SDL_QUIT == event.type)
            return 0;
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            mx_set_start_scene(game);
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            Mix_HaltChannel(game->sounds->channel[TANK_SHOT]);
            game->sounds->channel[TANK_SHOT] = Mix_PlayChannel(game->sounds->channel[TANK_SHOT], game->sounds->track[TANK_SHOT], 0);
            mx_spawn_bullet(game->player);
            //mx_add_enemies(game);
        }
    }
    if (move) {
        mx_player_move(game, move, time);
    }
    if (rotate)
        game->player->pos.angle = mx_rotate(game->player->pos.angle,
                                            rotate,
                                            0.1,
                                            time
        );
    if (move || rotate) {
        game->player->caterpillar_time += time;
        if (game->player->caterpillar_time > CAT_TIME){
            game->player->caterpillar = 1 - game->player->caterpillar;
            game->player->caterpillar_time = 0;
        }
    }
    mx_bullet_move(game, time);
    mx_enemy_move(game, time);
    mx_bullet_hit(game);
    mx_enemy_hit(game, time);
    game->enemies_spawn_time += time;
    if (game->enemies_spawn_time >= ENEMIES_SPAWN_SPEED){
        game->enemies_spawn_time -= ENEMIES_SPAWN_SPEED;
        mx_add_enemies(game);
    }

    mx_draw_battle_scene(renderer, game); //draw background, tank
    mx_draw_enemies(renderer, game);
    mx_draw_bullets(renderer, game);
    mx_set_cat_sound(game,move);

    if (game->player->death){
        mx_set_gameover_scene(game);
        mx_restart_gamestate(game);
    }
    game->last_time += time;
    return 1;
}
