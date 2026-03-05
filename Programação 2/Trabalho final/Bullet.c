#include <stdlib.h>
#include "Bullet.h"

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next) {
    /* verifica se a trajetoria eh valida (0 ou 1) */
    if (trajectory > 1) 
        return NULL;
    
    /* aloca memoria para a nova bala */
    bullet *new_bullet = (bullet*) malloc(sizeof(bullet));
    if (!new_bullet) 
        return NULL;
    
    /* inicializa os atributos da bala */
    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->trajectory = trajectory;
    new_bullet->next = (struct bullet*) next; 
    new_bullet->active = 1;                   
    new_bullet->type = 0;                    
    
    return new_bullet;
}

void bullet_move(bullet *element) {
    /* move a bala para esquerda ou direita conforme sua trajetoria */
    if (!element->trajectory) 
        element->x -= BULLET_MOVE;
    else 
        element->x += BULLET_MOVE;
}

void bullet_destroy(bullet *element) {
    if (!element) 
        return;
    
    /* marca como inativa antes de liberar a memoria */
    element->active = 0;
    
    free(element);
}