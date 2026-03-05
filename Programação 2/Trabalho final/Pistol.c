#include <stdlib.h>
#include "Pistol.h"

pistol* pistol_create() {

    /* aloca memoria para uma nova pistola */
    pistol *new_pistol = (pistol*) malloc(sizeof(pistol));
    if (!new_pistol) 
        return NULL;
    
    /* inicializa estado da pistola */
    new_pistol->shots = NULL;   
    new_pistol->cooldown = 0;    

    return new_pistol;
}

void pistol_update(pistol *gun) {
    /* reduz o tempo de recarga, se necessario */
    if (gun->cooldown > 0) 
        gun->cooldown--;
}

void pistol_shot(pistol *gun, unsigned short x, unsigned short y, unsigned char trajectory) {
    /* nao atira se ainda estiver em recarga */
    if (gun->cooldown > 0) 
        return;
    
    /* cria nova bala na posicao/direcao especificada */
    bullet *new_bullet = bullet_create(x, y, trajectory, NULL);
    if (!new_bullet) 
        return;
    
    /* adiciona bala no inicio da lista de tiros */
    new_bullet->next = (struct bullet*) gun->shots;
    gun->shots = new_bullet;
    
    /* define tempo de recarga */
    gun->cooldown = PISTOL_COOLDOWN;
}

void pistol_destroy(pistol *element) {
    if (!element) 
        return;
    
    /* libera todas as balas ativas */
    bullet *current = element->shots;
    while (current != NULL) {
        bullet *next = (bullet*)current->next;
        bullet_destroy(current);
        current = next;
    }
    
    /* libera a pistola */
    free(element);
}