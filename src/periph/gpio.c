#include "gpio.h"

#define GPIO_DEBOUNCE_INIT	1024
#define GPIO_DEBOUNCE_MAX	1536
#define GPIO_DEBOUNCE_MIN	512

// Механиза обработки дребезга
GPIO_State GPIO_Debounce(GPIO_TypeDef *gpio, uint32_t pinmask)
{
	int debounce_cnt = GPIO_DEBOUNCE_INIT;

	while (1) {
		if (gpio->IDR & pinmask) {
			debounce_cnt++;
		} else {
			debounce_cnt--;
		}

		if (debounce_cnt > GPIO_DEBOUNCE_MAX) {
			return GPIO_HIGH;
		}

		if (debounce_cnt < GPIO_DEBOUNCE_MIN) {
			return GPIO_LOW;
		}
	}
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
