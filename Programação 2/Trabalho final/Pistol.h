#ifndef __PISTOL__ 
#define __PISTOL__

#include "Bullet.h"

#define PISTOL_COOLDOWN 30 /* cooldown entre disparos */

/* estrutura de uma pistola */
typedef struct {
    unsigned char cooldown;  /* tempo restante de recarga */
    bullet *shots;           /* lista encadeada de balas ativas */
} pistol;

/* prototipos das funcoes */
pistol* pistol_create();
void pistol_shot(pistol *gun, unsigned short x, unsigned short y, unsigned char trajectory);
void pistol_update(pistol *gun);
void pistol_destroy(pistol *element);

#endif