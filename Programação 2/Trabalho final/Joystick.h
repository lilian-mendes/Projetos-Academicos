#ifndef __JOYSTICK__ 
#define __JOYSTICK__ 

/* estrutura do joystick */
typedef struct {
    unsigned char right;  /* estado do botao direito (1 = pressionado) */ 
    unsigned char left;   /* estado do botao esquerdo (1 = pressionado) */
    unsigned char up;     /* estado do botao cima (1 = pressionado) */
    unsigned char down;   /* estado do botao baixo (1 = pressionado) */
    unsigned char fire;   /* estado do botao de tiro (1 = pressionado) */
} joystick;

/* prototipos das funcoes */
joystick* joystick_create();
void joystick_destroy(joystick *element);
void joystick_reset(joystick *element);
void joystick_left(joystick *element, int value);   
void joystick_right(joystick *element, int value);  
void joystick_up(joystick *element, int value);     
void joystick_down(joystick *element, int value); 
void joystick_fire(joystick *element, int value); 

#endif