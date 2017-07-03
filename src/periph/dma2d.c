/*!
 --------------------------------------------------------------------
 \file		dma2d.c
 \author	kaljan.nothern
 \version	0.0.0
 \date		28.окт.2016
 \brief		
 --------------------------------------------------------------------
 */

#include "ltdc.h"
// Prototypes -------------------------------------------------------
// Defines ----------------------------------------------------------
// Variables --------------------------------------------------------
// Code -------------------------------------------------------------

void DMA2D_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	
	DMA2D->CR |= DMA2D_CR_MODE;
	DMA2D->OPFCCR |= 0x02;
}

/*
 --------------------------------------------------------------------
 Register-to-memory

 The register-to-memory mode is used to fill a user defined area with 
 a predefined color.
 
 The color format is set in the DMA2D_OPFCCR.
 
 The DMA2D does not perform any data fetching from any source. It just 
 writes the color defined in the DMA2D_OCOLR register to the area located 
 at the address pointed by the DMA2D_OMAR and defined in the DMA2D_NLR 
 and DMA2D_OOR.
 --------------------------------------------------------------------
 */
int DMA2D_FillSpace(int x, int y, int w, int h, uint16_t *maddr, uint16_t color)
{
	int timeout = 1000000;
	DMA2D->NLR = (((uint16_t)w) << 16) | ((uint16_t)h);
	DMA2D->OOR = x;
	DMA2D->OMAR = (uint32_t)(maddr);
	DMA2D->IFCR |= 0x3F;
	DMA2D->OCOLR = color;
	DMA2D->CR |= DMA2D_CR_START;
	while(!(DMA2D->ISR & DMA2D_ISR_TCIF)) {
		timeout--;
		if (timeout < 1) {
			break;
		}
	}
	
	if (timeout < 1) {
		return -1;
	}
	return 0;
}


int DMA2D_FillBuffer(int w, int h, uint16_t *fbaddr, uint16_t color)
{
	int timeout = 1000000;
	
	if (fbaddr == 0) {
		return -1;
	}
	
	DMA2D->NLR = (((uint16_t)w) << 16) | ((uint16_t)h);
	DMA2D->OOR = 0;
	DMA2D->OMAR = (uint32_t)(fbaddr);
	DMA2D->IFCR |= 0x3F;
	DMA2D->OCOLR = color;
	DMA2D->CR |= DMA2D_CR_START;
	
	while(!(DMA2D->ISR & DMA2D_ISR_TCIF)) {
		timeout--;
		if (timeout < 1) {
			break;
		}
	}
	
	if (timeout < 1) {
		return -1;
	}
	return 0;
}

int DMA2D_ClearBuffer(void)
{
	return DMA2D_FillBuffer(320, 240, (uint16_t *)VB_Addr, 0);
}
