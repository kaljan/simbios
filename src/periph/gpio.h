#ifndef GPIO_H
#define GPIO_H


#include "periph.h"


#define mLD3_On()		GPIOG->BSRRH = GPIO_BSRR_BS_13
#define mLD3_Off()		GPIOG->BSRRH = GPIO_BSRR_BS_14
#define mLD4_On()		GPIOG->BSRRL = GPIO_BSRR_BS_13
#define mLD4_Off()		GPIOG->BSRRL = GPIO_BSRR_BS_14

#define mToggleLD3() \
if (GPIOG->ODR & GPIO_ODR_ODR_13) { \
	GPIOG->BSRRH = GPIO_BSRR_BS_13; \
} else { \
	GPIOG->BSRRL = GPIO_BSRR_BS_13; \
}

#define mToggleLD4() \
if (GPIOG->ODR & GPIO_ODR_ODR_14) { \
	GPIOG->BSRRH = GPIO_BSRR_BS_14; \
} else { \
	GPIOG->BSRRL = GPIO_BSRR_BS_14; \
}


typedef enum _GPIO_State {
	LOW_STATE = 0,
	HIGH_STATE
} GPIO_State;


GPIO_State GPIO_Debounce(GPIO_TypeDef *gpio, uint32_t pinmask);
void GPIO_Init(void);


#endif // GPIO_H
