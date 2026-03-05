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

/* salva o proximo nivel a ser jogado em um arquivo */
void salvar_progresso(int proximo_nivel) {
    FILE* arquivo = fopen("progresso.txt", "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Nao foi possivel criar o arquivo de salvamento.\n");
        return;
    }
    fprintf(arquivo, "%d", proximo_nivel);
    fclose(arquivo);
}

/* carrega o progresso do arquivo, retorna 1 se nenhum save for encontrado */
int carregar_progresso() {
    FILE* arquivo = fopen("progresso.txt", "r");
    if (arquivo == NULL) 
        return 1;
    int nivel_salvo = 1;
    if (fscanf(arquivo, "%d", &nivel_salvo) != 1) 
        nivel_salvo = 1;
    fclose(arquivo);
    return nivel_salvo;
}

/* gerencia a tela de menu principal */
EstadoDoJogo executar_tela_menu(int nivel_salvo) {
    int opcao_selecionada = 0;
    const int TOTAL_OPCOES = 4;
    const char* opcoes_texto[] = {"Jogar", "Continuar", "Fases", "Sair"};

    bool pode_continuar = (nivel_salvo > 1);

    while (true) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return ESTADO_SAIR;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = (opcao_selecionada + 1) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_W:
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = (opcao_selecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if (opcao_selecionada == 0) { /* jogar */
                            nivel_atual = 1;
                            return ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 1) { /* continuar */
                            if (pode_continuar) {
                                nivel_atual = nivel_salvo;
                                return ESTADO_CONTINUAR;
                            }
                        }
                        if (opcao_selecionada == 2) { /* seleciona fase */
                            return ESTADO_SELECIONA_FASE;
                        }
                        if (opcao_selecionada == 3) { /* sair */
                            return ESTADO_SAIR;
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return ESTADO_SAIR;
                }
            }
        }

        al_draw_bitmap(menu_background, 0, 0, 0);

        int right_x = SCREEN_WIDTH - 40;
        ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 204, 0);
        ALLEGRO_COLOR cor_desativada = al_map_rgb(100, 100, 100);

        int y_posicoes[] = {
            SCREEN_HEIGHT / 2 + 40, SCREEN_HEIGHT / 2 + 85,
            SCREEN_HEIGHT / 2 + 130, SCREEN_HEIGHT / 2 + 175
        };

        for (int i = 0; i < TOTAL_OPCOES; i++) {
            ALLEGRO_COLOR cor_atual = (opcao_selecionada == i) ? cor_selecionada : cor_normal;

            if (i == 1 && !pode_continuar) {
                cor_atual = cor_desativada;
            }

            al_draw_text(menu_font_options, cor_atual, right_x, y_posicoes[i], ALLEGRO_ALIGN_RIGHT, opcoes_texto[i]);

            if (opcao_selecionada == i && !(i == 1 && !pode_continuar)) {
                int largura_texto = al_get_text_width(menu_font_options, opcoes_texto[i]);
                int padding = 15;
                float x_o_esquerdo = right_x - largura_texto - padding;
                float x_o_direito = right_x + padding;
                al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, y_posicoes[i], ALLEGRO_ALIGN_RIGHT, "o");
                al_draw_text(menu_element, cor_selecionada, x_o_direito, y_posicoes[i], ALLEGRO_ALIGN_LEFT, "o");
            }
        }
        al_flip_display();
        al_rest(1.0 / FPS);
    }
}


