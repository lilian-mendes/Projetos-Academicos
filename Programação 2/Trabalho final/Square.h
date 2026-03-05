#ifndef __SQUARE__
#define __SQUARE__

#include "Joystick.h"
#include "Pistol.h"

/* tamanho da tela */
#define SCREEN_WIDTH 847         
#define SCREEN_HEIGHT 478 

/* constantes do player */
#define SQUARE_STEP 3          
#define JUMP_HEIGHT 15          
#define GRAVITY 1                      
#define GROUND_LEVEL 350         
#define SPECIAL_SHOT_COOLDOWN 180
#define PLAYER_SHOOT_Y_OFFSET 90

/* estados do player */
typedef enum {
    STANDING,   
    WALKING,    
    JUMPING,   
    DUCKING,   
    SHOOTING,  
} CharacterState;

/* estrutura do player */
typedef struct {
    unsigned short x, y;            /* posicao */
    unsigned char width, height;    /* dimensoes */ 
    unsigned char face;             /* direcao (0=esquerda, 1=direita) */
    unsigned char is_grounded;      /* esta no chao */ 
    bool is_dying;                  /* morrendo */
    double time_of_death;           /* tempo de morte */ 
    short vertical_speed;           /* velocidade */ 
    unsigned char health;           /* vida atual */
    unsigned char max_health;       /* vida maxima */
    unsigned char stamina;          /* stamina atual */
    unsigned char max_stamina;      /* stamina maxima */
    unsigned char special_cooldown; /* cooldown do tiro especial */
    unsigned char special_ready;    /* tiro especial pronto */

    CharacterState state;           /* estado atual */ 
    joystick *control;              /* controle */ 
    pistol *gun;                    /* arma */
    
} square;

/* prototipos das funcoes */
square* square_create(unsigned short x, unsigned short y);
void square_move(square *element);
void square_jump(square *element);
void square_duck(square *element, unsigned char is_ducking);
void square_shoot(square *element);
void square_special_shoot(square *element);
void square_update_bullets(square *element);
void square_update(square *element);
void square_destroy(square *element);

#endif