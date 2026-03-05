#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "conjunto.h"
#include "lef.h"
#include "fila.h"

/* defines */
#define T_INICIO 0
#define T_FIM_DE_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DE_MUNDO / 100)

/* estruturas */
struct local {
    int x;
    int y;
};

struct distanciaBase {
    int ID;
    int distancia;
};

struct herois {
    int ID_herois;
    struct conjunto *habilidades;
    int paciencia;
    int velocidade;
    int xp;
    int base_ID;
};

struct base {
    int ID_base;
    int lotacao;
    struct conjunto *presentes;
    struct fila *fila_de_espera;
    struct local *local_base;

};

struct missao {
    int ID_missao;
    struct conjunto *hab_necessarias;
    struct local *local_missao;
};

struct mundo {
    int NHerois;
    struct herois *Herois[N_HEROIS];
    int NBases;
    struct base *Bases[N_BASES];
    int NMissoes;
    struct missao *Missoes[N_MISSOES];
    int NHabilidades;
    int TamMundo;
    int tempo;
};
////////////////////////////

/* inicializacao do mundo */
struct mundo *cria_mundo() {
    struct mundo *Mundo = (struct mundo *)malloc(sizeof(struct mundo));
    Mundo->NHerois = N_HEROIS;
    Mundo->NBases = N_BASES;
    Mundo->NMissoes = N_MISSOES;
    Mundo->TamMundo = N_TAMANHO_MUNDO;
    Mundo->tempo = 0;

    /* inicializacao herois */
    if (Mundo->Herois != NULL) {
        for (int i = 0; i < Mundo->NHerois; i++) {
            Mundo->Herois[i] = malloc(sizeof(struct herois));
            Mundo->Herois[i]->ID_herois = i;
            Mundo->Herois[i]->xp = 0;

            Mundo->Herois[i]->paciencia = rand() % 100;
            Mundo->Herois[i]->velocidade = rand() % (5000 - 50 + 1) + 50;

            /* Gera um valor aleatorio entre 1 e 3 para a quantidade de habilidades */
            Mundo->Herois[i]->habilidades = cria_cjt(rand() % 3 + 1);
            Mundo->Herois[i]->habilidades->card = Mundo->Herois[i]->habilidades->max;

            /* Faz um conjunto de habilidades aleatorias */
            for (int j = 0; j < Mundo->Herois[i]->habilidades->max; j++)
                Mundo->Herois[i]->habilidades->v[j] = rand() % 10;
        }
    }
    else
        printf("Falha em alocaar os herois\n");
    /* inicializacao bases */
    for (int i = 0; i < Mundo->NBases; i++) {
        Mundo->Bases[i] = malloc(sizeof(struct base));
        if (Mundo->Bases[i] != NULL) {
            Mundo->Bases[i]->ID_base = i;

            Mundo->Bases[i]->lotacao = rand() % (10 - 3 + 1) + 3;

            /* Define as coordenadas da base */
            Mundo->Bases[i]->local_base = malloc(sizeof(struct local)); 
            Mundo->Bases[i]->local_base->x = rand() % N_TAMANHO_MUNDO - 1;
            Mundo->Bases[i]->local_base->y = rand() % N_TAMANHO_MUNDO - 1;

            /* Cria um conjunto vazio de presentes na base e uma fila de espera vazia */
            Mundo->Bases[i]->presentes = cria_cjt(Mundo->Bases[i]->lotacao);
            Mundo->Bases[i]->fila_de_espera = fila_cria();
        }
        else
            printf("Falha em alocaar as bases\n");
    }
     /* inicializacao missoes */
     for (int i = 0; i < Mundo->NMissoes; i++) {
            Mundo->Missoes[i] = malloc(sizeof(struct missao));
            if (Mundo->Missoes[i] != NULL) {
                Mundo->Missoes[i]->ID_missao = i;

                /* Define as coordenadas da missao */
                Mundo->Missoes[i]->local_missao = malloc(sizeof(struct local));
                Mundo->Missoes[i]->local_missao->x = rand() % N_TAMANHO_MUNDO - 1;
                Mundo->Missoes[i]->local_missao->y = rand() % N_TAMANHO_MUNDO - 1;

                /* Gera um valor aleatorio entre 6 e 10 para a quantidade de habilidades */
                Mundo->Missoes[i]->hab_necessarias = cria_cjt(rand() % 5 + 6);
                Mundo->Missoes[i]->hab_necessarias->card = Mundo->Missoes[i]->hab_necessarias->max;

                /* Faz um conjunto de habilidades aleatorias */
                for (int j = 0; j < Mundo->Missoes[i]->hab_necessarias->max; j++)
                    Mundo->Missoes[i]->hab_necessarias->v[j] = rand() % 10;
            }
            else
            printf("Falha em alocar as missoes\n");
     }