/* gerencia a tela de selecao de fase */
EstadoDoJogo executar_tela_seleciona_fase() {
    int opcao_selecionada = 0;
    const int TOTAL_OPCOES = 3;
    const char* opcoes_texto[] = {"Fase 1", "Fase 2", "Voltar"};

    while (true) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return ESTADO_SAIR;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = (opcao_selecionada + 1) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_W:
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = (opcao_selecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        modo_fase_unica = true;
                        if (opcao_selecionada == 0) {
                            nivel_atual = 1;
                            return ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 1) {
                            nivel_atual = 2;
                            return ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 2) {
                            modo_fase_unica = false;
                            return ESTADO_MENU;
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return ESTADO_MENU;
                }
            }
        }

        al_draw_bitmap(seleciona_fase_background, 0, 0, 0);

        ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 204, 0);

        int pos_x[] = {550, 550, SCREEN_WIDTH - 35};
        int pos_y[] = {175, 380, SCREEN_HEIGHT - 50};
        int aligns[] = {ALLEGRO_ALIGN_LEFT, ALLEGRO_ALIGN_LEFT, ALLEGRO_ALIGN_RIGHT};

        for (int i = 0; i < TOTAL_OPCOES; i++) {
            ALLEGRO_COLOR cor_atual = (opcao_selecionada == i) ? cor_selecionada : cor_normal;
            al_draw_text(menu_font_options, cor_atual, pos_x[i], pos_y[i], aligns[i], opcoes_texto[i]);

            if (opcao_selecionada == i) {
                int largura_texto = al_get_text_width(menu_font_options, opcoes_texto[i]);
                int padding = 15;
                float x_o_esquerdo, x_o_direito;

                if (aligns[i] == ALLEGRO_ALIGN_LEFT) {
                    x_o_esquerdo = pos_x[i] - padding;
                    x_o_direito = pos_x[i] + largura_texto + padding;
                    al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, pos_y[i], ALLEGRO_ALIGN_RIGHT, "o");
                    al_draw_text(menu_element, cor_selecionada, x_o_direito, pos_y[i], ALLEGRO_ALIGN_LEFT, "o");
                }
                else {
                    x_o_esquerdo = pos_x[i] - largura_texto - padding;
                    x_o_direito = pos_x[i] + padding;
                    al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, pos_y[i], ALLEGRO_ALIGN_RIGHT, "o");
                    al_draw_text(menu_element, cor_selecionada, x_o_direito, pos_y[i], ALLEGRO_ALIGN_LEFT, "o");
                }
            }
        }
        al_flip_display();
        al_rest(1.0 / FPS);
    }
}

/* gerencia a tela de 'Game Over' */
EstadoDoJogo executar_tela_game_over() {
    int opcao_selecionada = 0;
    const int TOTAL_OPCOES = 2;
    const char* opcoes_texto[] = {"Reiniciar", "Voltar ao Menu"};

    while (true) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return ESTADO_SAIR;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_A:
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_D:
                    case ALLEGRO_KEY_RIGHT:
                        opcao_selecionada = (opcao_selecionada + 1) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if (opcao_selecionada == 0)
                            return ESTADO_JOGANDO;
                        if (opcao_selecionada == 1)
                            return ESTADO_MENU;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return ESTADO_MENU;
                }
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));
        if (game_over_img)
            al_draw_bitmap(game_over_img, 0, 0, 0);

        ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 204, 0);

        int pos_x[] = {SCREEN_WIDTH / 2 - 150, SCREEN_WIDTH / 2 + 150};
        int y_pos = SCREEN_HEIGHT - 100;

        for (int i = 0; i < TOTAL_OPCOES; i++) {
            ALLEGRO_COLOR cor_atual = (opcao_selecionada == i) ? cor_selecionada : cor_normal;
            al_draw_text(menu_font_options, cor_atual, pos_x[i], y_pos, ALLEGRO_ALIGN_CENTER, opcoes_texto[i]);

            if (opcao_selecionada == i) {
                int largura_texto = al_get_text_width(menu_font_options, opcoes_texto[i]);
                int padding = 15;
                float x_o_esquerdo = pos_x[i] - (largura_texto / 2.0) - padding;
                float x_o_direito = pos_x[i] + (largura_texto / 2.0) + padding;

                al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, y_pos, ALLEGRO_ALIGN_RIGHT, "o");
                al_draw_text(menu_element, cor_selecionada, x_o_direito, y_pos, ALLEGRO_ALIGN_LEFT, "o");
            }
        }
        al_flip_display();
        al_rest(1.0 / FPS);
    }
}

