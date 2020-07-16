#include "DEC2020.h"

double mx_find_angle(double x, double  y, double x1, double y1) {
    double angle;
    double katet1 = fabs(x - x1); // противоположный катет прямо-го тре-ка
    double katet2 = fabs(y - y1); // прилеглый катет прямо-го тре-ка
    double angle1;
    if (katet2) { // угол вектора героя к врагу в прямо-ном тре-ке
        angle1 = atan(katet1/katet2)/M_PI * 180;
    }
    else
            angle1 = 0;

    //printf("angle: %lf\n", angle1);
    if (x1 <= x &&  y1 <= y) {
        if (x1 == x &&  y1 != y)
            angle = 0;          // враг выше героя на одной линии оси Y
        else if (x1 != x &&  y1 == y)
            angle = 270;          // враг левее героя на одной линии оси X
        else
            angle = 360 - angle1;
    }
    else if (x1 <= x &&  y1 > y) {
        if (x1 == x)
            angle = 180;          // враг ниже героя на одной линии оси Y
        else
            angle = 180 + angle1;
    }
    else if (x1 >= x &&  y1 <= y) {
        if (x1 != x &&  y1 == y)
            angle = 90;   //враг правее героя на одной линии оси X
        else
            angle = angle1;

    }
    else if (x1 > x &&  y1 > y)
        angle = 180 - angle1;
    else
        return 0;

    return angle;
}

double mx_find_angle_tpos(t_position object, t_position hero) {
    return mx_find_angle(object.x, object.y, hero.x, hero.y);
}

double mx_rotate(double cur_angle, int direction, double rotate_speed, double time){
    double res = cur_angle + direction * rotate_speed * time;
    if (res > 360)
        res -= 360;
    else if (res < 0)
        res += 360;
    return res;
}

int mx_find_distance(t_position a, t_position b) {
    int k1 = abs(a.x - b.x);
    int k2 = abs(a.y - b.y);
    int hypotenuse = sqrt(k1 * k1 + k2 * k2);
    return hypotenuse;
}

void mx_set_enemy_dead(t_enemy *enemy) {
    enemy->dead = 1;
    enemy->step_time = 0;
    enemy->cur_sprite = 2;
}

void mx_bullet_hit(t_gamestate *gmst) {
    srand(time(0));
    t_bullet *bullets = gmst->player->cur_weapon->bullets_live;
    t_enemy *enemies = gmst->enemies;
    int buff, hit_dist;
    bool e_flag = true, b_flag = true;
    while (bullets) {
        enemies = gmst->enemies;
        while (enemies && bullets) {
            hit_dist = (bullets->size * 0.5 + enemies->size) * 0.5;

            if (!enemies->dead && (hit_dist > mx_find_distance(bullets->pos, enemies->pos))) {
                buff = enemies->cur_health;
                enemies->cur_health -= bullets->health;
                bullets->health -= buff;
                gmst->sounds->channel[BULLET_HIT] = Mix_PlayChannel(-1, gmst->sounds->track[BULLET_HIT], 0);
                
                if (enemies->cur_health <= 0) {
                    gmst->kill_count++;
                    //mx_spawn_corpse(gmst, enemies);
                    if(gmst->enemies == enemies) {
                        gmst->enemies = mx_del_enemy(gmst->enemies);
                        enemies = gmst->enemies;
                    }
                    else {
                        mx_set_enemy_dead(enemies);
                    }

                    int ch = rand() % 2;
                    gmst->sounds->channel[MONSTER_DEATH1 + ch] = Mix_PlayChannel(-1, gmst->sounds->track[MONSTER_DEATH1 + ch], 0);
                    e_flag = false;
                }
                if (bullets->health <= 0) {
                    if(gmst->player->cur_weapon->bullets_live == bullets) {
                        gmst->player->cur_weapon->bullets_live = mx_del_bullet(gmst->player->cur_weapon->bullets_live);
                        bullets = gmst->player->cur_weapon->bullets_live;
                    }
                    else {
                        bullets = mx_del_bullet(bullets);
                    }
                    b_flag = false;
                }
            }
            if (e_flag && enemies->next)
                enemies = enemies->next;
            else if(e_flag)
                enemies = NULL;
            e_flag = true;
        }
        if (b_flag && bullets->next)
            bullets = bullets->next;
        else if (b_flag)
            bullets = NULL;
        b_flag = true;
    }
}

void mx_enemy_hit(t_gamestate *gmst, unsigned int time_tick) {
    //write(2,"e_hit_start\n", 13);
    srand(time(0));
    t_player *player = gmst->player;
    t_enemy *enemies = gmst->enemies;
    int hit_dist;

    while (enemies) {
        hit_dist = player->size + enemies->size;
        hit_dist = (player->size + enemies->size);
        if(enemies->attack_time <= 0 && !enemies->dead) {

            if (hit_dist >= mx_find_distance(player->pos, enemies->pos)) {
                //printf("health : %d damaged : %d\n", player->cur_health, enemies->damage);
                player->cur_health -= enemies->damage;
                enemies->attack_time = enemies->attack_speed;
                int ch = rand() % 3;
                gmst->sounds->channel[MONSTER_HIT1 + ch] = Mix_PlayChannel(-1, gmst->sounds->track[MONSTER_HIT1 + ch], 0);
                if (player->cur_health < 0) {
                    player->death = true;
                gmst->sounds->channel[TANK_DEATH] = Mix_PlayChannel(-1, gmst->sounds->track[TANK_DEATH], 0);
                }
            }
        }
        else {
            if(enemies->attack_time > 0)
                enemies->attack_time -= time_tick;
        }
        if (enemies->next)
            enemies = enemies->next;
        else
            enemies = NULL;
    }
    //write(2,"e_hit_finish\n", 14);
}