    return Mundo;
}

void destroi_mundo(struct mundo* Mundo) {
    /* laco para liberar cada heroi */
    for (int i = 0; i < Mundo->NHerois; i++) {
        if (Mundo->Herois[i]->habilidades != NULL) {
            //free(Mundo->Herois[i]->habilidades->v);
            destroi_cjt(Mundo->Herois[i]->habilidades);
        }

        free(Mundo->Herois[i]);
    }

    /* laco para liberar cada base */
    for (int i = 0; i < Mundo->NBases; i++) {
        free(Mundo->Bases[i]->local_base);
        destroi_cjt(Mundo->Bases[i]->presentes);
        fila_destroi(&(Mundo->Bases[i]->fila_de_espera));
        free(Mundo->Bases[i]);

    }

    /* laco para liberar cada missao */
    for (int i = 0; i < Mundo->NMissoes; i++) {
        free(Mundo->Missoes[i]->local_missao);
        destroi_cjt(Mundo->Missoes[i]->hab_necessarias);
        free(Mundo->Missoes[i]);
    }

    /* libera a estrutura mundo */
    free(Mundo);
}

////////////////////////////

/* retorna 1 se o heroi vai esperar e 0 caso ele desista */
int evento_chega(struct herois *Herois, int tempo, struct base *Bases) {
    int espera = 0;
    Herois->base_ID = Bases->ID_base;

    /* deteremina se a base nao esta cheia */
    if ((Bases->lotacao > Bases->presentes->card) && (fila_vazia(Bases->fila_de_espera)))
        espera = 1;
        /* determina se o heroi tem paciencia para esperar na fila da base */
    else if (( Herois->paciencia ) > ( 10 * fila_tamanho(Bases->fila_de_espera)))
            espera = 1;
         else
            espera = 0;
    
    return espera;
}

int evento_desiste(int tempo, struct herois *Herois, struct base *Bases, struct mundo *Mundo) {
    printf ("%6d: DESISTE HEROI %2d BASE %d\n", tempo, Herois->ID_herois, Bases->ID_base);
    /* define uma base destino aleatoria*/
    return rand () % N_BASES;
}

void evento_espera(int tempo, struct herois *Herois, struct base *Bases) {
    /* adiciona o ID do heroi na fila de espera da base */
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, Herois->ID_herois, Bases->ID_base, Bases->presentes->card);
    enqueue(Bases->fila_de_espera, Herois->ID_herois);
}
  
void evento_avisa(int tempo, struct base *Bases) {
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, Bases->ID_base, Bases->presentes->card, Bases->lotacao);
    imprime_cjt(Bases->presentes);
    return;
}

/* calcula o TPB e retorna o valor */
int evento_entra(int tempo, struct herois *Herois, struct base *Bases) {
    int TPB;
    /* numero aleatorio auxiliar */
    int aux;
    aux = (rand () % 20) + 1;

    /* calcula o tempo de permanencia do heroi na base */
    TPB = 15 + (Herois->paciencia * aux);
    printf ("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, Herois->ID_herois, Bases->ID_base, Bases->presentes->card, Bases->lotacao, tempo + TPB);
    return TPB;
}

/* retira o heroi do conjunto de presentes da base e retorna uma base aleatoria */
int evento_sai(int tempo, struct herois *Herois, struct base *Bases, struct mundo *Mundo) {
    retira_cjt(Bases->presentes, Herois->ID_herois);
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, Herois->ID_herois, Bases->ID_base, Bases->presentes->card, Bases->lotacao);
    /* define uma base destino aleatoria */
    int base_aleatoria;
    /* um do while para que o heroi nao va para a mesma base */
    do 
        base_aleatoria = rand() % N_BASES;
    while (base_aleatoria == Bases->ID_base);

    return base_aleatoria;
}

