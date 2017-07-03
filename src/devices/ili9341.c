/*!
 --------------------------------------------------------------------
 \file		ili9341_init.c
 \author	kaljan.nothern
 \version	0.0.0
 \date
 \brief
 --------------------------------------------------------------------
 */

#include "periph.h"
#include "ili9341.h"

// Defines ----------------------------------------------------------
//! CSX PC2
#define GPIO_LCD_CSX_DOWN	GPIOC->BSRRH |= GPIO_BSRR_BS_2;
#define GPIO_LCD_CSX_UP		GPIOC->BSRRL |= GPIO_BSRR_BS_2;

//! DCX PD13
#define	GPIO_LCD_DCX_DATA	GPIOD->BSRRL |= GPIO_BSRR_BS_13;
#define	GPIO_LCD_DCX_CMD	GPIOD->BSRRH |= GPIO_BSRR_BS_13;

// Define LCD ILI9341 Commands
// Regulative Command Set
#define ILI9341_RCMD_NOP		0x00 //!< No Operation
#define ILI9341_RCMD_SWRST		0x01 //!< Software Reset
#define ILI9341_RCMD_RD_ID		0x04 //!< Read Display Identification Information
#define ILI9341_RCMD_RD_STAT	0x09 //!< Read Display Status
#define ILI9341_RCMD_RD_PM		0x0A //!< Read Display Power Mode
#define ILI9341_RCMD_RD_MADCTL	0x0B //!< Read Display MADCTL
#define ILI9341_RCMD_RD_DPF		0x0C //!< Read Display Pixel Format
#define ILI9341_RCMD_RD_DIF		0x0D //!< Read Display Image Format
#define ILI9341_RCMD_RD_DSM		0x0E //!< Read Display Signal Mode
#define ILI9341_RCMD_RD_DSDR	0x0F //!< Read Display Self-Diagnostic Result
#define ILI9341_RCMD_SLEEPIN	0x10 //!< Enter Sleep Mode
#define ILI9341_RCMD_SLEEPOUT	0x11 //!< Sleep OUT
#define ILI9341_RCMD_PMON		0x12 //!< Partial Mode ON
#define ILI9341_RCMD_NMON		0x13 //!< Normal Display Mode ON
#define ILI9341_RCMD_INVON		0x20 //!< Display Inversion OFF
#define ILI9341_RCMD_INVOFF		0x21 //!< Display Inversion ON
#define ILI9341_RCMD_GAMMA		0x26 //!< Gamma Set
#define ILI9341_RCMD_OFF		0x28 //!< Display OFF
#define ILI9341_RCMD_ON			0x29 //!< Display ON
#define ILI9341_RCMD_CASET		0x2A //!< Column Address Set
#define ILI9341_RCMD_PASET		0x2B //!< Page Address Set
#define ILI9341_RCMD_MWR		0x2C //!< Memory Write
#define ILI9341_RCMD_CSET		0x2D //!< Color SET
#define ILI9341_RCMD_MRD		0x2E //!< Memory Read
#define ILI9341_RCMD_PA			0x30 //!< Partial Area
#define ILI9341_RCMD_VSD		0x33 //!< Vertical Scrolling Definition
#define ILI9341_RCMD_TLOFF		0x34 //!< Tearing Effect Line OFF
#define ILI9341_RCMD_TLON		0x35 //!< Tearing Effect Line ON
#define ILI9341_RCMD_MAC 		0x36 //!< Memory Access Control
#define ILI9341_RCMD_VSSA		0x37 //!< Vertical Scrolling Start Address
#define ILI9341_RCMD_IDLOFF		0x38 //!< Idle Mode OFF
#define ILI9341_RCMD_IDLON		0x39 //!< Idle Mode ON
#define ILI9341_RCMD_PFSET		0x3A //!< Pixel Format Set
#define ILI9341_RCMD_WRMC		0x3C //!< Write Memory Continue
#define ILI9341_RCMD_RDMC		0x3E //!< Read Memory Continue
#define ILI9341_RCMD_STSET		0x44 //!< Set Tear Scanline
#define ILI9341_RCMD_SGET		0x45 //!< Get Scanline
#define ILI9341_RCMD_WRDB		0x51 //!< Write Display Brightness
#define ILI9341_RCMD_RDDB		0x52 //!< Read Display Brightness
#define ILI9341_RCMD_WRCTRL		0x53 //!< Write CTRL Display
#define ILI9341_RCMD_RDCTRL		0x54 //!< Read CTRL Display
#define ILI9341_RCMD_WRCABC		0x55 //!< Write Content Adaptive Brightness Control
#define ILI9341_RCMD_RDCABC		0x56 //!< Read Content Adaptive Brightness Control
#define ILI9341_RCMD_WRCMIN		0x5E //!< Write CABC Minimum Brightness
#define ILI9341_RCMD_RDCMIN		0x5F //!< Read CABC Minimum Brightness
#define ILI9341_RCMD_RDID1		0xDA //!< Read ID1
#define ILI9341_RCMD_RDID2		0xDB //!< Read ID2
#define ILI9341_RCMD_RDID3		0xDC //!< Read ID3

