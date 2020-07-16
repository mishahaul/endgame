#include "DEC2020.h"

// Initialization and spawn
void mx_spawn_corpse(t_gamestate *gmst, t_enemy *enemy) {
    t_corpse *new_corpse = (t_corpse *)malloc(sizeof(t_corpse));
    new_corpse->type = enemy->type;
    new_corpse->pos = enemy->pos;
    new_corpse->size = enemy->size;
    new_corpse->timer = 10;
    new_corpse->prev =  NULL;
    if (gmst->corpses == NULL)
        new_corpse->next = NULL;
    else {
        gmst->corpses->prev = new_corpse;
        new_corpse->next = gmst->corpses;
    }
    gmst->corpses = new_corpse;
    //write(2, "corpse spawned\n",15);
}

void mx_spawn_enemy(t_gamestate *gmst, int type, t_position pos) {
    t_enemy *new_enemy = (t_enemy *)malloc(sizeof(t_enemy));
    new_enemy->pos.x = pos.x;
    new_enemy->pos.y = pos.y;
    new_enemy->pos.p_x = pos.x;
    new_enemy->pos.p_y = pos.y;
    mx_rotate(mx_find_angle_tpos(gmst->player->pos, new_enemy->pos), 1, 0.5, 1);
    new_enemy->size = ENEMY_SIZE_1;
    new_enemy->dead = 0;
    new_enemy->attack_time = 0;

    new_enemy->type = type;
    switch (type) {
        case 0:
            new_enemy->speed = ENEMIES_SPEED_0;
            new_enemy->attack_speed = ENEMY_ATTACK_SPEED_0;
            new_enemy->damage = ENEMY_DAMAGE_0;
            new_enemy->max_health = ENEMY_HEALTH_0;
            new_enemy->sprites_deskr[0] = TEX_ENEMY0;
            new_enemy->sprites_deskr[1] = TEX_ENEMY0_1;
            new_enemy->sprites_deskr[2] = TEX_ENEMY0_DEATH1;
            new_enemy->sprites_deskr[3] = TEX_ENEMY0_DEATH2;
            new_enemy->sprites_deskr[4] = TEX_ENEMY0_DEATH3;
            new_enemy->sprites_deskr[5] = TEX_ENEMY0_DEATH4;
            new_enemy->sprites_deskr[6] = TEX_ENEMY0_DEATH5;
            new_enemy->sprites_deskr[7] = TEX_ENEMY0_DEATH6;
            new_enemy->sprites_deskr[8] = TEX_ENEMY0_DEATH7;
            new_enemy->sprites_deskr[9] = TEX_ENEMY0_DEATH8;
            new_enemy->sprites_deskr[10] = TEX_ENEMY0_DEATH9;
            break;
        case 1:
            new_enemy->speed = ENEMIES_SPEED_1;
            new_enemy->attack_speed = ENEMY_ATTACK_SPEED_1;
            new_enemy->damage = ENEMY_DAMAGE_1;
            new_enemy->max_health = ENEMY_HEALTH_1;
            new_enemy->sprites_deskr[0] = TEX_ENEMY1;
            new_enemy->sprites_deskr[1] = TEX_ENEMY1_1;
            new_enemy->sprites_deskr[2] = TEX_ENEMY1_DEATH1;
            new_enemy->sprites_deskr[3] = TEX_ENEMY1_DEATH2;
            new_enemy->sprites_deskr[4] = TEX_ENEMY1_DEATH3;
            new_enemy->sprites_deskr[5] = TEX_ENEMY1_DEATH4;
            new_enemy->sprites_deskr[6] = TEX_ENEMY1_DEATH5;
            new_enemy->sprites_deskr[7] = TEX_ENEMY1_DEATH6;
            new_enemy->sprites_deskr[8] = TEX_ENEMY1_DEATH7;
            new_enemy->sprites_deskr[9] = TEX_ENEMY1_DEATH8;
            new_enemy->sprites_deskr[10] = TEX_ENEMY1_DEATH9;
            break;
        case 2:
            new_enemy->speed = ENEMIES_SPEED_2;
            new_enemy->attack_speed = ENEMY_ATTACK_SPEED_2;
            new_enemy->damage = ENEMY_DAMAGE_2;
            new_enemy->max_health = ENEMY_HEALTH_2;
            new_enemy->sprites_deskr[0] = TEX_ENEMY2;
            new_enemy->sprites_deskr[1] = TEX_ENEMY2_1;
            new_enemy->sprites_deskr[2] = TEX_ENEMY2_DEATH1;
            new_enemy->sprites_deskr[3] = TEX_ENEMY2_DEATH2;
            new_enemy->sprites_deskr[4] = TEX_ENEMY2_DEATH3;
            new_enemy->sprites_deskr[5] = TEX_ENEMY2_DEATH4;
            new_enemy->sprites_deskr[6] = TEX_ENEMY2_DEATH5;
            new_enemy->sprites_deskr[7] = TEX_ENEMY2_DEATH6;
            new_enemy->sprites_deskr[8] = TEX_ENEMY2_DEATH7;
            new_enemy->sprites_deskr[9] = TEX_ENEMY2_DEATH8;
            new_enemy->sprites_deskr[10] = TEX_ENEMY2_DEATH9;
            break;
        default:
        break;
    }
    new_enemy->cur_health = new_enemy->max_health;
    new_enemy->cur_sprite = 0;
    new_enemy->prev = NULL;
    if (gmst->enemies == NULL)
        new_enemy->next = NULL;
    else {
        gmst->enemies->prev = new_enemy;
        new_enemy->next = gmst->enemies;
    }
    gmst->enemies = new_enemy;
}

