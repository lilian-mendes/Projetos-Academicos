#include <stdlib.h>
#include "Joystick.h"

joystick* joystick_create() {
	/* aloca memoria para o joystick */
	joystick *element = (joystick*) malloc (sizeof(joystick));
	if (!element) 
		return NULL;

	/* inicializa todos botoes como desativados */
	element->right = 0;
	element->left = 0;
	element->up = 0;
	element->down = 0;
	element->fire = 0;

	return element;
}

/* libera a memoria alocada para a estrutura do joystick. */
void joystick_destroy(joystick *element) { 
	free(element);
}

void joystick_reset(joystick *element) {
	/* desativa todos os botoes */
    element->right = 0;
    element->left = 0;
    element->up = 0;
    element->down = 0;
    element->fire = 0;
}

/* define o estado do botao 'esquerda' */
void joystick_left(joystick *element, int value)  { 
	element->left = value; 
}

/* define o estado do botao 'direita' */
void joystick_right(joystick *element, int value) { 
	element->right = value; 
}

/* define o estado do botao 'cima' */
void joystick_up(joystick *element, int value) {
	 element->up = value; 
}

/* define o estado do botao 'baixo' */
void joystick_down(joystick *element, int value) {
	 element->down = value;
}

/* define o estado do botao 'atira' */
void joystick_fire(joystick *element, int value) {
	 element->fire = value; 
}