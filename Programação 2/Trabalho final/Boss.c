#include <allegro5/allegro5.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include "Boss.h"
#include "Square.h"

/* constantes de teleporte (nivel 2) */
#define TELEPORT_COOLDOWN 3.0
#define TELEPORT_MIN_DIST 150
#define TELEPORT_MAX_DIST 400
#define TELEPORT_REAPPEAR_DELAY 0.5

/* cria um novo boss */
boss* boss_create(int x, int y, int nivel) {
    boss *b = malloc(sizeof(boss));
    if (!b) 
        return NULL;

    b->x = x;
    b->y = y;
    b->width = BOSS_WIDTH;
    b->height = BOSS_HEIGHT;
    b->face = 1; 
    b->vertical_speed = 0;
    b->active = 1;
    b->time_of_death = 0;
    b->level = nivel;

    /* configuracoes por nivel */
    if (nivel == 1)
        b->health = b->max_health = 15;
    else if (nivel == 2) {
        b->health = b->max_health = 25;
        b->is_visible = true;
        b->teleport_timer = al_get_time();
        b->became_visible_timer = al_get_time();
    }

    b->gun = pistol_create();
    b->control = joystick_create();
    
    return b;
}

/* movimentacao basica do boss (nivel 1) */
void boss_move(boss *b) {
    if (!b || !b->active) return;

    /* movimento horizontal */
    if (b->control->left) 
        b->x -= BOSS_SPEED;
    else if (b->control->right) 
        b->x += BOSS_SPEED;

    /* aplica gravidade */
    b->y += b->vertical_speed;
    b->vertical_speed += BOSS_GRAVITY;

    /* verifica colisao com o chao */
    if (b->y >= GROUND_LEVEL) {
        b->y = GROUND_LEVEL;
        b->vertical_speed = 0;
    }
}

void boss_update(boss *b, square *player) {
    if (!b || !b->active || b->health <= 0) {
        if(b->control) 
            joystick_reset(b->control);
        return;
    }

    /* boss do nivel 1: movimento e tiro simples */
    if (b->level == 1) {
        b->face = (player->x > b->x); /* vira para o player */
        joystick_reset(b->control);

        if (b->gun->cooldown <= 0) {
            float distancia_x = fabs(player->x - b->x);
            
            /* atira se estiver no alcance */
            if (distancia_x < BOSS_FIRE_RANGE) {
                int bx;
                if (b->face)
                    bx = b->x + (b->width / 2);
                else 
                    bx = b->x - (b->width / 2);
                int by = b->y - 80;
                pistol_shot(b->gun, bx, by, b->face);
                b->gun->cooldown = BOSS_COOLDOWN;
            } 
            /*  move em direcao ao player */
            else {
                if (b->face == 1) 
                    joystick_right(b->control, 1);
                else 
                    joystick_left(b->control, 1);
            }
        }
    }
    /* boss do nivel 1: movimento e tiro simples */
    else if (b->level == 2) {
        joystick_reset(b->control);

        /* logica do teleporte */
        if (!b->is_visible) {
            if (al_get_time() - b->teleport_timer > TELEPORT_REAPPEAR_DELAY) {
                b->is_visible = true;
                b->became_visible_timer = al_get_time();
            }
            return;
        }

        b->face = (player->x > b->x);

        /* teleporta apos o tempo de cooldown */
        if (al_get_time() - b->became_visible_timer > TELEPORT_COOLDOWN) {
            b->is_visible = false;
            b->teleport_timer = al_get_time();

            /* nova posicao aleatoria proxima ao jogador */
            int side;
            if (rand() % 2)
                side = 1;
            else 
                side = -1;
            int distance = TELEPORT_MIN_DIST + (rand() % (TELEPORT_MAX_DIST - TELEPORT_MIN_DIST));
            b->x = player->x + (side * distance);

            /* garante que nao saia da tela */
            if (b->x < player->x - SCREEN_WIDTH/2) b->x = player->x - SCREEN_WIDTH/2 + 100;
            if (b->x > player->x + SCREEN_WIDTH/2) b->x = player->x + SCREEN_WIDTH/2 - 100;
        }
        /* atira se a arma estiver pronta */
        else if (b->gun->cooldown <= 0) {
            int bx;
            if (b->face)
                bx = b->x + (b->width / 2);
            else 
                bx = b->x - (b->width / 2); 
            int by = b->y - 80;
            pistol_shot(b->gun, bx, by, b->face);
            b->gun->cooldown = BOSS_COOLDOWN;
        }
    }

    /* atualiza cooldown da arma */
    pistol_update(b->gun);
}

/* verifica colisao de bala (do player) com o boss */
int check_bullet_collision_boss(bullet *b, boss *boss) {
    if (!b || !boss || !boss->active || boss->health <= 0) 
        return 0;
    
    /* dimensoes da bala */
    int bullet_w = 10;
    int bullet_h = 5;

    if (b->x + bullet_w/2 > boss->x - boss->width/2 && b->x - bullet_w/2 < boss->x + boss->width/2 &&
        b->y + bullet_h/2 > boss->y - boss->height && b->y - bullet_h/2 < boss->y)
        return 1;
    return 0;
}

/* libera memoria do boss */
void boss_destroy(boss *b) {
    if (!b) 
        return;

    pistol_destroy(b->gun);
    joystick_destroy(b->control);
    free(b);
}