void mx_spawn_bullet(t_player *player) {
    t_bullet *new_bull = (t_bullet *)malloc(sizeof(t_bullet));

    new_bull->pos = player->pos;
    new_bull->pos.angle = player->tower_angle;

    new_bull->health = player->cur_weapon->damage;
    new_bull->type = player->cur_weapon->type;
    new_bull->size = BULLET_SIZE_1;
    new_bull->prev = NULL;
    if(player->cur_weapon->bullets_live == NULL)
        new_bull->next = NULL;
    else {
        new_bull->next = player->cur_weapon->bullets_live;
        player->cur_weapon->bullets_live->prev = new_bull;
    }
    player->cur_weapon->bullets_live = new_bull;
    player->cur_weapon->bullets_count++;
    //write(2, "bullet spawned\n",15);
}

t_weapon *mx_init_waepon(int type) {
    if (type == 0) {
        t_weapon *weapon = (t_weapon *)malloc(sizeof(t_weapon));

        weapon->bullet_speed = BULLET_SPEED_1;
        weapon->damage = BULLET_DAMAGE_1;
        weapon->firerate = 1;
        weapon->bullets_count = 0;
        weapon->bullets_live = NULL;
        weapon->type = type;
        return weapon;
    }
    //write(2, "weapon initialized\n",19);
    return NULL;
}

t_player *mx_init_player(void) {
    t_player *res = (t_player *)malloc(sizeof(t_player));

    res->tower_angle = 0;
    res->pos.angle = 0;
    res->pos.x = W_WIDTH/2;
    res->pos.y = W_HEIGHT/2;
    res->pos.p_x = W_WIDTH/2;
    res->pos.p_y = W_HEIGHT/2;
    res->caterpillar = 0;
    res->max_health = PLAYER_HEALTH;
    res->cur_health = res->max_health;
    res->speed = PLAYER_SPEED;
    res->size = 10;
    res->exp = 0;
    res->cur_weapon = mx_init_waepon(0);
    res->death = 0;
    //write(2, "player initialized\n",19);
    return res;
}

//Death and destruction
void mx_del_corpse(t_enemy *corpse) {
    t_enemy *tmp = corpse;

    if (corpse) {
        if (corpse->next == NULL && corpse->prev == NULL) {
            free(tmp);
            corpse = NULL;
        }
        else if (corpse->next == NULL) {
            corpse->prev->next = NULL;
            corpse = corpse->prev;
            free(tmp);
        }
        else if (corpse->prev == NULL) {
            corpse = corpse->next;
            free(tmp);
        }
        else {
            corpse->prev->next = corpse->next;
            corpse->next->prev = corpse->prev;
            corpse = corpse->next;
            free(tmp);
        }
    }
}

void mx_del_all_enemy(t_enemy *enemy) {
    while (enemy){
        t_enemy* tmp = enemy->next;
        free(enemy);
        enemy = tmp;
    }
}

t_enemy *mx_del_enemy(t_enemy *enemy) {
    t_enemy *tmp = NULL;
    if (enemy) {
        if (enemy->next == NULL && enemy->prev == NULL) {
            //write(2, " 1$", 3);
            free(enemy);
            enemy = NULL;
        }
        else if (enemy->next == NULL) {
            //write(2, " 2$", 3);
            enemy->prev->next = NULL;
            tmp = enemy->prev;
            free(enemy);
            enemy = NULL;
        }
        else if (enemy->prev == NULL) {
            //write(2, " 3$", 3);
            enemy->next->prev = NULL;
            tmp = enemy->next;
            free(enemy);
            enemy = NULL;
        }
        else if (enemy->prev && enemy->next) {
            //write(2, " 4$", 3);
            enemy->prev->next = enemy->next;
            enemy->next->prev = enemy->prev;
            tmp = enemy->next;
            free(enemy);
            enemy = NULL;
        }
    }
    return tmp;
}

t_bullet *mx_del_bullet(t_bullet *bull) {
    t_bullet *tmp = NULL;
    if (bull) {
        //write(2, "bul_del_start\n", 15);
        if (bull->next == NULL && bull->prev == NULL) {
            //write(2, "bul_del_case00\n", 16);
            free(bull);
            bull = NULL;
        }
        else if (bull->next == NULL) {
            //write(2, "bul_del_case01\n", 16);
            tmp = bull->prev;
            //write(2, "bul_del_case01_01\n", 19);
            bull->prev->next = NULL;
            //write(2, "bul_del_case01_02\n", 19);
            free(bull);
            bull = NULL;
        }
        else if (bull->prev == NULL) {
            //write(2, "bul_del_case02\n", 16);
            bull->next->prev = NULL;
            tmp = bull->next;
            free(bull);
            bull = NULL;
        }
        else if (bull->prev && bull->next) {
            //write(2, "bul_del_case03\n", 16);
            bull->prev->next = bull->next;
            bull->next->prev = bull->prev;
            tmp = bull->next;
            free(bull);
            bull = NULL;
        }
    }
    return tmp;
}

void mx_del_weapon(t_weapon *weapon) {
    if (weapon) {
        while(weapon->bullets_live)
            mx_del_bullet(weapon->bullets_live);
        free(weapon);
    }
}

void mx_del_player(t_player *player) {
    if (player) {
        mx_del_weapon(player->cur_weapon);
        free(player);
    }
}
