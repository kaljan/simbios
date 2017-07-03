#ifndef PERIPH_H
#define PERIPH_H

#include <stm32f4xx.h>
#include <stdint.h>
#include <stdlib.h>

#include "gpio.h"
#include "nvic.h"
#include "ltdc.h"
#include "spi.h"
#include "adc.h"

#define _PI (double)(3.1415926535897932384626433832795)
#define mRadToDeg(rad) ((((double)rad) * (double)(180)) / _PI)
#define mDegToRad(deg) ((((double)deg) * _PI) / (double)(180))


#endif // PERIPH_H