/* gerencia a tela exibida apos concluir uma fase */
EstadoDoJogo executar_tela_fase_concluida() {
    salvar_progresso(nivel_atual + 1);

    int opcao_selecionada = 0;
    const int TOTAL_OPCOES = 2;
    const char* opcoes_texto[] = {"Próxima Fase", "Voltar ao Menu"};

    while (true) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return ESTADO_SAIR;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = (opcao_selecionada + 1) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_W:
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = (opcao_selecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if (opcao_selecionada == 0) {
                            if (nivel_atual >= 2) 
                                return ESTADO_VITORIA_FINAL;
                            nivel_atual++;
                            return ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 1) 
                            return ESTADO_MENU;
                        break;
                }
            }
        }

        char texto_fase[30];
        sprintf(texto_fase, "Fase %d Concluída!", nivel_atual);

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(menu_font_large, al_map_rgb(255, 204, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, ALLEGRO_ALIGN_CENTER, texto_fase);

        ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 204, 0);
        int y_posicoes[] = {SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2 + 50};

        for (int i = 0; i < TOTAL_OPCOES; i++) {
             ALLEGRO_COLOR cor_atual = (opcao_selecionada == i) ? cor_selecionada : cor_normal;
             al_draw_text(menu_font_options, cor_atual, SCREEN_WIDTH / 2, y_posicoes[i], ALLEGRO_ALIGN_CENTER, opcoes_texto[i]);

             if(opcao_selecionada == i) {
                int largura_texto = al_get_text_width(menu_font_options, opcoes_texto[i]);
                int padding = 15;
                float x_o_esquerdo = SCREEN_WIDTH / 2 - (largura_texto / 2.0) - padding;
                float x_o_direito = SCREEN_WIDTH / 2 + (largura_texto / 2.0) + padding;

                al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, y_posicoes[i], ALLEGRO_ALIGN_RIGHT, "o");
                al_draw_text(menu_element, cor_selecionada, x_o_direito, y_posicoes[i], ALLEGRO_ALIGN_LEFT, "o");
             }
        }

        al_flip_display();
        al_rest(1.0 / FPS);
    }
}

/* funcao de estado para transicao de vitoria de fase */
EstadoDoJogo executar_tela_vitoria() {
    return ESTADO_VITORIA;
}

/* gerencia a tela de vitoria final do jogo */
EstadoDoJogo executar_tela_vitoria_final() {
    int opcao_selecionada = 0;
    const int TOTAL_OPCOES = 2;
    const char* opcoes_texto[] = {"Jogar Novamente", "Voltar ao Menu"};

    while(true) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return ESTADO_SAIR;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        opcao_selecionada = (opcao_selecionada + 1) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_W:
                    case ALLEGRO_KEY_UP:
                        opcao_selecionada = (opcao_selecionada - 1 + TOTAL_OPCOES) % TOTAL_OPCOES;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if (opcao_selecionada == 0) {
                            nivel_atual = 1;
                            return ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 1)
                            return ESTADO_MENU;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return ESTADO_MENU;
                }
            }
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));
        if (game_win_img)
            al_draw_bitmap(game_win_img, 0, 0, 0);

        ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 204, 0);

        int left_x = 40;
        int pos_y[] = {SCREEN_HEIGHT / 2 + 40, SCREEN_HEIGHT / 2 + 110};

        for (int i = 0; i < TOTAL_OPCOES; i++) {
            ALLEGRO_COLOR cor_atual = (opcao_selecionada == i) ? cor_selecionada : cor_normal;
            al_draw_text(menu_font_options, cor_atual, left_x, pos_y[i], ALLEGRO_ALIGN_LEFT, opcoes_texto[i]);

            if (opcao_selecionada == i) {
                int largura_texto = al_get_text_width(menu_font_options, opcoes_texto[i]);
                int padding = 15;
                float x_o_esquerdo = left_x - padding;
                float x_o_direito = left_x + largura_texto + padding;

                al_draw_text(menu_element, cor_selecionada, x_o_esquerdo, pos_y[i], ALLEGRO_ALIGN_RIGHT, "o");
                al_draw_text(menu_element, cor_selecionada, x_o_direito, pos_y[i], ALLEGRO_ALIGN_LEFT, "o");
            }
        }

        al_flip_display();
        al_rest(1.0 / FPS);
    }
}

