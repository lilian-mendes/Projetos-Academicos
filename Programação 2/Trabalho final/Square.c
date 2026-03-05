#include <allegro5/allegro5.h> 
#include <stdlib.h>
#include "Square.h"

/* cria um novo jogador */
square* square_create(unsigned short x, unsigned short y) {
    square *new_square = (square*)malloc(sizeof(square));
    if(!new_square) 
        return NULL;
    
    new_square->x = x;
    new_square->y = y;
    new_square->width = 30;
    new_square->height = 50;
    new_square->state = STANDING;
    new_square->face = 1; 
    new_square->is_grounded = 1;
    new_square->is_dying = false;
    new_square->time_of_death = 0.0;
    new_square->vertical_speed = 0;

    new_square->health = 10;
    new_square->max_health = 10;
    new_square->stamina = 10;
    new_square->max_stamina = 10;
    new_square->special_cooldown = 0;
    new_square->special_ready = 1;

    new_square->control = joystick_create();
    new_square->gun = pistol_create();
    
    return new_square;
}

/* movimentacao (horizonal/andar) */
void square_move(square *element) {
    /* move para a esquerda com limite de tela */
    if (element->control->left) {
        if (element->x - SQUARE_STEP - element->width / 2 >= 0) 
            element->x -= SQUARE_STEP;
        else
            element->x = element->width / 2;
        /* vira para esquerda */
        element->face = 0; 
        if (element->state != JUMPING) 
            element->state = WALKING;
    }
    else if (element->control->right) {
        element->x += SQUARE_STEP;
        element->face = 1;
        if (element->state != JUMPING) 
            element->state = WALKING;
    }
    else if (element->state == WALKING)
        /* para de andar */
        element->state = STANDING; 
}

/* acao de pular */
void square_jump(square *element) {
    if(element->is_grounded) {
        element->vertical_speed = -JUMP_HEIGHT;
        element->is_grounded = 0;
        element->state = JUMPING;
    }
}

/* acao de agachar */
void square_duck(square *element, unsigned char is_ducking) {
    if(is_ducking && element->is_grounded) {
        element->state = DUCKING;
        element->height = 30; /* altura agachado */
    } else if(element->state == DUCKING) {
        element->state = STANDING;
        element->height = 50; /* altura normal */
    }
}

/* tiro simples */
void square_shoot(square *element) {
    if (element->gun->cooldown == 0 && element->stamina > 0) {
        element->state = SHOOTING;
        element->stamina--;

        /* calcula posicao do tiro */
        unsigned short bullet_x;
        if (element->face)  
            bullet_x = element->x + (element->width / 2);
        else 
            bullet_x = element->x - (element->width / 2); 
        unsigned short bullet_y = element->y - PLAYER_SHOOT_Y_OFFSET;
        
        pistol_shot(element->gun, bullet_x, bullet_y, element->face);
    }
}

/* tiro especial */
void square_special_shoot(square *element) {
    if (element->special_ready && element->stamina >= 5) {
        element->state = SHOOTING;
        element->stamina -= 5;
        element->special_ready = 0;
        element->special_cooldown = SPECIAL_SHOT_COOLDOWN;

        for (int i = -1; i <= 1; i++) {
            /* calcula posicao do tiro */
            unsigned short bullet_x;
            if (element->face)  
                bullet_x = element->x + (element->width / 2);
            else 
                bullet_x = element->x - (element->width / 2); 
            unsigned short bullet_y = element->y - PLAYER_SHOOT_Y_OFFSET;
            
            pistol_shot(element->gun, bullet_x, bullet_y, element->face);
            
            if(element->gun->shots) 
                element->gun->shots->type = 1; 
        }
    }
}

/* atualiza todas as balas */
void square_update_bullets(square *element) {
    bullet *curr = element->gun->shots;
    while (curr != NULL) {
        bullet_move(curr);
        curr = curr->next;
    }
}

/* atualizacao do player*/
void square_update(square *element) {
    /* atualiza arma e balas */
    pistol_update(element->gun);
    square_update_bullets(element);
    
    /* aplica gravidade */
    element->y += element->vertical_speed;
    element->vertical_speed += GRAVITY;
    
    /* verifica a colisao com o chao */
    if(element->y >= GROUND_LEVEL) {
        element->y = GROUND_LEVEL;
        element->vertical_speed = 0;
        element->is_grounded = 1;
        if(element->state == JUMPING) 
            element->state = STANDING;
    }
    
    /* reseta estado de tiro apos animacao */
    if(element->state == SHOOTING && element->gun->cooldown < PISTOL_COOLDOWN - 15)
         element->state = STANDING;

    /* regenera stamina (1 por segundo)*/
    static double last_regen_time = 0;
    double current_time = al_get_time();
    
    if (current_time - last_regen_time >= 1.0) {
        if (element->stamina < element->max_stamina)
            element->stamina++;
        last_regen_time = current_time;
    }

    /* atualiza cooldown do tiro especial */
    if (!element->special_ready && element->special_cooldown > 0) 
        element->special_cooldown--;
    else 
        element->special_ready = 1;
}

/* libera memoria do player */
void square_destroy(square *element) {
    if (!element) 
        return;
    
    pistol_destroy(element->gun);
    joystick_destroy(element->control);
    free(element);
}