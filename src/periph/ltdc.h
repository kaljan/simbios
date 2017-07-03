#ifndef LTDC_H
#define LTDC_H

#include "periph.h"

#define LTDC_HSYNC		4
#define LTDC_HBP		3
#define LTDC_HADR		240
#define LTDC_HFP		3
#define LTDC_VSYNC		4
#define LTDC_VBP		2
#define LTDC_VADR		320
#define LTDC_VFP		4

extern volatile uint16_t *VB_Addr;

void LTDC_Init(void);
void LTDC_SwapBuffer(void);
void LTDC_ToggleLayer(void);

int DMA2D_FillBuffer(int w, int h, uint16_t *fbaddr, uint16_t color);
int DMA2D_ClearBuffer(void);

#endif
