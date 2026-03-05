#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "Square.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Pistol.h"
#include "Sprite.h"
#include "Boss.h"
#include "Game.h"

/* variaveis globais do jogo */
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
int nivel_atual = 1;
int nivel_salvo = 1;
bool modo_fase_unica = false;

ALLEGRO_BITMAP *menu_background = NULL;
ALLEGRO_BITMAP *seleciona_fase_background = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *background_fase2 = NULL;
ALLEGRO_BITMAP *game_over_img = NULL;
ALLEGRO_BITMAP *game_win_img = NULL;
ALLEGRO_BITMAP *heal_sprite = NULL;

ALLEGRO_FONT *menu_font_large = NULL;
ALLEGRO_FONT *menu_font_medium = NULL;
ALLEGRO_FONT *menu_font_options = NULL;
ALLEGRO_FONT *menu_element = NULL;

float bg_scroll_speed = 0.5;

int main() {
    if (!inicializar_allegro())
        return -1;

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    if (!carregar_recursos_globais()) {
        destruir_recursos_globais();
        return -1;
    }

    nivel_salvo = carregar_progresso();

    EstadoDoJogo estado_atual = ESTADO_MENU;
    while (estado_atual != ESTADO_SAIR) {
        switch (estado_atual) {
            case ESTADO_MENU:
                EstadoDoJogo proximo_estado = executar_tela_menu(nivel_salvo);
                if (proximo_estado == ESTADO_JOGANDO) {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(menu_font_options, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER, "Fase 1");
                    al_flip_display();
                    al_rest(2.0);
                }
                
                estado_atual = proximo_estado;
                break;
            
            case ESTADO_CONTINUAR:
                char texto_fase[20];
                sprintf(texto_fase, "Fase %d", nivel_atual);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(menu_font_options, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER, texto_fase);
                al_flip_display();
                al_rest(2.0);
                estado_atual = ESTADO_JOGANDO;
                break;

            case ESTADO_SELECIONA_FASE:
                estado_atual = executar_tela_seleciona_fase();
                break;

            case ESTADO_JOGANDO:
                estado_atual = executar_jogo();
                break;

            case ESTADO_GAME_OVER:
                estado_atual = executar_tela_game_over();
                break;

            case ESTADO_VITORIA:
                if (modo_fase_unica || nivel_atual >= 2) 
                    estado_atual = ESTADO_VITORIA_FINAL;
                else 
                    estado_atual = executar_tela_fase_concluida();
                
                if (estado_atual == ESTADO_MENU) 
                    nivel_salvo = carregar_progresso();

                if (estado_atual == ESTADO_JOGANDO) {
                    char texto_fase[20];
                    sprintf(texto_fase, "Fase %d", nivel_atual);
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(menu_font_options, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER, texto_fase);
                    al_flip_display();
                    al_rest(2.0);
                }
                break;

            case ESTADO_VITORIA_FINAL:
                estado_atual = executar_tela_vitoria_final();
                break;

            case ESTADO_SAIR:
                break;
        }
    }

    destruir_recursos_globais();
    return 0;
}