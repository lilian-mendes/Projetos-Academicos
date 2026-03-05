#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Enemy.h"
#include "Square.h"

/* cria um novo inimigo basico com posicao e tipo especificos */
enemy* enemy_create(unsigned short x, unsigned short y, unsigned char type) {
    enemy *new_enemy = (enemy*)malloc(sizeof(enemy));
    if(!new_enemy) 
        return NULL;
    
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->width = ENEMY_WIDTH;
    new_enemy->height = ENEMY_HEIGHT;
    new_enemy->face = rand() % 2; /* direcao inicial aleatoria */
    new_enemy->vertical_speed = 0;
    new_enemy->health = 5;
    new_enemy->max_health = 5;
    new_enemy->type = type;
    new_enemy->is_dying = false;
    new_enemy->time_of_death = 0.0;

    /* cria componentes */
    new_enemy->control = joystick_create();
    new_enemy->gun = pistol_create();

    /* define movimento inicial baseado na direcao */
    if(new_enemy->face) 
        joystick_right(new_enemy->control, 1);
    else 
        joystick_left(new_enemy->control, 1);
    
    return new_enemy;
}

/* atualiza a posicao do inimigo */
void enemy_move(enemy *e) {

    if (e->control->left) {
        e->x -= ENEMY_SPEED;
        if (e->x < e->width / 2) {
            e->x = e->width / 2;
            joystick_left(e->control, 0);
            joystick_right(e->control, 1);
        }
    }
    else if (e->control->right)
        e->x += ENEMY_SPEED;

    /* aplica gravidade */
    e->y += e->vertical_speed;
    e->vertical_speed += GRAVITY;

    /* verifica colisao com o chao */
    if(e->y >= GROUND_LEVEL) {
        e->y = GROUND_LEVEL;
        e->vertical_speed = 0;
    }
}

void enemy_update(enemy *e, square *player) {
    if (e->health <= 0 || e->is_dying) {
        joystick_reset(e->control); 
        return;
    }

    /* sempre vira para o jogador */
    if (player->x < e->x) 
        e->face = 0;
    else 
        e->face = 1;
    joystick_reset(e->control);

    /* logica do tiro */
    if (e->gun->cooldown <= 0) {
        float distancia_x = fabs(player->x - e->x);
        
        /* atira se o jogador estiver perto */
        if (distancia_x < 400) {
            pistol_shot(e->gun, e->x, e->y - ENEMY_SHOOT_Y_OFFSET, e->face);
            e->gun->cooldown = ENEMY_PISTOL_COOLDOWN;
        } 
        /* anda em direcao ao jogador se estiver longe */
        else {
            if (e->face == 1) joystick_right(e->control, 1);
            else joystick_left(e->control, 1);
        }
    }

    /* atualiza cooldown da arma */
    pistol_update(e->gun);
    enemy_move(e);     
}

/* verifica colisao entre bala (do player) e inimigo */
int check_bullet_collision(bullet *b, enemy *e) {
    if (!b || !e || e->is_dying) 
        return 0;

    /* calcula hitbox do inimigo */
    float hitbox_center_y = e->y - ENEMY_SHOOT_Y_OFFSET;
    float hitbox_left = e->x - (HITBOX_WIDTH / 2);
    float hitbox_right = e->x + (HITBOX_WIDTH / 2);
    float hitbox_top = hitbox_center_y - (HITBOX_HEIGHT / 2);
    float hitbox_bottom = hitbox_center_y + (HITBOX_HEIGHT / 2);

    /* verifica se a bala esta no hitbox */
    if (b->x > hitbox_left && b->x < hitbox_right && b->y > hitbox_top && b->y < hitbox_bottom) {
        /* dano diferenciado para balas especiais */
        if (b->type == 1)
            e->health -= 3;
        else 
            e->health -= 1;
        /* garante que o inimigo nao vai ficar um saldo de vida negativa */ 
        if (e->health < 0) 
            e->health = 0;
        return 1;
    }
    return 0;
}

int check_player_bullet_collision(bullet *b, square *player) {
    if (!b || !player || player->health <= 0) 
        return 0;
    
    /* calcula a posicao do hitbox do jogador */
    float hitbox_center_y = player->y - PLAYER_SHOOT_Y_OFFSET;
    float player_hitbox_left = player->x - (HITBOX_WIDTH / 2);
    float player_hitbox_right = player->x + (HITBOX_WIDTH / 2);
    float player_hitbox_top = hitbox_center_y - (HITBOX_HEIGHT / 2);
    float player_hitbox_bottom = hitbox_center_y + (HITBOX_HEIGHT / 2);
    
    /* verifica se o ponto da bala (b->x, b->y) esta dentro dos limites do hitbox */
    if (b->x > player_hitbox_left && b->x < player_hitbox_right && b->y > player_hitbox_top && b->y < player_hitbox_bottom) 
        return 1;
    
    return 0;
}

/* libera memoria do inimigo */
void enemy_destroy(enemy *e) {
    if (!e) 
        return;

    pistol_destroy(e->gun);
    joystick_destroy(e->control);
    free(e);
}