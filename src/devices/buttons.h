#ifndef BUTTONS_H
#define BUTTONS_H


#include "periph.h"


#define BUTTON_HOLD_MAX	100


typedef enum _button_signal {
	NONE = 0,
	PRESSED,
	HOLD
} button_signal;

typedef enum _button_function {
	BTN_MENU = 0,
	BTN_UP,
	BTN_DOWN,
	BTN_EXIT
} button_function;

typedef struct _button_dsc_t {
	GPIO_TypeDef *gpio;
	uint32_t pinmask;
	GPIO_State state;	// Состояние порта
	int hold_cnt;
	button_signal signal;	// Сигнал от кнопки
} button_dsc_t;

struct button_list {
	button_dsc_t *button;
	struct button_list *next;
};

#endif // BUTTONS_H
