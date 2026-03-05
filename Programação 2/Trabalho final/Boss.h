#ifndef BOSS_H
#define BOSS_H

#include "Pistol.h"
#include "Joystick.h"
#include "Square.h"

/* constantes do boss */
#define BOSS_WIDTH 80
#define BOSS_HEIGHT 100
#define BOSS_JUMP_HEIGHT 12
#define BOSS_GRAVITY 1
#define BOSS_SPEED 3
#define BOSS_FIRE_RANGE 400
#define BOSS_COOLDOWN 90

/* estrutura do boss */
typedef struct {
    int x, y;                    /* posicao */
    int width, height;           /* dimensoes */
    int face;                    /* direcao (0=esquerda, 1=direita) */
    float vertical_speed;        /* velocidade */ 
    double time_of_death;        /* tempo de morte */ 
    int active;                  /* atividade */
    int health;                  /* vida atual */
    int max_health;              /* vida maxima */
   
    joystick *control;           /* controle */ 
    pistol *gun;                 /* arma */

    /* variaveis para diferenciar os boss de cada nivel */
    int level;                   /* nivel de dificuldade (1 ou 2) */
    double teleport_timer;       /* temporizador de teleporte (nivel 2) */
    double became_visible_timer; /* temporizador de visibilidade (nivel 2) */
    bool is_visible;             /* visibilidade (nivel 2)*/
} boss;

/* prototipos das funcoes */
boss* boss_create(int x, int y, int nivel);
void boss_update(boss *b, square *player);
int check_bullet_collision_boss(bullet *b, boss *boss);
void boss_destroy(boss *b);
void boss_move(boss *b);

#endif