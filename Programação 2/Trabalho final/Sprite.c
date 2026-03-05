#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#include "Sprite.h"

/* sprites do jogo */
ALLEGRO_BITMAP *heart = NULL;
ALLEGRO_BITMAP *stamina_icon = NULL;

/* sprites do player */
ALLEGRO_BITMAP *sprite_standing = NULL;
ALLEGRO_BITMAP *sprite_walking[2] = {NULL, NULL};
ALLEGRO_BITMAP *sprite_crouching = NULL;
ALLEGRO_BITMAP *sprite_jumping[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
ALLEGRO_BITMAP *sprite_shooting = NULL;
ALLEGRO_BITMAP *sprite_jump_shooting = NULL;
ALLEGRO_BITMAP *sprite_duck_shooting = NULL;
ALLEGRO_BITMAP *sprite_crouch_walking = NULL;
ALLEGRO_BITMAP *sprite_dying = NULL;
ALLEGRO_BITMAP *sprite_bullet = NULL;
ALLEGRO_BITMAP *special_bullet = NULL;

/* sprites do inimigo basico */
ALLEGRO_BITMAP *enemy_sprite_standing = NULL;
ALLEGRO_BITMAP *enemy_sprite_walking[2] = {NULL, NULL};
ALLEGRO_BITMAP *enemy_sprite_jumping[3] = {NULL, NULL, NULL};
ALLEGRO_BITMAP *enemy_sprite_shooting = NULL;
ALLEGRO_BITMAP *enemy_sprite_crouching = NULL;
ALLEGRO_BITMAP *enemy_sprite_dying[2] = {NULL, NULL};
ALLEGRO_BITMAP *sprite_bullet_enemy = NULL;

/* sprites do boss da fase 1 */
ALLEGRO_BITMAP *boss_bullet = NULL;
ALLEGRO_BITMAP *boss_dying = NULL;
ALLEGRO_BITMAP *boss_shooting = NULL;
ALLEGRO_BITMAP *boss_standing = NULL;
ALLEGRO_BITMAP *boss_walking[2] = {NULL, NULL};

/* sprites do boss da fase 2 */
ALLEGRO_BITMAP *boss_fase2_bullet = NULL;
ALLEGRO_BITMAP *boss_fase2_dying = NULL;
ALLEGRO_BITMAP *boss_fase2_shooting = NULL;
ALLEGRO_BITMAP *boss_fase2_standing = NULL;
ALLEGRO_BITMAP *boss_fase2_walking[2] = {NULL, NULL};

/* carrega todas as sprites */
int load_sprites() {
    /* carrega sprites do player */
    sprite_standing = al_load_bitmap("sprites/standing.png");
    sprite_walking[0] = al_load_bitmap("sprites/walking_1.png");
    sprite_walking[1] = al_load_bitmap("sprites/walking_2.png");
    sprite_crouching = al_load_bitmap("sprites/crouching.png");
    for (int i = 0; i < 6; i++) {
        char path[64];
        snprintf(path, sizeof(path), "sprites/jumping_%d.png", i + 1);
        sprite_jumping[i] = al_load_bitmap(path);
    }
    sprite_shooting = al_load_bitmap("sprites/shooting.png");
    sprite_jump_shooting = al_load_bitmap("sprites/jumping_and_shooting.png");
    sprite_duck_shooting = al_load_bitmap("sprites/crouching_and_shooting.png");
    sprite_crouch_walking = al_load_bitmap("sprites/crouch_walk.png");
    sprite_dying = al_load_bitmap("sprites/dying.png");
    sprite_bullet = al_load_bitmap("sprites/bullet.png");
    heart = al_load_bitmap("sprites/heart_icon.png");
    stamina_icon = al_load_bitmap("sprites/stamina_icon.png"); 
    special_bullet = al_load_bitmap("sprites/special.png");

    /* carrega sprites do inimigo basico */
    enemy_sprite_standing = al_load_bitmap("enemy_sprites/standing_enemy.png");
    enemy_sprite_walking[0] = al_load_bitmap("enemy_sprites/walking_1.png");
    enemy_sprite_walking[1] = al_load_bitmap("enemy_sprites/walking_2.png");
    enemy_sprite_jumping[0] = al_load_bitmap("enemy_sprites/jumping_1.png");
    enemy_sprite_jumping[1] = al_load_bitmap("enemy_sprites/jumping_2.png");
    enemy_sprite_jumping[2] = al_load_bitmap("enemy_sprites/jumping_3.png");
    enemy_sprite_shooting = al_load_bitmap("enemy_sprites/shooting.png");
    enemy_sprite_crouching = al_load_bitmap("enemy_sprites/crouching.png");
    enemy_sprite_dying[0] = al_load_bitmap("enemy_sprites/dying_1.png");
    enemy_sprite_dying[1] = al_load_bitmap("enemy_sprites/dying_2.png");
    sprite_bullet_enemy = al_load_bitmap("enemy_sprites/bullet.png");

    /* carrega sprites do boss da fase 1 */
    boss_bullet = al_load_bitmap("boss_sprites/boss_bullet.png");
    boss_dying = al_load_bitmap("boss_sprites/boss_dying.png");
    boss_shooting = al_load_bitmap("boss_sprites/boss_shooting.png");
    boss_standing = al_load_bitmap("boss_sprites/boss_standing.png");
    boss_walking[0] = al_load_bitmap("boss_sprites/boss_walking_1.png");
    boss_walking[1] = al_load_bitmap("boss_sprites/boss_walking_2.png");

    /* carrega sprites do boss da fase 2 */
    boss_fase2_bullet = al_load_bitmap("boss_sprites/boss_fase2_bullet.png");
    boss_fase2_dying = al_load_bitmap("boss_sprites/boss_fase2_dying.png");
    boss_fase2_shooting = al_load_bitmap("boss_sprites/boss_fase2_shooting.png");
    boss_fase2_standing = al_load_bitmap("boss_sprites/boss_fase2_standing.png");
    boss_fase2_walking[0] = al_load_bitmap("boss_sprites/boss_fase2_walking_1.png");
    boss_fase2_walking[1] = al_load_bitmap("boss_sprites/boss_fase2_walking_2.png");

    /*vVerifica se todas as sprites foram carregadas corretamente */
    if (!sprite_standing || !sprite_walking[0] || !sprite_walking[1] || !sprite_crouching ||
        !sprite_shooting || !sprite_jump_shooting || !sprite_duck_shooting || !sprite_crouch_walking || !sprite_dying ||
        !sprite_bullet || !heart || !stamina_icon ||!enemy_sprite_standing || !enemy_sprite_walking[0] || !special_bullet ||
        !enemy_sprite_walking[1] || !enemy_sprite_jumping[0] || !enemy_sprite_jumping[1] || !enemy_sprite_jumping[2] ||
        !enemy_sprite_shooting || !enemy_sprite_crouching || !enemy_sprite_dying[0] || !enemy_sprite_dying[1] || !sprite_bullet_enemy ||
        !boss_bullet || !boss_dying || !boss_shooting || !boss_standing || !boss_walking[0] || !boss_walking[1] ||
        !boss_fase2_bullet || !boss_fase2_dying || !boss_fase2_shooting || !boss_fase2_standing || !boss_fase2_walking[0] || !boss_fase2_walking[1]) {
        return 0;
    }
    
    return 1;
}

/* libera a memoria de todas as sprites */
void destroy_sprites() {
    /* libera as sprites do player */
    if (sprite_standing) al_destroy_bitmap(sprite_standing);
    if (sprite_walking[0]) al_destroy_bitmap(sprite_walking[0]);
    if (sprite_walking[1]) al_destroy_bitmap(sprite_walking[1]);
    if (sprite_crouching) al_destroy_bitmap(sprite_crouching);
    for (int i = 0; i < 6; i++) {
        if (sprite_jumping[i]) al_destroy_bitmap(sprite_jumping[i]);
    }
    if (sprite_shooting) al_destroy_bitmap(sprite_shooting);
    if (sprite_jump_shooting) al_destroy_bitmap(sprite_jump_shooting);
    if (sprite_duck_shooting) al_destroy_bitmap(sprite_duck_shooting);
    if (sprite_crouch_walking) al_destroy_bitmap(sprite_crouch_walking);
    if (sprite_dying) al_destroy_bitmap(sprite_dying);
    if (sprite_bullet) al_destroy_bitmap(sprite_bullet);
    if (heart) al_destroy_bitmap(heart);
    if (stamina_icon) al_destroy_bitmap(stamina_icon);
    if (special_bullet) al_destroy_bitmap(special_bullet);

    /* libera as sprites do inimigo basico */
    if (enemy_sprite_standing) al_destroy_bitmap(enemy_sprite_standing);
    if (enemy_sprite_walking[0]) al_destroy_bitmap(enemy_sprite_walking[0]);
    if (enemy_sprite_walking[1]) al_destroy_bitmap(enemy_sprite_walking[1]);
    if (enemy_sprite_jumping[0]) al_destroy_bitmap(enemy_sprite_jumping[0]);
    if (enemy_sprite_jumping[1]) al_destroy_bitmap(enemy_sprite_jumping[1]);
    if (enemy_sprite_jumping[2]) al_destroy_bitmap(enemy_sprite_jumping[2]);
    if (enemy_sprite_shooting) al_destroy_bitmap(enemy_sprite_shooting);
    if (enemy_sprite_crouching) al_destroy_bitmap(enemy_sprite_crouching);
    if (enemy_sprite_dying[0]) al_destroy_bitmap(enemy_sprite_dying[0]);
    if (enemy_sprite_dying[1]) al_destroy_bitmap(enemy_sprite_dying[1]);
    if (sprite_bullet_enemy) al_destroy_bitmap(sprite_bullet_enemy);

    /* libera sprites do boss da fase 1 */
    if (boss_bullet) al_destroy_bitmap(boss_bullet);
    if (boss_dying) al_destroy_bitmap(boss_dying);
    if (boss_shooting) al_destroy_bitmap(boss_shooting);
    if (boss_standing) al_destroy_bitmap(boss_standing);
    if (boss_walking[0]) al_destroy_bitmap(boss_walking[0]);
    if (boss_walking[1]) al_destroy_bitmap(boss_walking[1]);

    /* libera sprites do boss da fase 2 */
    if (boss_fase2_bullet) al_destroy_bitmap(boss_fase2_bullet);
    if (boss_fase2_dying) al_destroy_bitmap(boss_fase2_dying);
    if (boss_fase2_shooting) al_destroy_bitmap(boss_fase2_shooting);
    if (boss_fase2_standing) al_destroy_bitmap(boss_fase2_standing);
    if (boss_fase2_walking[0]) al_destroy_bitmap(boss_fase2_walking[0]);
    if (boss_fase2_walking[1]) al_destroy_bitmap(boss_fase2_walking[1]);
}