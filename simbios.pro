
CONFIG -= qt

DEFINES += \
	STM32F4XX \
	VECT_TAB_SRAM \
	STM32F429_439xx \
	__FPU_PRESENT=1 \
	__FPU_USED=1 \
	ARM_MATH_CM4

INCLUDEPATH += \
	D:/Keil/ARM/Inc/ST/STM32F4xx/ \
	D:/Keil/ARM/CMSIS/Include/ \
	src/ \
	src/devices \
	src/display \
	src/periph \
	src/system

SOURCES += \
    src/main.c \
    src/devices/ili9341.c \
    src/display/icons.c \
    src/display/fonts.c \
    src/display/display.c \
    src/periph/spi.c \
    src/periph/nvic.c \
    src/periph/ltdc.c \
    src/periph/dma2d.c \
    src/system/system_irq.c \
    src/system/system.c \
    src/periph/adc.c \
    src/fft/fft_tables.c \
    src/fft/arm_sqrt_q15.c \
    src/fft/arm_max_q15.c \
    src/fft/arm_cmplx_mag_q15.c \
    src/fft/arm_cfft_radix4_q15.c \
    src/fft/arm_cfft_radix4_init_q15.c \
    src/fft/arm_bitreversal.c \
    src/periph/gpio.c

HEADERS += \
    src/devices/ili9341.h \
    src/display/display.h \
    src/periph/spi.h \
    src/periph/periph.h \
    src/periph/nvic.h \
    src/periph/ltdc.h \
    src/system/system.h \
    src/periph/adc.h \
    src/fft/fft.h \
    src/periph/gpio.h
