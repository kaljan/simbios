
#include "display.h"
#include "periph.h"

#include <math.h>

int curve[2048];

void draw_adc_buffer(void)
{
	int i;
	while (ADC_Ready == 0);
	ADC_Ready = 0;
	
	
	for (i = 0; i < 1000; i++) {
		uint16_t adcv = *(ADC_BufferPointer + i);
		double x = (((double)adcv) - 2048.0) / 4096.0;
		curve[i] = (int)(x * 224);
	}
	
	i = 0;
	while (curve[i] < 50) {
		i++;
		if (i > 999) {
			i = 0; 
			break;
		}
	}
	
	draw_curve(&curve[i], RGB565CONVERT(0, 255, 0));
}

int main(void)
{
	adc_init();

	init_display();

	draw_grid();

	LTDC_SwapBuffer();
	LTDC_ToggleLayer();

	while (1) {
	
		DMA2D_ClearBuffer();
		draw_adc_buffer();
		LTDC_SwapBuffer();
	}
}
