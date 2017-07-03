#ifndef SPI_H
#define SPI_H

#include "periph.h"

void SPI5_Poll_Init(void);
void SPI5_Poll_DeInit(void);
unsigned char SPI5_Poll_Data(unsigned char data);

#endif // SPI_H
