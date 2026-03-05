#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* constantes globais do jogo */
#define MAX_ENEMIES 6                /* numero maximo de inimigos */
#define ENEMY_SPAWN_DISTANCE 350     /* distancia que o jogador precisa percorrer para um novo inimigo aparecer */
#define ENEMIES_TO_KILL_FOR_BOSS 6   /* numero de inimigos a serem derrotados para o boss aparecer */
#define INITIAL_SPAWN_X 800          /* coordenada X inicial para o surgimento do primeiro inimigo */
#define FPS 60.0                     /* taxa de quadros por segundo */

/* estados do jogo */
typedef enum {
    ESTADO_MENU,             /* menu principal */
    ESTADO_CONTINUAR,        /* continua na fase em que o usuario passou */
    ESTADO_JOGANDO,          /* jogo em andamento */
    ESTADO_SELECIONA_FASE,   /* seleciona fase */
    ESTADO_GAME_OVER,        /* fim de jogo (derrota) */
    ESTADO_VITORIA,          /* fim de uma fase (vitoria) */
    ESTADO_VITORIA_FINAL,    /* fim de jogo (vitoria) */
    ESTADO_SAIR              /* sair do jogo */
} EstadoDoJogo;

/* variaveis globais */
extern ALLEGRO_DISPLAY *display;                  /* janela principal do jogo */
extern ALLEGRO_EVENT_QUEUE *event_queue;          /* fila de eventos (teclado, mouse, etc) */
extern ALLEGRO_BITMAP *menu_background;           /* background do menu */
extern ALLEGRO_BITMAP *seleciona_fase_background; /* background da tela de selecao de fase */
extern ALLEGRO_BITMAP *background;                /* background da fase 1 */
extern ALLEGRO_BITMAP *background_fase2;          /* background da fase 2 */
extern ALLEGRO_BITMAP *game_over_img;             /* background de game over */
extern ALLEGRO_BITMAP *game_win_img;              /* background de vitoria */
extern ALLEGRO_BITMAP *heal_sprite;               /* imagem do item de cura */
extern ALLEGRO_FONT *menu_font_large;             /* fonte grande para titulos */
extern ALLEGRO_FONT *menu_font_medium;            /* fonte media */
extern ALLEGRO_FONT *menu_font_options;           /* fonte para opcoes de menu */
extern ALLEGRO_FONT *menu_element;                /* fonte para o elemento de selecao */
extern float bg_scroll_speed;                     /* velocidade do background */
extern int nivel_atual;                           /* fase atual sendo jogada */
extern bool modo_fase_unica;                      /* flag para indicar se o jogador escolheu uma fase para jogar */


/* prototipos das funcoes */
EstadoDoJogo executar_tela_fase_concluida();
EstadoDoJogo executar_tela_menu();
EstadoDoJogo executar_tela_seleciona_fase();
EstadoDoJogo executar_tela_game_over();
EstadoDoJogo executar_tela_vitoria();
EstadoDoJogo executar_tela_vitoria_final(); 
EstadoDoJogo executar_jogo();
bool inicializar_allegro();
bool carregar_recursos_globais();
void destruir_recursos_globais();
int carregar_progresso();
void salvar_progresso(int proximo_nivel);
void reiniciar_jogo_sessao(square **player_ptr, enemy *enemies[], boss **final_boss_ptr, float *camera_x, int *enemies_killed_count, 
    int *active_enemies_count, float *next_enemy_spawn_x, int *boss_spawned, bool *heal_active, double *last_heal_spawn);
void destruir_sessao_jogo(square* player, enemy *enemies[], boss* final_boss);

#endif