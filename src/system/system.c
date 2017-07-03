/*!
 --------------------------------------------------------------------
 \file		rcc.c
 \author	kaljan.nothern
 \version	0.0.0
 \date		13 января 2016 г., 21:38:16
 \brief
 --------------------------------------------------------------------
 */
#include "system.h"
#include <stm32f4xx.h>

// Prototypes -------------------------------------------------------
void FMC_SDRAM_Init(void);

// Defines ----------------------------------------------------------
#define PLL_M			8
#define PLL_N			336
#define PLL_P			2
#define PLL_Q			7

#define FMC_SDRAM_TRCD	2		//!< Row to column delay:
#define FMC_SDRAM_TRP 	2		//!< Row precharge delay:
#define FMC_SDRAM_TWR 	2		//!< Recovery delay:
#define FMC_SDRAM_TRC 	7		//!< Row cycle delay:
#define FMC_SDRAM_TRAS	4		//!< Self refresh time:
#define FMC_SDRAM_TXSR	7		//!< Exit Self-refresh delay:
#define FMC_SDRAM_TMRD	2		//!< Load Mode Register to Active:
#define FMC_SDRAM_SDRTR 1293

// Variables --------------------------------------------------------

/*!
 --------------------------------------------------------------------
 \brief Системная инициализация

 --------------------------------------------------------------------
 */
void SystemInit(void)
{	
// Если нужно включаем FPU
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= 0x00F00000;
#endif
	
	// Сброс настроек системного генератора.
	RCC->CR  |= RCC_CR_HSION;

	RCC->CFGR = 0;

	RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_PLLON | RCC_CR_CSSON);

	RCC->PLLCFGR = 0x24003010;

	RCC->CR &= ~(RCC_CR_HSEBYP);

	RCC->CIR = 0;// Запретить все прерывания.

	// Запускаем внешний генератор и дожидаемся его готовности
	RCC->CR |= RCC_CR_HSEON;

	while (!(RCC->CR & RCC_CR_HSERDY)) {}

	// Настройка PWR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	
	// Настройка системного генератора.

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

	RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
		(RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

	RCC->CR |= RCC_CR_PLLON;

	while(!(RCC->CR & RCC_CR_PLLRDY)) {}

	// Этот кусок нужно перенести
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
	
#ifdef VECT_TAB_SRAM
	SCB->VTOR = SRAM_BASE;
#else
	SCB->VTOR = FLASH_BASE;
#endif
	SCB->AIRCR = 0x05FA0700;

	FMC_SDRAM_Init();

}

/*!
 --------------------------------------------------------------------
 \brief Инициализация SMC для SDRAM.

 Весь процес инициализации взят из "stm32f4xx Reference manual.pdf"
 37.7.3 SDRAM controller functional description. SDRAM initialization.
 стр 1651.

 Так как SDRAM подключен к банку 2, то его адрес будет 0xD0000000.
 Объём памяти 8 МБ. (ISSI IS42S16400J-7TL).

 --------------------------------------------------------------------
 */
void FMC_SDRAM_Init(void)
{
	int i = 1000000;
	// Сначала нужно разрешить тактирования лапок портов
	RCC->AHB1ENR |=
		RCC_AHB1ENR_GPIOAEN |
		RCC_AHB1ENR_GPIOBEN |
		RCC_AHB1ENR_GPIOCEN |
		RCC_AHB1ENR_GPIODEN |
		RCC_AHB1ENR_GPIOEEN |
		RCC_AHB1ENR_GPIOFEN |
		RCC_AHB1ENR_GPIOGEN;

	// Настройка GPIO
	GPIOB->MODER |= 0x00002800;
	GPIOB->OTYPER |= 0x00000000;
	GPIOB->OSPEEDR |= 0x00003C00;
	GPIOB->AFR[0] |= 0x0CC00000;

	GPIOC->MODER |= 0x00000002;
	GPIOC->OTYPER |= 0x00000000;
	GPIOC->OSPEEDR |= 0x00000003;
	GPIOC->AFR[0] |= 0x0000000C;

	GPIOD->MODER |= 0xA02A000A;
	GPIOD->OTYPER |= 0x00000000;
	GPIOD->OSPEEDR |= 0xF03F000F;
	GPIOD->AFR[0] |= 0x000000CC;
	GPIOD->AFR[1] |= 0xCC000CCC;

	GPIOE->MODER |= 0xAAAA800A;
	GPIOE->OTYPER |= 0x00000000;
	GPIOE->OSPEEDR |= 0xFFFFC00F;
	GPIOE->AFR[0] |= 0xC00000CC;
	GPIOE->AFR[1] |= 0xCCCCCCCC;

	GPIOF->MODER |= 0xAA800AAA;
	GPIOF->OTYPER |= 0x00000000;
	GPIOF->OSPEEDR |= 0xFFC00FFF;
	GPIOF->AFR[0] |= 0x00CCCCCC;
	GPIOF->AFR[1] |= 0xCCCCC000;

	GPIOG->MODER |= 0x80020A0A;
	GPIOG->OTYPER |= 0x00000000;
	GPIOG->OSPEEDR |= 0xF0030F0F;
	GPIOG->AFR[0] |= 0x00CC00CC;
	GPIOG->AFR[1] |= 0xC000000C;

	// Разрешить тактирование FMC
	RCC->AHB3ENR |=
		RCC_AHB3ENR_FMCEN;

	// Настройка FMC
	// Step. 1
	FMC_Bank5_6->SDCR[0] =
		FMC_SDCR1_RPIPE_1 |
		FMC_SDCR1_RBURST |
		FMC_SDCR1_SDCLK_1;

	FMC_Bank5_6->SDCR[1] =
		FMC_SDCR2_CAS_1 |
		FMC_SDCR2_CAS_0 |
		FMC_SDCR2_NB |
		FMC_SDCR2_MWID_0 |
		FMC_SDCR2_NR_0;

	// Step. 2
	FMC_Bank5_6->SDTR[0] =
		((FMC_SDRAM_TRP & 0x0000000F) << 20) |
		((FMC_SDRAM_TRC & 0x0000000F) << 12);

	FMC_Bank5_6->SDTR[1] =
		((FMC_SDRAM_TRCD & 0x0000000F) << 24) |
		((FMC_SDRAM_TWR & 0x0000000F) << 16) |
		((FMC_SDRAM_TRAS & 0x0000000F) << 8) |
		((FMC_SDRAM_TXSR & 0x0000000F) << 4) |
		(FMC_SDRAM_TMRD & 0x0000000F);

	// Step. 3
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	FMC_Bank5_6->SDCMR =
		FMC_SDCMR_MODE_0 |
		FMC_SDCMR_CTB2 |
		FMC_SDCMR_NRFS_0;

	// Step. 4
	while (i--) {__asm volatile("nop");}

	// Step. 5
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	FMC_Bank5_6->SDCMR =
		FMC_SDCMR_MODE_1 |
		FMC_SDCMR_CTB2 |
		FMC_SDCMR_NRFS_0;

	// Step. 6
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	FMC_Bank5_6->SDCMR =
		FMC_SDCMR_MODE_0 |
		FMC_SDCMR_MODE_1 |
		FMC_SDCMR_CTB2 |
		FMC_SDCMR_NRFS_3;

	// Step. 7
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	FMC_Bank5_6->SDCMR =
		FMC_SDCMR_MODE_2 |
		FMC_SDCMR_CTB2 |
		FMC_SDCMR_NRFS_0 |
		(0x00000231 << 9);

	// Step. 8
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	FMC_Bank5_6->SDRTR = (FMC_SDRAM_SDRTR << 1);

	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
}
