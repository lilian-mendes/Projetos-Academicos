#ifndef __ENEMY__
#define __ENEMY__

#include <stdbool.h>
#include "Joystick.h"
#include "Pistol.h"
#include "Square.h"

/* constantes do inimigo basico */
#define ENEMY_WIDTH 30          
#define ENEMY_HEIGHT 50         
#define ENEMY_SPEED 2
#define ENEMY_JUMP_CHANCE 0.01f
#define ENEMY_SHOOT_CHANCE 0.005f
#define ENEMY_CHANGE_DIR_CHANCE 0.02f 
#define ENEMY_SHOOT_Y_OFFSET 90
#define HITBOX_HEIGHT 40        
#define HITBOX_WIDTH 25         
#define ENEMY_PISTOL_COOLDOWN 90 

/* estrutura do inimigo basico */
typedef struct {
    unsigned short x, y;         /* posicao */
    unsigned char width, height; /* dimensoes */ 
    unsigned char face;          /* direcao (0=esquerda, 1=direita) */
    short vertical_speed;        /* velocidade */
    int health;                  /* vida atual */
    int max_health;              /* vida maxima */
    unsigned char type;          /* tipo de inimigo */
    double time_of_death;        /* tempo de morte */
    bool is_dying;               /* morrendo */

    joystick *control;           /* controle */
    pistol *gun;                 /* arma */
} enemy;

/* prototipos das funcoes */
enemy* enemy_create(unsigned short x, unsigned short y, unsigned char type);
void enemy_update(enemy *e, square *player);
void enemy_move(enemy *e);
void enemy_destroy(enemy *e);
int check_bullet_collision(bullet *b, enemy *e);
int check_player_bullet_collision(bullet *b, square *player);

#endif