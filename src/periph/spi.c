/*!
 --------------------------------------------------------------------
 \file		spi.c
 \author	kaljan.nothern
 \version	0.0.0
 \date
 \brief
 --------------------------------------------------------------------
 */

#include "spi.h"

// Variables --------------------------------------------------------

/*!
 --------------------------------------------------------------------
 \brief	Эта функция настраивает SPI5.
 
 Запилить структуру для запуска SPI
 
 в структуре будет следующее
 
 Режим:
 - POLLING
 - INTERRUPT
 - DMA
 
 +-----+------+-----------+------+
 | Pin | Port | Function  | Type |
 +-----+------+-----------+------+
 |  19 |  PF7 | SPI5_SCK  |  AF5 |
 |  20 |  PF8 | SPI5_MISO |  AF5 |
 |  21 |  PF9 | SPI5_MOSI |  AF5 |
 +-----+------+-----------+------+
 --------------------------------------------------------------------
 */
void SPI5_Poll_Init(void)
{
	// 1. Настроим выводы для SPI
	
	GPIOF->MODER |= 
		GPIO_MODER_MODER7_1 |
		GPIO_MODER_MODER8_1 |
		GPIO_MODER_MODER9_1;
	
	GPIOF->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR7 |
		GPIO_OSPEEDER_OSPEEDR8 |
		GPIO_OSPEEDER_OSPEEDR9;
		
	GPIOF->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	
	GPIOF->AFR[0] |= 0x50000000;
	GPIOF->AFR[1] |= 0x00000055;

	if (!(RCC->APB2ENR & RCC_APB2ENR_SPI5EN)) {
		RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	}
	
	SPI5->CR1 = 0;
	SPI5->CR2 = 0;
	SPI5->I2SCFGR = 0;
	
	SPI5->CR1 |= SPI_CR1_SSM;
	SPI5->CR1 |= SPI_CR1_SSI;
	SPI5->CR1 |= SPI_CR1_MSTR;
	SPI5->CR1 |= SPI_CR1_SPE;
	
	SPI5->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1;
}

/*!
 --------------------------------------------------------------------
 \brief
 
 --------------------------------------------------------------------
 */
void SPI5_Poll_DeInit(void)
{
	GPIOF->MODER &= ~(
		GPIO_MODER_MODER7_1 |
		GPIO_MODER_MODER8_1 |
		GPIO_MODER_MODER9_1);
	
	GPIOF->OSPEEDR &= ~(
		GPIO_OSPEEDER_OSPEEDR7 |
		GPIO_OSPEEDER_OSPEEDR8 |
		GPIO_OSPEEDER_OSPEEDR9);
		
	GPIOF->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	
	GPIOF->AFR[0] &= ~0x50000000;
	GPIOF->AFR[1] &= ~0x00000055;
	
	SPI5->CR1 = 0;
	SPI5->CR2 = 0;
	SPI5->I2SCFGR = 0;

	RCC->APB2ENR &= ~RCC_APB2ENR_SPI5EN;
}

/*!
 --------------------------------------------------------------------
 \brief		Функция передачи данных по SPI в режиме polling.
 
 \param 	data - 8-битные данне которые нужно передать в SPI
 
 \return	Принятые данные из SPI

 --------------------------------------------------------------------
 */
unsigned char SPI5_Poll_Data(unsigned char data)
{
	unsigned char temp;
	while (!(SPI5->SR & SPI_SR_TXE));
	SPI5->DR = data;
	while (!(SPI5->SR & SPI_SR_RXNE));
	temp = SPI5->DR;
	return temp;
}
