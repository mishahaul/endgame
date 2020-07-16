#ifndef DEC2020_H
#define DEC2020_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "textures.h"

#define W_WIDTH 1600
#define W_HEIGHT 900

#define PLAYER_SPEED 0.2
#define PLAYER_HEALTH 5
#define REV_SPEED_MOD 0.75

#define ENEMY_SIZE_1 50
#define ENEMY_HEALTH_0 50
#define ENEMY_HEALTH_1 50
#define ENEMY_HEALTH_2 50
#define ENEMY_DAMAGE_0 5
#define ENEMY_DAMAGE_1 5
#define ENEMY_DAMAGE_2 5
#define ENEMY_ATTACK_SPEED_0 500
#define ENEMY_ATTACK_SPEED_1 700
#define ENEMY_ATTACK_SPEED_2 900
#define ENEMIES_SPEED_0 0.05
#define ENEMIES_SPEED_1 0.06
#define ENEMIES_SPEED_2 0.1
#define ENEMIES_SPAWN_SPEED 8000

#define CAT_TIME 80
#define LEG_TIME 100

#define BULLET_SIZE_1 15
#define BULLET_SPEED_1 1
#define BULLET_DAMAGE_1 25
#define SOUNDS_AMOUNT 11

#define MAX_NAME_LENGTH 16
#define NAME_NUMBER 10
#define LEADERBOARD_FILE_NAME "resource/leaderboard.txt"

enum e_scenestate {
    INTRO_STATE,
    START_STATE,
    GAME_STATE,
    GAMEOVER_STATE,
    LEADERBOARD_STATE
};

typedef struct s_player_stats {
    char names[NAME_NUMBER][MAX_NAME_LENGTH];
    int scores[NAME_NUMBER];
}               t_player_stats;

enum e_tracks {
    TANK_MOVE,
    TANK_SHOT,
    TANK_DEATH,
    MONSTER_TALK1,
    MONSTER_TALK2,
    MONSTER_HIT1,
    MONSTER_HIT2,
    MONSTER_HIT3,
    MONSTER_DEATH1,
    MONSTER_DEATH2,
    BULLET_HIT
};
typedef struct s_position {
    int x;
    int y;
    double p_x;
    double p_y;
    double angle;
}              t_position;

typedef struct s_bullet {
    t_position pos;
    int health;
    int type;
    int size;
    struct s_bullet *next;
    struct s_bullet *prev;
}              t_bullet;

typedef struct s_weapon {
    double firerate;
    int damage;
    int bullet_speed;
    int type;
    int bullets_count;
    t_bullet *bullets_live;
}               t_weapon;

typedef struct s_player {
    t_position pos;
    double tower_angle;
    int caterpillar;
    unsigned int caterpillar_time;
    int max_health;
    int cur_health;
    int size;
    double speed;
    int exp;
    bool death;
    t_weapon *cur_weapon;
}               t_player;

typedef struct s_enemy {
    t_position pos;
    int max_health;
    int cur_health;
    double speed;
    int damage;
    int attack_time;
    unsigned int attack_speed;
    int size;
    int type;
    unsigned int step_time;
    bool dead;
    enum e_textures sprites_deskr[11];
    uint cur_sprite;
    struct s_enemy *next;
    struct s_enemy *prev;
}               t_enemy;

typedef struct s_corpse {
    t_position pos;
    int type;
    int timer;
    int size;
    struct s_corpse *next;
    struct s_corpse *prev;
}               t_corpse;
typedef struct s_ingameSound {
    Mix_Chunk **track;
    int *channel;
    bool *pause;
}              t_ingameSound;
typedef struct s_gamestate {
    unsigned int last_time;
    enum e_scenestate scene;
    bool input_name;
    t_player *player;
    t_enemy *enemies;
    int kill_count;
    unsigned int enemies_spawn_time;
    unsigned int wave_number;
    t_corpse *corpses;
    t_ingameSound *sounds;
    SDL_Texture **textures;
    Mix_Music *backgroundSound;

}               t_gamestate;

t_gamestate *mx_init_gamestate(SDL_Renderer *renderer);
t_gamestate *mx_restart_gamestate(t_gamestate *game);
void mx_free_gamestate(t_gamestate *game);
void mx_set_start_scene(t_gamestate *game);
void mx_set_battle_scene(t_gamestate *game);
void mx_set_gameover_scene(t_gamestate *game);

int mx_playgame(SDL_Renderer *renderer, t_gamestate *game);
int mx_mainmenu(SDL_Renderer *renderer, t_gamestate *game);
int mx_gameover_menu(SDL_Renderer *renderer, t_gamestate *game);
int mx_battle(SDL_Renderer *renderer, t_gamestate *game);
void mx_leaderboard(SDL_Renderer *renderer, t_gamestate *game);

t_player_stats mx_read_leaderboeard_file(void);
void file_write(t_player_stats stats);
t_player_stats board_updated(t_player_stats stats, char new_name[MAX_NAME_LENGTH], int new_score);
//Spawning and initialization
void mx_spawn_corpse(t_gamestate *gmst, t_enemy *enemy);
void mx_spawn_enemy(t_gamestate *gmst, int type, t_position pos);
void mx_spawn_bullet(t_player *player);
t_weapon *mx_init_waepon(int type);
t_player *mx_init_player(void);
//Death and destruction
void mx_del_corpse(t_enemy *corpse);
t_enemy *mx_del_enemy(t_enemy *enemy);
void mx_del_all_enemy(t_enemy *enemy);
t_bullet *mx_del_bullet(t_bullet *bull);
void mx_del_weapon(t_weapon *weapon);
void mx_del_player(t_player *player);
//Geometry
double mx_find_angle(double x, double  y, double x1, double y1);
double mx_find_angle_tpos(t_position object, t_position hero);
double mx_rotate(double cur_angle, int direction, double rotate_speed, double time);

int mx_find_distance(t_position a, t_position b);
void mx_bullet_hit(t_gamestate *gmst);
void mx_enemy_hit(t_gamestate *gmst, unsigned int time_tick);

void mx_mover(t_position *pos, double speed, int direction, unsigned int time);
void mx_player_move(t_gamestate *gmst, int direction, unsigned int time);
void mx_enemy_move(t_gamestate *gmst, unsigned int time_tick);
void mx_bullet_move(t_gamestate *gmst, unsigned int time);

#endif
