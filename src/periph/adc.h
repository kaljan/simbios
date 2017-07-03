#ifndef ADC_H
#define ADC_H

#include "periph.h"

extern volatile int ADC_Ready;
extern volatile uint16_t *ADC_BufferPointer;


void adc_init(void);

#endif // ADC_H
