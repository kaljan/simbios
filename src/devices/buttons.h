#ifndef BUTTONS_H
#define BUTTONS_H


#include "periph.h"


typedef int (*button_callback)(void);

typedef enum _button_evt {
	NONE = 0,
	PRESSED,
	HOLD
} button_evt;

typedef struct _din_dsc_t {
	GPIO_TypeDef *gpio;
	uint32_t pinmask;
	GPIO_State dstate;
	int hold_cnt;
} din_dsc_t;

struct din_list_t {
	din_dsc_t *din;
	struct din_list_t *next;
};

typedef struct _button_dsc_t {
	GPIO_TypeDef *gpio;
	uint32_t pinmask;
	GPIO_State button_event;
	GPIO_Event button_state;
	int hold_cnt;
	button_callback callback;
} button_dsc_t;

#endif // BUTTONS_H