void mx_mover(t_position *pos, double speed, int direction, unsigned int time) {
    if (direction < 0)
        speed *= REV_SPEED_MOD;
    double dist = direction*speed*time;
    if (pos->angle >= 0 && pos->angle <= 90) {
        if (pos->angle == 0)
            pos->p_y -= dist;
        else if (pos->angle == 90)
            pos->p_x += dist;
        else {
            pos->p_x += sinf(pos->angle * M_PI / 180) * dist;
            pos->p_y -= cosf(pos->angle * M_PI / 180) * dist;
        }
    }
    else if (pos->angle > 90 && pos->angle <= 180) {
        if (pos->angle == 180)
            pos->p_y += dist;
        else {
            pos->p_x += sinf(M_PI - pos->angle * M_PI / 180) * dist;
            pos->p_y += cosf(M_PI - pos->angle * M_PI / 180) * dist;
        }
    }
    else if (pos->angle > 180 && pos->angle <= 270) {
        if (pos->angle == 270)
            pos->p_x -= dist;
        else {
            pos->p_x -= sinf(pos->angle * M_PI / 180 - M_PI) * dist;
            pos->p_y += cosf(pos->angle * M_PI / 180 - M_PI) * dist;
        }
    }
    else if (pos->angle > 270 && pos->angle < 360) {
            pos->p_x -= sinf(2 * M_PI - pos->angle * M_PI / 180) * dist;
            pos->p_y -= cosf(2 * M_PI - pos->angle * M_PI / 180) * dist;
    }
    pos->x = round(pos->p_x);
    pos->y = round(pos->p_y);
}

void mx_player_move(t_gamestate *gmst, int direction, unsigned int time) {
    t_enemy *enemy = gmst->enemies;
    t_player *player = gmst->player;
    t_position cls = player->pos;
    t_position buff = cls;

    mx_mover(&cls, player->speed, direction, time);
    while (enemy) {
        //printf("minimal: %d actual: %d\n", player->size + enemy->size, mx_find_distance(cls, enemy->pos));
        if ((player->size + enemy->size)/1.5 > mx_find_distance(cls, enemy->pos))
            cls = buff;
        enemy = enemy->next;
    }
    if (cls.p_x < 0)
        cls.p_x = 0;
    else if (cls.p_x > W_WIDTH)
        cls.p_x = W_WIDTH;
    if (cls.p_y < 0)
        cls.p_y = 0;
    else if (cls.p_y > W_HEIGHT)
        cls.p_y = W_HEIGHT;
    player->pos = cls;
}

void mx_enemy_move(t_gamestate *gmst, unsigned int time_tick) {
    //write(2, "enemy_move_start\n",18);
    t_enemy *enemy = gmst->enemies;
    t_player *player = gmst->player;
    t_position cls, buff;
    srand(time(0));
    int check = rand() % 100, ch = rand() % 2;
    if (enemy && check > 80)
        gmst->sounds->channel[MONSTER_TALK1 + ch] = Mix_PlayChannel(-1, gmst->sounds->track[MONSTER_TALK1 + ch], 0);
    while (enemy) {
        enemy->step_time += time_tick;
        if (!enemy->dead){
            //write(2, "not_dead_enemy\n",15);
            if (enemy->step_time > LEG_TIME) {
                enemy->step_time -= LEG_TIME;
                enemy->cur_sprite = 1 - enemy->cur_sprite;
            }
            cls = enemy->pos;
            buff = cls;
            mx_mover(&cls, enemy->speed,1, time_tick);
            if (player->size+enemy->size <= mx_find_distance(cls, player->pos))
                enemy->pos = cls;
            enemy = enemy->next;
        }
        else {
            bool del_fl = true;
            //write(2, "dead_enemy\n",11);
            if (enemy->step_time > LEG_TIME*2) {
                enemy->cur_sprite++;
                enemy->step_time =0;
                if (enemy->cur_sprite >= 4){
                    //write(2, "init_del_enemy\n",15);
                    t_enemy *tmp = enemy->next;
                    mx_del_enemy(enemy);
                    enemy = tmp;
                    del_fl = false;
                }
            }
            if (del_fl) {
                enemy = enemy->next;
            }
        }
    //write(2, "enemy_move_finish\n",19);
    }
}

void mx_bullet_move(t_gamestate *gmst, unsigned int time) {
    //write(2, "b_move_start\n",13);
    t_bullet *bullet = gmst->player->cur_weapon->bullets_live;
    int speed = gmst->player->cur_weapon->bullet_speed;
    //write(2, "b_move_check1\n",14);
    while (bullet) {
        mx_mover(&bullet->pos, speed, 1, time);
        if(bullet->pos.x > W_WIDTH || bullet->pos.x < 0
            || bullet->pos.y > W_HEIGHT || bullet->pos.y < 0) {
                //write(2, "b_move_check2\n",14);
            if (gmst->player->cur_weapon->bullets_live == bullet) {
                //write(2, "b_move_check3\n",14);
                gmst->player->cur_weapon->bullets_live = mx_del_bullet(gmst->player->cur_weapon->bullets_live);
                bullet = gmst->player->cur_weapon->bullets_live;
            }
            else {
                bullet = mx_del_bullet(bullet);
                //write(2, "b_move_check5\n",14);
            }
        }
        if (bullet)
            bullet = bullet->next;
    }
    //write(2, "b_move_finish\n",14);
}