/* funcao principal do jogo, contendo o loop de jogo, alem de gerenciar a criacao de entidades, eventos, logica de atualizacao, colisoes e renderizacao */
 EstadoDoJogo executar_jogo() {
    /* variaveis de estado da sessao de jogo */
    square *player = NULL;
    enemy *enemies[MAX_ENEMIES] = {NULL};
    boss *final_boss = NULL;

    float camera_x = 0;
    int enemies_killed_count = 0;
    int active_enemies_count = 0;
    float next_enemy_spawn_x = 0;
    int boss_spawned = 0;
    bool heal_active = false;
    float heal_x = 0, heal_y = GROUND_LEVEL;
    double last_heal_spawn = 0;
    int walk_frame = 0, jump_frame = 0, frame_counter = 0, shoot_timer = 15;
    int enemy_walk_frame = 0, enemy_jump_frame = 0, enemy_shoot_anim_timer = 0, enemy_frame_counter = 0;
    bool paused = false;

    /* inicializa ou reinicia todas as variaveis e entidades da sessao */
    reiniciar_jogo_sessao(&player, enemies, &final_boss, &camera_x, &enemies_killed_count, &active_enemies_count, &next_enemy_spawn_x, &boss_spawned,
    &heal_active, &last_heal_spawn);

    bool jogo_em_andamento = true;
    while(jogo_em_andamento) {
        /* secao de processamento de eventos */
        ALLEGRO_EVENT event;
        while(al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                jogo_em_andamento = false;
                destruir_sessao_jogo(player, enemies, final_boss);
                return ESTADO_SAIR;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        jogo_em_andamento = false;
                        break;
                    case ALLEGRO_KEY_P:  /* pausa/despausa o jogo */
                        paused = !paused;
                        break;
                    case ALLEGRO_KEY_A:
                    case ALLEGRO_KEY_LEFT:
                        joystick_left(player->control, 1);
                        break;
                    case ALLEGRO_KEY_D:
                    case ALLEGRO_KEY_RIGHT:
                        joystick_right(player->control, 1);
                        break;
                    case ALLEGRO_KEY_W:
                    case ALLEGRO_KEY_UP:
                        joystick_up(player->control, 1);
                        square_jump(player);
                        break;
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        joystick_down(player->control, 1);
                        square_duck(player, 1);
                        break;
                    case ALLEGRO_KEY_C:
                    case ALLEGRO_KEY_SPACE:
                        joystick_fire(player->control, 1);
                        square_shoot(player); shoot_timer = 15;
                        break;
                    case ALLEGRO_KEY_F:
                        square_special_shoot(player);
                        shoot_timer = 15;
                        break;
                }
            }
            else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_A:
                    case ALLEGRO_KEY_LEFT:
                        joystick_left(player->control, 0);
                        break;
                    case ALLEGRO_KEY_D:
                    case ALLEGRO_KEY_RIGHT:
                        joystick_right(player->control, 0);
                        break;
                    case ALLEGRO_KEY_S:
                    case ALLEGRO_KEY_DOWN:
                        square_duck(player, 0);
                        joystick_down(player->control, 0);
                        break;
                    case ALLEGRO_KEY_C:
                    case ALLEGRO_KEY_SPACE:
                        joystick_fire(player->control, 0);
                        break;
                }
            }
        }

        /* secao de logica do jogo (so executa se nao estiver pausado) */
        if (!paused) {
            camera_x = player->x - SCREEN_WIDTH / 2;

            if (!player->is_dying) {
                square_move(player);
                square_update(player);
            }

            active_enemies_count = 0;
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i]) {
                    enemy_update(enemies[i], player);
                    for (bullet *b = enemies[i]->gun->shots; b != NULL; b = (bullet*)b->next)
                        bullet_move(b);
                    if (enemies[i]->health > 0)
                        active_enemies_count++;
                }
            }

            if (player->x >= next_enemy_spawn_x && enemies_killed_count < ENEMIES_TO_KILL_FOR_BOSS) {
                 if (active_enemies_count < MAX_ENEMIES) {
                    for (int i = 0; i < MAX_ENEMIES; i++) {
                        if (enemies[i] == NULL) {
                            enemies[i] = enemy_create(camera_x + SCREEN_WIDTH - 100 + (rand() % 200), GROUND_LEVEL, 2);
                            break;
                        }
                    }
                 }
                 next_enemy_spawn_x += ENEMY_SPAWN_DISTANCE;
            }

            for (int i=0; i<MAX_ENEMIES; i++) {
                if(enemies[i] && enemies[i]->is_dying && al_get_time() - enemies[i]->time_of_death >= 2.0) {
                    enemy_destroy(enemies[i]);
                    enemies[i] = NULL;
                    enemies_killed_count++;
                }
            }

            if (!boss_spawned && enemies_killed_count >= ENEMIES_TO_KILL_FOR_BOSS && active_enemies_count == 0) {
                final_boss = boss_create(camera_x + SCREEN_WIDTH - 200, GROUND_LEVEL, nivel_atual);
                boss_spawned = 1;
            }

            if (!heal_active && al_get_time() - last_heal_spawn >= 20) {
                 heal_x = camera_x + (rand() % (SCREEN_WIDTH - al_get_bitmap_width(heal_sprite)));
                 heal_active = true;
                 last_heal_spawn = al_get_time();
            }
            if (heal_active && player->x + player->width/2 > heal_x && player->x - player->width/2 < heal_x + al_get_bitmap_width(heal_sprite) &&
            player->y > heal_y - al_get_bitmap_height(heal_sprite)) {
                player->health = fmin(player->max_health, player->health + 3);
                heal_active = false;
            }

            if (final_boss && final_boss->active) {
                boss_update(final_boss, player);
                boss_move(final_boss);
                for (bullet *b = final_boss->gun->shots; b != NULL; b = (bullet*)b->next)
                    bullet_move(b);
            }

            bullet *prev_p_b = NULL, *p_b = player->gun->shots;
            while(p_b) {
                bullet *next_p_b = (bullet*)p_b->next;
                bool hit = false;
                for (int i=0; i < MAX_ENEMIES && !hit; i++) {
                    if (enemies[i] && !enemies[i]->is_dying && check_bullet_collision(p_b, enemies[i])) {
                        hit = true;
                        if(enemies[i]->health <= 0) {
                            enemies[i]->is_dying = true;
                            enemies[i]->time_of_death = al_get_time();
                        }
                    }
                }
                if (!hit && final_boss && final_boss->active && check_bullet_collision_boss(p_b, final_boss)) {
                    hit = true;
                    if (p_b->type == 1)
                        final_boss->health -= 3;
                    else
                        final_boss->health -= 1;
                    if(final_boss->health <= 0) {
                        final_boss->health = 0;
                        if (final_boss->time_of_death == 0.0)
                            final_boss->time_of_death = al_get_time();
                    }
                }
                
                if (hit || p_b->x - camera_x < 0 || p_b->x - camera_x > SCREEN_WIDTH) {
                    if (prev_p_b)
                        prev_p_b->next = (struct bullet*) next_p_b;
                    else
                        player->gun->shots = next_p_b;
                    bullet_destroy(p_b);
                }
                else
                    prev_p_b = p_b;

                p_b = next_p_b;
            }

            for(int i=0; i < MAX_ENEMIES; i++) {
                if(enemies[i]) {
                    bullet *prev_e_b = NULL, *e_b = enemies[i]->gun->shots;
                    while(e_b) {
                        bullet *next_e_b = (bullet*)e_b->next;
                        if(check_player_bullet_collision(e_b, player)) {
                            player->health--;
                            if(prev_e_b)
                                prev_e_b->next = (struct bullet*)next_e_b;
                            else
                                enemies[i]->gun->shots = next_e_b;
                            bullet_destroy(e_b);
                        }
                        else
                            prev_e_b = e_b;

                        e_b = next_e_b;
                    }
                }
            }

            if (final_boss && final_boss->active) {
                bullet *prev_b_b = NULL, *b_b = final_boss->gun->shots;
                 while(b_b) {
                    bullet *next_b_b = (bullet*)b_b->next;
                    if(check_player_bullet_collision(b_b, player)) {
                        player->health--;
                        if(prev_b_b)
                            prev_b_b->next = (struct bullet*)next_b_b;
                        else
                            final_boss->gun->shots = next_b_b;
                        bullet_destroy(b_b);
                    }
                    else
                        prev_b_b = b_b;

                    b_b = next_b_b;
                }
            }

            if(final_boss && final_boss->health <= 0 && final_boss->time_of_death > 0.0 && al_get_time() - final_boss->time_of_death >= 2.0) {
                destruir_sessao_jogo(player, enemies, final_boss);
                return ESTADO_VITORIA;
            }

            if (player->health <= 0 && !player->is_dying) {
                player->is_dying = true;
                player->time_of_death = al_get_time();
            }

            if (player->is_dying && al_get_time() - player->time_of_death >= 2.0) {
                destruir_sessao_jogo(player, enemies, final_boss);
                return ESTADO_GAME_OVER;
            }

            if(++frame_counter >= 10) {
                walk_frame = (walk_frame + 1) % 2;
                jump_frame = (jump_frame + 1) % 6;
                frame_counter = 0;
            }
            if(++enemy_frame_counter >= 10) {
                enemy_walk_frame = (enemy_walk_frame + 1) % 2;
                enemy_jump_frame = (enemy_jump_frame + 1) % 3;
                enemy_frame_counter = 0;
            }
            if(shoot_timer > 0)
                shoot_timer--;
            if(enemy_shoot_anim_timer > 0)
                enemy_shoot_anim_timer--;
        }

        al_clear_to_color(al_map_rgb(0,0,0));

        ALLEGRO_BITMAP *current_background = (nivel_atual == 1) ? background : background_fase2;
        int bg_width_sprite = al_get_bitmap_width(current_background);
        float bg_offset_scroll = -fmod(camera_x * bg_scroll_speed, bg_width_sprite);
        for (int i = 0; i < (SCREEN_WIDTH / bg_width_sprite) + 2; i++)
            al_draw_bitmap(current_background, bg_offset_scroll + (i * bg_width_sprite), 0, 0);
        if (bg_offset_scroll > 0)
            al_draw_bitmap(current_background, bg_offset_scroll - bg_width_sprite, 0, 0);

        int heart_size = 20;
        int heart_x = 10, heart_y = 10;
        al_draw_scaled_bitmap(heart, 0, 0, al_get_bitmap_width(heart), al_get_bitmap_height(heart), heart_x, heart_y, heart_size, heart_size, 0);
        int bar_x = heart_x + heart_size + 8, bar_y = heart_y + (heart_size / 2) - 7, bar_width = 300, bar_height = 14;
        float percent = (float)player->health / player->max_health;
        
        if(percent < 0)
            percent = 0;
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + (bar_width * percent), bar_y + bar_height, al_map_rgb(255, 0, 0));
        al_draw_rectangle(bar_x - 1, bar_y - 1, bar_x + bar_width + 1, bar_y + bar_height + 1, al_map_rgb(255, 255, 255), 2);

        int stamina_size = 20, stamina_x = 10, stamina_y = 40;
        al_draw_scaled_bitmap(stamina_icon, 0, 0, al_get_bitmap_width(stamina_icon), al_get_bitmap_height(stamina_icon), stamina_x,
        stamina_y, stamina_size, stamina_size, 0);
        bar_x = stamina_x + stamina_size + 8;
        bar_y = stamina_y + (stamina_size / 2) - 7;
        percent = (float)player->stamina / player->max_stamina;
        
        if(percent < 0)
            percent = 0;
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + (bar_width * percent), bar_y + bar_height, al_map_rgb(50, 200, 50));
        al_draw_rectangle(bar_x - 1, bar_y - 1, bar_x + bar_width + 1, bar_y + bar_height + 1, al_map_rgb(255, 255, 255), 2);
        if (heal_active)
            al_draw_bitmap(heal_sprite, heal_x - camera_x, heal_y - al_get_bitmap_height(heal_sprite), 0);

        ALLEGRO_BITMAP *player_sprite = sprite_standing;

        if (player->is_dying)
            player_sprite = sprite_dying; 
        else {
            if (player->state == JUMPING && shoot_timer > 0)
                player_sprite = sprite_jump_shooting;
            else if (player->state == DUCKING && shoot_timer > 0)
                player_sprite = sprite_duck_shooting;
            else if (shoot_timer > 0)
                player_sprite = sprite_shooting;
            else if (player->state == DUCKING && (player->control->left || player->control->right))
                player_sprite = sprite_crouch_walking;
            else {
                switch (player->state) {
                    case WALKING:
                        player_sprite = sprite_walking[walk_frame];
                        break;
                    case JUMPING:
                        player_sprite = sprite_jumping[jump_frame];
                        break;
                    case DUCKING:
                        player_sprite = sprite_crouching;
                        break;
                    default:
                        player_sprite = sprite_standing;
                        break;
                }
            }
        }

        al_draw_bitmap(player_sprite, SCREEN_WIDTH/2 - al_get_bitmap_width(player_sprite)/2, player->y - al_get_bitmap_height(player_sprite),
        player->face == 0 ? ALLEGRO_FLIP_HORIZONTAL : 0);

        for(bullet *b = player->gun->shots; b; b=(bullet*)b->next) {
            int flip_flag = (player->face == 0) ? ALLEGRO_FLIP_HORIZONTAL : 0;
            ALLEGRO_BITMAP* bullet_sprite = (b->type == 1) ? special_bullet : sprite_bullet;
            al_draw_bitmap(bullet_sprite, b->x - camera_x, b->y-5, flip_flag);
        }

        for(int i=0; i<MAX_ENEMIES; i++) {
            if(enemies[i]) {
                ALLEGRO_BITMAP *enemy_sprite = enemy_sprite_standing;
                if (enemies[i]->is_dying)
                    enemy_sprite = enemy_sprite_dying[1];
                else if (enemies[i]->y < GROUND_LEVEL)
                    enemy_sprite = enemy_sprite_jumping[enemy_jump_frame];
                else if (enemies[i]->control->left || enemies[i]->control->right)
                     enemy_sprite = enemy_sprite_walking[enemy_walk_frame];
                else if (enemies[i]->gun->cooldown > 0)
                    enemy_sprite = enemy_sprite_shooting;
                al_draw_bitmap(enemy_sprite, enemies[i]->x - camera_x - al_get_bitmap_width(enemy_sprite)/2, enemies[i]->y - al_get_bitmap_height(enemy_sprite),
                enemies[i]->face == 0 ? ALLEGRO_FLIP_HORIZONTAL : 0);
                for(bullet *b = enemies[i]->gun->shots; b; b=(bullet*)b->next)
                    al_draw_bitmap(sprite_bullet_enemy, b->x - camera_x, b->y-5, 0);
            }
        }

        if (final_boss) {
            ALLEGRO_BITMAP *current_boss_sprite = NULL;
            ALLEGRO_BITMAP *current_boss_bullet_sprite = NULL;

            if (final_boss->is_visible) {
                if (nivel_atual == 1) {
                    if (final_boss->health <= 0)
                        current_boss_sprite = boss_dying;
                    else if (final_boss->active) {
                        if (final_boss->gun->cooldown > 0)
                            current_boss_sprite = boss_shooting;
                        else if (final_boss->control->left || final_boss->control->right)
                            current_boss_sprite = boss_walking[walk_frame];
                        else
                            current_boss_sprite = boss_standing;
                    }
                    current_boss_bullet_sprite = boss_bullet;
                }
                
                else if (nivel_atual == 2) {
                    if (final_boss->health <= 0)
                        current_boss_sprite = boss_fase2_dying;
                    else if (final_boss->active) {
                        if (final_boss->gun->cooldown > 0)
                            current_boss_sprite = boss_fase2_shooting;
                        else
                            current_boss_sprite = boss_fase2_standing;
                    }
                    current_boss_bullet_sprite = boss_fase2_bullet;
                }

                if (current_boss_sprite)
                    al_draw_bitmap(current_boss_sprite,final_boss->x - al_get_bitmap_width(current_boss_sprite)/2 - camera_x,
                    final_boss->y - al_get_bitmap_height(current_boss_sprite), final_boss->face == 0 ? ALLEGRO_FLIP_HORIZONTAL : 0);
            }

            
            if (nivel_atual == 1)
                current_boss_bullet_sprite = boss_bullet;
            else if (nivel_atual == 2)
                current_boss_bullet_sprite = boss_fase2_bullet;

            
            for(bullet *b = final_boss->gun->shots; b; b=(bullet*)b->next) {
                if (current_boss_bullet_sprite)
                    al_draw_bitmap(current_boss_bullet_sprite, b->x - camera_x, b->y-5, 0);
            }
        }

        
        if (paused) {
            al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0,0,0,180));
            al_draw_text(menu_font_options, al_map_rgb(255,255,255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 30, ALLEGRO_ALIGN_CENTER, "PAUSADO");
        }

        al_flip_display();
        al_rest(1.0 / FPS);
    }

    
    destruir_sessao_jogo(player, enemies, final_boss);
    return ESTADO_SAIR;
}

