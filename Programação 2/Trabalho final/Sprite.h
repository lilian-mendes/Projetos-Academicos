#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>

/* Ponteiros globais para as imagens/sprites do jogo */

/* sprites do jogo */
extern ALLEGRO_BITMAP *heart;
extern ALLEGRO_BITMAP *stamina_icon;

/* sprites do jogador */
extern ALLEGRO_BITMAP *sprite_standing;
extern ALLEGRO_BITMAP *sprite_walking[2];
extern ALLEGRO_BITMAP *sprite_crouching;
extern ALLEGRO_BITMAP *sprite_jumping[6];
extern ALLEGRO_BITMAP *sprite_shooting;
extern ALLEGRO_BITMAP *sprite_jump_shooting;
extern ALLEGRO_BITMAP *sprite_duck_shooting;
extern ALLEGRO_BITMAP *sprite_crouch_walking;
extern ALLEGRO_BITMAP *sprite_dying;
extern ALLEGRO_BITMAP *sprite_bullet;
extern ALLEGRO_BITMAP *special_bullet;

/* sprites do inimigo */
extern ALLEGRO_BITMAP *enemy_sprite_standing;
extern ALLEGRO_BITMAP *enemy_sprite_walking[2];
extern ALLEGRO_BITMAP *enemy_sprite_jumping[3];
extern ALLEGRO_BITMAP *enemy_sprite_shooting;
extern ALLEGRO_BITMAP *enemy_sprite_crouching;
extern ALLEGRO_BITMAP *enemy_sprite_dying[2];
extern ALLEGRO_BITMAP *sprite_bullet_enemy;

/* sprites do boss da fase 1 */
extern ALLEGRO_BITMAP *boss_bullet;
extern ALLEGRO_BITMAP *boss_dying;
extern ALLEGRO_BITMAP *boss_shooting;
extern ALLEGRO_BITMAP *boss_standing;
extern ALLEGRO_BITMAP *boss_walking[2];

/* sprites do boss da fase 2 */
extern ALLEGRO_BITMAP *boss_fase2_bullet;
extern ALLEGRO_BITMAP *boss_fase2_dying;
extern ALLEGRO_BITMAP *boss_fase2_shooting;
extern ALLEGRO_BITMAP *boss_fase2_standing;
extern ALLEGRO_BITMAP *boss_fase2_walking[2];

/* prototipos das funcoes */
int load_sprites();
void destroy_sprites();

#endif