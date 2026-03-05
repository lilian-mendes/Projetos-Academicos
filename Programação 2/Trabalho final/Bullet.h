#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 5    /* Velocidade de movimento da bala */
#define BULLET_WIDTH 10  /* largura da bala */
#define BULLET_HEIGHT 5   /* altura da bala */

/* estrutura de uma bala */
typedef struct bullet {
	unsigned short x;          /* posicao no eixo X */
	unsigned short y;          /* posicao no eixo Y */
	unsigned char trajectory;  /* trajetoria da bala (0 = esquerda, 1 = direita) */
    unsigned char active;      /* flag para indicar se a bala esta ativa (1) ou deve ser removida (0) */
	unsigned char type;        /* tipo da bala (0 = normal, 1 = especial) */
	
	struct bullet *next;       /* ponteiro para a proxima bala em uma lista encadeada */
} bullet;

/* prototipos das funcoes */
bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next);
void bullet_move(bullet *element);
void bullet_destroy(bullet *element);

#endif