/* inicializa o Allegro e todos os addons necessarios */
bool inicializar_allegro() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de imagens.\n");
        return false;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar primitivas.\n");
        return false;
    }
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Falha ao inicializar addon TTF.\n");
        return false;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao instalar teclado.\n");
        return false;
    }

    return true;
}

/* carrega todos os recursos que serao usados em varias partes do jogo */
bool carregar_recursos_globais() {
    menu_background = al_load_bitmap("sprites/menu.png");
    background = al_load_bitmap("sprites/background.png");
    background_fase2 = al_load_bitmap("sprites/background_fase2.png");
    seleciona_fase_background = al_load_bitmap("sprites/seleciona_fase.png");
    game_over_img = al_load_bitmap("sprites/game_over_img.png");
    game_win_img = al_load_bitmap("sprites/win_img.png");
    heal_sprite = al_load_bitmap("sprites/heal.png");
    menu_font_large = al_load_ttf_font("Magic Retro.ttf", 48, 0);
    menu_font_medium = al_load_ttf_font("Magic Retro.ttf", 24, 0);
    menu_font_options = al_load_ttf_font("Magic Retro.ttf", 36, 0);
    menu_element = al_load_ttf_font("Cheeky Rabbit.ttf", 36, 0);

    if (!menu_font_large || !menu_font_medium || !menu_font_options || !menu_element) {
        fprintf(stderr, "Falha ao carregar fontes! Usando fonte padrao.\n");
        menu_font_large = menu_font_medium = al_create_builtin_font();
    }

    if (!menu_background || !background || !heal_sprite || !background_fase2 || !game_over_img || !game_win_img || !seleciona_fase_background) {
        fprintf(stderr, "Falha ao carregar uma ou mais imagens de fundo/UI.\n");
        return false;
    }

    if (!load_sprites()) {
        fprintf(stderr, "Falha ao carregar um ou mais sprites do jogo.\n");
        return false;
    }

    return true;
}