// Extended Command Set
#define ILI9341_ECMD_RGBISC		0xB0 //!< RGB Interface Signal Control
#define ILI9341_ECMD_FRCTN		0xB1 //!< Frame Control (In Normal Mode)
#define ILI9341_ECMD_FRCTI		0xB2 //!< Frame Control (In Idle Mode)
#define ILI9341_ECMD_FRTCP		0xB3 //!< Frame Control (In Partial Mode)
#define ILI9341_ECMD_DICT		0xB4 //!< Display Inversion Control
#define ILI9341_ECMD_BPCT		0xB5 //!< Blanking Porch Control
#define ILI9341_ECMD_DFCT		0xB6 //!< Display Function Control
#define ILI9341_ECMD_EMS		0xB7 //!< Entry Mode Set
#define ILI9341_ECMD_BC1		0xB8 //!< Backlight Control 1
#define ILI9341_ECMD_BC2		0xB9 //!< Backlight Control 2
#define ILI9341_ECMD_BC3		0xBA //!< Backlight Control 3
#define ILI9341_ECMD_BC4		0xBB //!< Backlight Control 4
#define ILI9341_ECMD_BC5		0xBC //!< Backlight Control 5
#define ILI9341_ECMD_BC6		0xBD //!< Backlight Control 6
#define ILI9341_ECMD_BC7		0xBE //!< Backlight Control 7
#define ILI9341_ECMD_BC8		0xBF //!< Backlight Control 8
#define ILI9341_ECMD_PWCT1		0xC0 //!< Power Control 1
#define ILI9341_ECMD_PWCT2		0xC1 //!< Power Control 2
#define ILI9341_ECMD_VCOM_CT1	0xC5 //!< VCOM Control 1
#define ILI9341_ECMD_VCOM_CT2	0xC7 //!< VCOM Control 2
#define ILI9341_ECMD_NVMWR		0xD0 //!< NV Memory Write
#define ILI9341_ECMD_NVMPKEY	0xD1 //!< NV Memory Protection Key
#define ILI9341_ECMD_NVMSR		0xD2 //!< NV Memory Status Read
#define ILI9341_ECMD_RDID4		0xD3 //!< Read ID4
#define ILI9341_ECMD_PGC		0xE0 //!< Positive Gamma Correction
#define ILI9341_ECMD_NGC		0xE1 //!< Negative Gamma Correction
#define ILI9341_ECMD_DGC1		0xE2 //!< Digital Gamma Control 1
#define ILI9341_ECMD_DGC2		0xE3 //!< Digital Gamma Control 2
#define ILI9341_ECMD_IC			0xF6 //!< Interface Control

