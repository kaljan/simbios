#include "gpio.h"

#define GPIO_DEBOUNCE_INIT	1024
#define GPIO_DEBOUNCE_MAX	1536
#define GPIO_DEBOUNCE_MIN	512

typedef enum _din_state {
	PRESSED = 0,
	RELEASED
} din_state;

typedef enum _din_event {
	NONE = 0,
	CLICKED,
	DOUBLE_CLICK,
	HOLD
} din_event;

typedef struct _din_dsc_t {
	GPIO_TypeDef *gpio;
	uint32_t pinmask;
	din_state dstate;
	int hold_cnt;
} din_dsc_t;

struct din_list_t {
	din_dsc_t *din;
	struct din_list_t *next;
};

typedef enum _logic_event {
	HIGH_TO_LOW,
	LOW_TO_HIGH
} logic_event;

// Механиза обработки дребезга
static din_state din_debounce(GPIO_TypeDef *gpio, uint32_t pinmask)
{
	int debounce_cnt = GPIO_DEBOUNCE_INIT;

	while (1) {
		if (gpio->IDR & pinmask) {
			debounce_cnt++;
		} else {
			debounce_cnt--;
		}

		if (debounce_cnt > GPIO_DEBOUNCE_MAX) {
			return RELEASED;
		}

		if (debounce_cnt < GPIO_DEBOUNCE_MIN) {
			return PRESSED;
		}
	}
}

static int din_init(din_dsc_t *din, GPIO_TypeDef *gpio, int pin)
{
	if (din == 0) {
		return -1;
	}
	
	if (gpio == 0) {
		return -1;
	}
	
	if (pin < 0 || pin > 31) {
		return -1;
	}
	
	din->gpio = gpio;
	din->pinmask = ((uint32_t)1) << pin;
	din->dstate = RELEASED;
	din->hold_cnt = 0;
	
	return 0;
}

// Обработка нажатия
static int din_handle(din_dsc_t *din)
{
	din_state state;
	if (din == 0) {
		return -1;
	}
	
	state = din_debounce(din->gpio, din->pinmask);
	
	if ((din->dstate == RELEASED) && (state == PRESSED)) {
		din->dstate = state;
	} else if ((din->dstate == PRESSED) && (state == RELEASED)) {
		din->dstate = state;
	}
	
	return 0;
}

void GPIO_Init(void)
{
	GPIOG->MODER |=
		GPIO_MODER_MODER13_0 |
		GPIO_MODER_MODER14_0;

	GPIOG->OSPEEDR |=
		GPIO_OSPEEDER_OSPEEDR13 |
		GPIO_OSPEEDER_OSPEEDR14;

	GPIOG->BSRRH =
		GPIO_BSRR_BS_13 | 
		GPIO_BSRR_BS_14;
}