/* calcula a distancia do heroi ate a outra base e a retorna */
int evento_viaja(int tempo, struct herois *Herois, int base_destino, struct mundo *Mundo) {
    int distancia;
    int duracao;
    int distancia_x = ((Mundo->Bases[base_destino]->local_base->x - Mundo->Bases[Herois->base_ID]->local_base->x) * (Mundo->Bases[base_destino]->local_base->x - Mundo->Bases[Herois->base_ID]->local_base->x));
    int distancia_y = ((Mundo->Bases[base_destino]->local_base->y - Mundo->Bases[Herois->base_ID]->local_base->y) * (Mundo->Bases[base_destino]->local_base->y - Mundo->Bases[Herois->base_ID]->local_base->y));
    /* calcula a distancia e duracao da viajem a ser feita */
    /* distancia cartesiana = sqrt((x1 - x2)^2) + ((y1 - y2)^2) */
    distancia = distancia_x + distancia_y;
    distancia = sqrt(distancia);
    duracao = (distancia / Herois->velocidade);
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, Herois->ID_herois, Herois->base_ID, base_destino, distancia, Herois->velocidade, tempo + duracao);
   
    /* retorna o tempo de duracao da viajem */
    return duracao;
}

/* funcao auxiliar para ordenar */
int comparar(const void* a, const void* b) {
    return (*(struct distanciaBase*)a).distancia - (*(struct distanciaBase*)b).distancia;
}

/* retorna 1 se a missao for concluida e 0 caso contrario */
int evento_missao(int tempo, struct missao *Missao, struct mundo *Mundo) {
    double menor_distancia = INT_MAX;
    int distancia_atual = 0;
    int base_mais_proxima = -1;
    int base_apta = 0;
    int igual;

    struct conjunto *habilidades_base; 

    /* vetor com a distancia de todas as bases ate a missao */
    struct distanciaBase *distanciasBases = malloc(N_BASES * sizeof(struct distanciaBase));
    for (int i = 0; i < N_BASES; i++) {
        distanciasBases[i].distancia = sqrt((Mundo->Bases[i]->local_base->x - Missao->local_missao->x) * (Mundo->Bases[i]->local_base->x - Missao->local_missao->x)) + ((Mundo->Bases[i]->local_base->y - Missao->local_missao->y) * (Mundo->Bases[i]->local_base->y - Missao->local_missao->y));
        distanciasBases[i].ID = i;
    }

    /* usa o qsort para ordenar o vetor da mais proxima para a mais distante da missao */
    qsort(distanciasBases, N_BASES, sizeof(struct distanciaBase), comparar);
    printf("%6d: MISSAO %d HAB REQ: ", tempo, Missao->ID_missao);
    imprime_cjt(Missao->hab_necessarias);

    /* salva as habilidades e a base mais proxima da missao */
    for (int i = 0; i < N_BASES; i++) {
        habilidades_base = cria_cjt(10 * N_HABILIDADES);
        int indice_atual = distanciasBases[i].ID;
        for (int j = 0; j < N_HEROIS; j++) {
            if (pertence_cjt(Mundo->Bases[indice_atual]->presentes, Mundo->Herois[j]->ID_herois)) {
                /* conjunto para armazenar as habilidades totais de uma base */
                struct conjunto *temp = uniao_cjt(habilidades_base, Mundo->Herois[j]->habilidades);
                destroi_cjt(habilidades_base);
                habilidades_base = temp;
            }
        }
        printf("%6d: MISSAO %d HAB BASE %d: ", tempo, Missao->ID_missao, Mundo->Bases[indice_atual]->ID_base);
        imprime_cjt(habilidades_base);

        /* se houver uma base que tem todas as habilidades necessarias */
        if (contido_cjt(Missao->hab_necessarias, habilidades_base) == 1) 
            igual = 1;
        else
            igual = 0;

        if (igual) {
            distancia_atual = distanciasBases[i].distancia;
            if (distancia_atual <= menor_distancia) {
                menor_distancia = distancia_atual;
                base_mais_proxima = distanciasBases[i].ID;
                base_apta = 1;
            }
        }
        if (!vazio_cjt(habilidades_base))
            destroi_cjt(habilidades_base);

    }

        /* incrementa o xp dos herois que estao na base_apta */
        if (base_apta) {
            for (int i = 0; i < N_HEROIS; i++) {
                if (pertence_cjt(Mundo->Bases[base_mais_proxima]->presentes, Mundo->Herois[i]->ID_herois))
                    Mundo->Herois[i]->xp += 1;
            }
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", tempo, Missao->ID_missao, Mundo->Bases[base_mais_proxima]->ID_base);
            imprime_cjt(Mundo->Bases[base_mais_proxima]->presentes);
            free(distanciasBases);

            return 1;
        }
        
        printf("%6d: MISSAO %d IMPOSSIVEL \n", tempo, Missao->ID_missao);
        free(distanciasBases);

        return 0;
}