#define ILI9341_ECMD_PWRA		0xCB //!< Power control A
#define ILI9341_ECMD_PWRB		0xCF //!< Power control B
#define ILI9341_ECMD_DTCA		0xE8 //!< Driver timing control A
#define ILI9341_ECMD_DTCB		0xEA //!< Driver timing control B
#define ILI9341_ECMD_PWR_SEQ	0xED //!< Power on sequence control
#define ILI9341_ECMD_E3G		0xF2 //!< Enable 3G
#define ILI9341_ECMD_PRC		0xF7 //!< Pump ratio control

#define ILI9341_CMD				0
#define ILI9341_DATA			1

// Typedefs ---------------------------------------------------------
typedef struct _t_lcd_config {
	uint8_t di;
	uint8_t data;
	uint32_t delay;
} t_lcd_config;

// Variables --------------------------------------------------------
const t_lcd_config lcd_config[] = {
	{ILI9341_CMD, 0xCA, 0},
	{ILI9341_DATA, 0xC3, 0},
	{ILI9341_DATA, 0x08, 0},
	{ILI9341_DATA, 0x50, 0},
	{ILI9341_CMD, ILI9341_ECMD_PWRB, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0xC1, 0},
	{ILI9341_DATA, 0x30, 0},
	{ILI9341_CMD, ILI9341_ECMD_PWR_SEQ, 0},
	{ILI9341_DATA, 0x64, 0},
	{ILI9341_DATA, 0x03, 0},
	{ILI9341_DATA, 0x12, 0},
	{ILI9341_DATA, 0x81, 0},
	{ILI9341_CMD, ILI9341_ECMD_DTCA, 0},
	{ILI9341_DATA, 0x85, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x78, 0},
	{ILI9341_CMD, ILI9341_ECMD_PWRA, 0},
	{ILI9341_DATA, 0x39, 0},
	{ILI9341_DATA, 0x2C, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x34, 0},
	{ILI9341_DATA, 0x02, 0},
	{ILI9341_CMD, ILI9341_ECMD_PRC, 0},
	{ILI9341_DATA, 0x20, 0},
	{ILI9341_CMD, ILI9341_ECMD_DTCB, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_CMD, ILI9341_ECMD_FRCTN, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x1B, 0},
	{ILI9341_CMD, ILI9341_ECMD_DFCT, 0},
	{ILI9341_DATA, 0x0A, 0},
	{ILI9341_DATA, 0xA2, 0},

	{ILI9341_CMD, ILI9341_ECMD_PWCT1, 0},
	{ILI9341_DATA, 0x10, 0},
	{ILI9341_CMD, ILI9341_ECMD_PWCT2, 0},
	{ILI9341_DATA, 0x10, 0},
	{ILI9341_CMD, ILI9341_ECMD_VCOM_CT1, 0},
	{ILI9341_DATA, 0x45, 0},
	{ILI9341_DATA, 0x15, 0},
	{ILI9341_CMD, ILI9341_ECMD_VCOM_CT2, 0},
	{ILI9341_DATA, 0x90, 0},
	{ILI9341_CMD, ILI9341_RCMD_MAC, 0},
	{ILI9341_DATA, 0xC8, 0},
	{ILI9341_CMD, ILI9341_ECMD_E3G, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_CMD, ILI9341_ECMD_RGBISC, 0},
	{ILI9341_DATA, 0xC2, 0},
	
	{ILI9341_CMD, ILI9341_ECMD_BPCT, 0},
	{ILI9341_DATA, LTDC_VFP, 0}, // VFP
	{ILI9341_DATA, LTDC_VBP, 0}, // VBP
	{ILI9341_DATA, LTDC_HFP, 0}, // HFP
	{ILI9341_DATA, LTDC_HBP, 0}, // HBP
    
	{ILI9341_CMD, ILI9341_ECMD_DFCT, 0},
	{ILI9341_DATA, 0x0A, 0},
	{ILI9341_DATA, 0xA7, 0},
	{ILI9341_DATA, 0x27, 0},
	{ILI9341_DATA, 0x02, 0},
	{ILI9341_CMD, ILI9341_RCMD_CASET, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0xEF, 0},
	{ILI9341_CMD, ILI9341_RCMD_PASET, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x01, 0},
	{ILI9341_DATA, 0x3F, 0},
	{ILI9341_CMD, ILI9341_ECMD_IC, 0},
	{ILI9341_DATA, 0x01, 0},
	{ILI9341_DATA, 0x20, 0},
	{ILI9341_DATA, 0x06, 1000000},
	
	{ILI9341_CMD, ILI9341_RCMD_PFSET, 0},
	{ILI9341_DATA, 0x56, 0},

	{ILI9341_CMD, ILI9341_RCMD_GAMMA, 0},
	{ILI9341_DATA, 0x08, 0},
	{ILI9341_CMD, ILI9341_ECMD_PGC, 0},
	{ILI9341_DATA, 0x0F, 0},
	{ILI9341_DATA, 0x29, 0},
	{ILI9341_DATA, 0x24, 0},
	{ILI9341_DATA, 0x0C, 0},
	{ILI9341_DATA, 0x0E, 0},
	{ILI9341_DATA, 0x09, 0},
	{ILI9341_DATA, 0x4E, 0},
	{ILI9341_DATA, 0x78, 0},
	{ILI9341_DATA, 0x3C, 0},
	{ILI9341_DATA, 0x09, 0},
	{ILI9341_DATA, 0x13, 0},
	{ILI9341_DATA, 0x05, 0},
	{ILI9341_DATA, 0x17, 0},
	{ILI9341_DATA, 0x11, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_CMD, ILI9341_ECMD_NGC, 0},
	{ILI9341_DATA, 0x00, 0},
	{ILI9341_DATA, 0x16, 0},
	{ILI9341_DATA, 0x1B, 0},
	{ILI9341_DATA, 0x04, 0},
	{ILI9341_DATA, 0x11, 0},
	{ILI9341_DATA, 0x07, 0},
	{ILI9341_DATA, 0x31, 0},
	{ILI9341_DATA, 0x33, 0},
	{ILI9341_DATA, 0x42, 0},
	{ILI9341_DATA, 0x05, 0},
	{ILI9341_DATA, 0x0C, 0},
	{ILI9341_DATA, 0x0A, 0},
	{ILI9341_DATA, 0x28, 0},
	{ILI9341_DATA, 0x2F, 0},
	{ILI9341_DATA, 0x0F, 0},
	{ILI9341_CMD, ILI9341_RCMD_SLEEPOUT, 1000000},
	{ILI9341_CMD, ILI9341_RCMD_ON, 0},
	{0xFF, 0, 0}
};