/* Libera todos os recursos globais e finaliza os sistemas do Allegro. */
void destruir_recursos_globais() {
    destroy_sprites();
    if (menu_background) al_destroy_bitmap(menu_background);
    if (background) al_destroy_bitmap(background);
    if (background_fase2) al_destroy_bitmap(background_fase2);
    if (seleciona_fase_background) al_destroy_bitmap(seleciona_fase_background);
    if (game_over_img) al_destroy_bitmap(game_over_img);
    if (game_win_img) al_destroy_bitmap(game_win_img);
    if (heal_sprite) al_destroy_bitmap(heal_sprite);
    if (menu_font_large && menu_font_large != al_create_builtin_font()) al_destroy_font(menu_font_large);
    if (menu_font_medium && menu_font_medium != al_create_builtin_font()) al_destroy_font(menu_font_medium);
    if (menu_font_options && menu_font_options != menu_font_medium) al_destroy_font(menu_font_options);
    if (menu_element && menu_element != al_create_builtin_font()) al_destroy_font(menu_element);
    if (event_queue) al_destroy_event_queue(event_queue);
    if (display) al_destroy_display(display);

    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_uninstall_system();
}

/* reinicia o estado de uma sessao de jogo */
void reiniciar_jogo_sessao(square **player_ptr, enemy *enemies[], boss **final_boss_ptr, float *camera_x, int *enemies_killed_count, int *active_enemies_count,
    float *next_enemy_spawn_x, int *boss_spawned, bool *heal_active, double *last_heal_spawn) {
    /* destroi a sessao anterior para evitar vazamentos de memoria */
    destruir_sessao_jogo(*player_ptr, enemies, *final_boss_ptr);

    /* cria as novas entidades e reseta as variaveis de estado */
    *player_ptr = square_create(50, GROUND_LEVEL);
    *final_boss_ptr = NULL;
    for (int i = 0; i < MAX_ENEMIES; i++)
        enemies[i] = NULL;

    *camera_x = 0;
    *enemies_killed_count = 0;
    *active_enemies_count = 0;
    *next_enemy_spawn_x = INITIAL_SPAWN_X;
    *boss_spawned = 0;
    *heal_active = false;
    *last_heal_spawn = al_get_time();
}

/* libera a memoria de todas as entidades pertencentes a uma sessao de jogo */
void destruir_sessao_jogo(square* player, enemy *enemies[], boss* final_boss) {
    if (player)
        square_destroy(player);
    if (final_boss)
        boss_destroy(final_boss);
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i]) {
            enemy_destroy(enemies[i]);
            enemies[i] = NULL;
        }
    }
}