void evento_fim(struct mundo *Mundo, int missaoConcluida, int missaoAdiada) {
    printf("%6d: FIM\n", Mundo->tempo);
    /* imprime todos os dados de cada heroi */
    for (int i = 0; i < Mundo->NHerois; i++) {
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", Mundo->Herois[i]->ID_herois, Mundo->Herois[i]->paciencia, Mundo->Herois[i]->velocidade, Mundo->Herois[i]->xp);
        /* imprime as habilidades */
        imprime_cjt(Mundo->Herois[i]->habilidades);
    }
    /* imprime as estatisticas finais das missoes */
    printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO \n", missaoConcluida, N_MISSOES, ((double)missaoConcluida/N_MISSOES)*100.0, ((double)missaoConcluida+(double)missaoAdiada)/N_MISSOES);

}


int main () {
    struct lef_t *lef_mundo;
    struct evento_t *evento, *novo_evento; 
    int base_destino, base_atual;
    int heroi_removido;
    int TPB;
    int tempo_de_viajem;
    int M_concluidas = 0, M_adiadas = 0;
    int espera;

    srand (0);

    /* inicializa o mundo, as bases, os herois e as missoes */
    struct mundo *Mundo = cria_mundo();
    lef_mundo = cria_lef();

    /* coloca na lef todos os eventos chega dos herois */
    for (int i = 0; i < N_HEROIS; i++) {
        novo_evento = cria_evento( rand() % 4321, 1, Mundo->Herois[i]->ID_herois, rand() % N_BASES);
        insere_lef(lef_mundo, novo_evento);
        
    }

    /* coloca na lef todas missoes em tempos aleatorios */
    for (int i = 0; i < N_MISSOES; i++) {
        novo_evento = cria_evento( rand() % (T_FIM_DE_MUNDO - 1), 8, Mundo->Missoes[i]->ID_missao, 0);
        insere_lef(lef_mundo, novo_evento);
        
    }
    /* cria o evento fim e coloca na lef */
    struct evento_t *fim = cria_evento(T_FIM_DE_MUNDO, 9, 0, 0); 
    insere_lef(lef_mundo, fim);

   while (!vazia_lef(lef_mundo)) {


        /* faz o primeiro evento da lef */
        evento = retira_lef(lef_mundo);

        /* atualiza o tempo do mundo a cada evento */
        Mundo->tempo = evento->tempo;

        switch (evento->tipo) {

            case 1: /* EVENTO CHEGA */
                 espera = evento_chega(Mundo->Herois[evento->dado1], evento->tempo, Mundo->Bases[evento->dado2]);
                 
                /* se o heroi tiver paciencia ou a base ainda nao estiver cheia ele espera */
                if (espera) {
                    printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", Mundo->tempo, Mundo->Herois[evento->dado1]->ID_herois, Mundo->Bases[evento->dado2]->ID_base, Mundo->Bases[evento->dado2]->presentes->card, Mundo->Bases[evento->dado2]->lotacao);
                    
                    /* adiciona o evento espera na lef */
                    novo_evento = cria_evento(Mundo->tempo, 3, evento->dado1, evento->dado2);
                    insere_lef(lef_mundo, novo_evento);
                    /* apaga o evento atual */
                    destroi_evento(evento);
                }
                else{
                /* caso ele nao tenha paciencia para esperar na fila */
                printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", Mundo->tempo, Mundo->Herois[evento->dado1]->ID_herois, Mundo->Bases[evento->dado2]->ID_base, Mundo->Bases[evento->dado2]->presentes->card, Mundo->Bases[evento->dado2]->lotacao);
                
                /* adiciona o evento desiste na lef */
                novo_evento = cria_evento(Mundo->tempo, 2, evento->dado1, evento->dado2);
                insere_lef(lef_mundo, novo_evento);
                
                /* apaga o evento atual */
                destroi_evento(evento);
                }
                break;
            
            case 2: /* EVENTO DESISTE */
                base_destino = evento_desiste(evento->tempo, Mundo->Herois[evento->dado1], Mundo->Bases[evento->dado2], Mundo);
                /* adiciona o evento viaja na lef */
                novo_evento = cria_evento(Mundo->tempo, 7, evento->dado1, base_destino);
                insere_lef(lef_mundo, novo_evento);
                
                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 3: /* EVENTO ESPERA */
                evento_espera(evento->tempo, Mundo->Herois[evento->dado1], Mundo->Bases[evento->dado2]);                
                /* adiciona o evento avisa na lef */
                novo_evento = cria_evento(Mundo->tempo, 4, evento->dado1, evento->dado2);
                insere_lef(lef_mundo, novo_evento);
                
                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 4: /* EVENTO AVISA */
                evento_avisa(evento->tempo, Mundo->Bases[evento->dado2]);
                /* enquanto a base nao estiver lotada e a fila nao estiver vazia */
                while ((Mundo->Bases[evento->dado2]->lotacao > Mundo->Bases[evento->dado2]->presentes->card) && (fila_tamanho (Mundo->Bases[evento->dado2]->fila_de_espera) != 0)) {                   
                    /* tira o primeiro heroi da fila */
                    dequeue(Mundo->Bases[evento->dado2]->fila_de_espera, &heroi_removido);
                   
                    /* insere o heroi na base */
                    insere_cjt(Mundo->Bases[evento->dado2]->presentes, heroi_removido);
                    
                    /* adiciona o evento entra na lef */
                    novo_evento = cria_evento(Mundo->tempo, 5, heroi_removido, evento->dado2);
                    insere_lef(lef_mundo, novo_evento);
                }
               
                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 5: /* EVENTO ENTRA */
                TPB = evento_entra(evento->tempo, Mundo->Herois[evento->dado1], Mundo->Bases[evento->dado2]);

                /* adiciona o evento sai na lef */
                novo_evento = cria_evento((Mundo->tempo + TPB), 6, evento->dado1, evento->dado2);
                insere_lef(lef_mundo, novo_evento);
                
                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 6: /* EVENTO SAI */
                /* sorteia uma base aleatoria e cria o evento viaja */
                base_atual = evento->dado2;
                base_destino = evento_sai(evento->tempo, Mundo->Herois[evento->dado1], Mundo->Bases[evento->dado2], Mundo);
            
                /* adiciona o evento viaja na lef */
                novo_evento = cria_evento((Mundo->tempo + TPB), 7, evento->dado1, base_destino);
                insere_lef(lef_mundo, novo_evento);
                
                novo_evento = cria_evento(Mundo->tempo, 4, evento->dado1, base_atual);
                insere_lef(lef_mundo, novo_evento);

                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 7: /* EVENTO VIAJA */
                tempo_de_viajem = evento_viaja(evento->tempo, Mundo->Herois[evento->dado1], Mundo->Bases[evento->dado2]->ID_base, Mundo); 
                
                /* adiciona o evento chega na lef */
                novo_evento = cria_evento((Mundo->tempo + tempo_de_viajem), 1, evento->dado1, evento->dado2);
                insere_lef(lef_mundo, novo_evento);
               
                /* apaga o evento atual */
                destroi_evento(evento);
                break;

            case 8: /* EVENTO MISSAO */
                /* se a missao for concluida */
                if (evento_missao(evento->tempo, Mundo->Missoes[evento->dado1], Mundo)){
                    M_concluidas += 1;
                 
                    /* apaga o evento atual */
                    destroi_evento(evento);
                }
                else{
                    /* se ela nao for adia a missao */
                    M_adiadas += 1;
                    novo_evento =  cria_evento((Mundo->tempo + 24 * 60), 8, evento->dado1, 0);
                    insere_lef(lef_mundo, novo_evento);

                    /* apaga o evento atual */
                    destroi_evento(evento);
                }

                break;

            case 9: /* EVENTO FIM */
                evento_fim(Mundo, M_concluidas, M_adiadas);
                destroi_evento(evento);
                destroi_mundo(Mundo);
                destroi_lef(lef_mundo);
                destroi_evento(novo_evento);
                return 0;

        }
    }

}