/*!
 --------------------------------------------------------------------
 \brief Отправка команды и данных в дисплей.

 \param cmd - номер команды
 \param data - указатель на массив данных
 \param ndata - количество данных в массиве data
 
 --------------------------------------------------------------------
 */
static void ILI9341_SPI_Write(char di, uint8_t data)
{
	if (di == 0) 
		GPIO_LCD_DCX_CMD
	else GPIO_LCD_DCX_DATA
	
	GPIO_LCD_CSX_DOWN
	SPI5_Poll_Data(data);
	GPIO_LCD_CSX_UP
}

/*!
 --------------------------------------------------------------------
 \brief

 --------------------------------------------------------------------
 */
void ili9341_init(void)
{
	int i;
	const t_lcd_config *lcd = &lcd_config[0];
	
	GPIOC->MODER |= GPIO_MODER_MODER2_0;	
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOC->BSRRL |= GPIO_BSRR_BS_2;	
	
	GPIOD->MODER |= GPIO_MODER_MODER13_0;	
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
		
	SPI5_Poll_Init();

	while (lcd->di != 0xFF) {
		ILI9341_SPI_Write(lcd->di, lcd->data);
		if (lcd->delay != 0) {
			i = lcd->delay;
			while (i--) {__asm volatile("nop");}
		}
		lcd++;
	}
	
	SPI5_Poll_DeInit();
	
	GPIOC->MODER &= ~GPIO_MODER_MODER2_0;
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;
		
	GPIOD->MODER &= ~GPIO_MODER_MODER13_0;	
	GPIOD->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;
	
}
