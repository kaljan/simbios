
#include "ltdc.h"

extern void DMA2D_Init(void);

#define L1FB1_ADDR	(uint16_t*) 0xD0000000 // end: 0xD00257FF
#define L1FB2_ADDR	(uint16_t*) 0xD0025800 // end: 0xD004AFFF
#define L1FB3_ADDR	(uint16_t*) 0xD004B000 // end: 0xD00707FF
#define L2FB1_ADDR	(uint16_t*) 0xD0070800 // end: 0xD0095FFF
#define L2FB2_ADDR	(uint16_t*) 0xD0096000 // end: 0xD00BB7FF
#define L2FB3_ADDR	(uint16_t*) 0xD00BB800 // end: 0xD00E0FFF

#define PLLSAI_N		231		//!< PLLSAI division factor for VCO
#define PLLSAI_R		5		//!< PLLSAI division factor for LCD clock
#define PLLSAI_Q		2		//!< PLLSAI division factor for SAI1 clock

volatile uint16_t *VB_Addr;

struct vb_t {
	uint16_t* vb;
	struct vb_t* next;
};

struct vb_layer_t {
	struct vb_t* vb; /* Это указатель на видео память, из которой 
	LTDC выводит изображение на экран. соответсвтенно vb->next 
	указывает на буфер в который идёт запись*/
	LTDC_Layer_TypeDef* layer;
	struct vb_layer_t* next;
};


static struct vb_t l1vb1;
static struct vb_t l1vb2;
static struct vb_t l1vb3;
static struct vb_t l2vb1;
static struct vb_t l2vb2;
static struct vb_t l2vb3;
static struct vb_layer_t l1;
static struct vb_layer_t l2;
static struct vb_layer_t* l;

volatile int reload;

void LTDC_IRQHandler(void)
{
	if (LTDC->ISR & LTDC_ISR_RRIF) {
		LTDC->ICR = LTDC_ICR_CRRIF;
		reload = 1;
	}
}

/*!
 --------------------------------------------------------------------
 \brief 
 
 PG11  LTDC_B3     AF14 
 PG12  LTDC_B4      AF9 
 PA3   LTDC_B5     AF14 
 PB8   LTDC_B6     AF14 
 PB9   LTDC_B7     AF14 
 PA6   LTDC_G2     AF14 
 PG10  LTDC_G3      AF9 
 PB10  LTDC_G4     AF14 
 PB11  LTDC_G5     AF14 
 PV7   LTDC_G6     AF14 
 PD3   LTDC_G7     AF14 
 PB0   LTDC_R3      AF9 
 PA11  LTDC_R4     AF14 
 PA12  LTDC_R5     AF14 
 PB1   LTDC_R6      AF9 
 PG6   LTDC_R7     AF14 
 PF10  LTDC_EN     AF14 
 PG7   LTDC_DTCLK  AF14 
 PC6   LTDC_HSYNC  AF14 
 PA4   LTDC_VSYNC  AF14 
 
 --------------------------------------------------------------------
 */
static void gpio_config(void)
{
	GPIOA->MODER |= 
		GPIO_MODER_MODER3_1 |
		GPIO_MODER_MODER4_1 |
		GPIO_MODER_MODER6_1 |
		GPIO_MODER_MODER11_1 |
		GPIO_MODER_MODER12_1;
	
	GPIOA->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR3 |
		GPIO_OSPEEDER_OSPEEDR4 |
		GPIO_OSPEEDER_OSPEEDR6 |
		GPIO_OSPEEDER_OSPEEDR11 |
		GPIO_OSPEEDER_OSPEEDR12;
	
	GPIOA->AFR[0] |= 0x0E0EE000;
	GPIOA->AFR[1] |= 0x000EE000;
	
	GPIOB->MODER |= 
		GPIO_MODER_MODER0_1 |
		GPIO_MODER_MODER1_1 |
		GPIO_MODER_MODER8_1 |
		GPIO_MODER_MODER9_1 |
		GPIO_MODER_MODER10_1 |
		GPIO_MODER_MODER11_1;
	
	GPIOB->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR0 |
		GPIO_OSPEEDER_OSPEEDR1 |
		GPIO_OSPEEDER_OSPEEDR8 |
		GPIO_OSPEEDER_OSPEEDR9 |
		GPIO_OSPEEDER_OSPEEDR10 |
		GPIO_OSPEEDER_OSPEEDR11;
	
	GPIOB->AFR[0] |= 0x00000099;
	GPIOB->AFR[1] |= 0x0000EEEE;
	
	GPIOC->MODER |= 
		GPIO_MODER_MODER6_1 |
		GPIO_MODER_MODER7_1;

	GPIOC->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR6 |
		GPIO_OSPEEDER_OSPEEDR7;
	
	GPIOC->BSRRL |= GPIO_BSRR_BS_10;
	
	GPIOC->AFR[0] |= 0xEE000000;
	
	GPIOD->MODER |= 
		GPIO_MODER_MODER3_1;
	
	GPIOD->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR3;
	
	GPIOD->BSRRL |= GPIO_BSRR_BS_6;
	
	GPIOD->AFR[0] |= 0x0000E000;
	
	GPIOF->MODER |= 
		GPIO_MODER_MODER10_1;
	
	GPIOF->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR10;
	
	GPIOF->AFR[1] |= 0x00000E00;
	
	GPIOG->MODER |= 
		GPIO_MODER_MODER6_1 |
		GPIO_MODER_MODER7_1 |
		GPIO_MODER_MODER10_1 |
		GPIO_MODER_MODER11_1 |
		GPIO_MODER_MODER12_1;
	
	GPIOG->OSPEEDR |= 
		GPIO_OSPEEDER_OSPEEDR6 |
		GPIO_OSPEEDER_OSPEEDR7 |
		GPIO_OSPEEDER_OSPEEDR10 |
		GPIO_OSPEEDER_OSPEEDR11 |
		GPIO_OSPEEDER_OSPEEDR12;
	
	GPIOG->AFR[0] |= 0xEE000000;
	GPIOG->AFR[1] |= 0x0009E900;	
}

static void clock_config(void)
{
	RCC->PLLSAICFGR = 
		((0x00000007 & PLLSAI_R) << 28) |
		((0x0000000F & PLLSAI_Q) << 24) |
		((0x0000001FF & PLLSAI_N) << 6);
	
	RCC->DCKCFGR = 0x00020000;
	
	RCC->CR |= RCC_CR_PLLSAION;
	
	while (!(RCC->CR & RCC_CR_PLLSAIRDY));
	
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;	
}

static void layer_config(LTDC_Layer_TypeDef* layer, uint16_t* fbaddr)
{
	layer->WHPCR =
		((LTDC_HSYNC + LTDC_HBP + LTDC_HADR - 1) << 16) |
		(LTDC_HSYNC + LTDC_HBP);

	layer->WVPCR =
		((LTDC_VSYNC + LTDC_VBP + LTDC_VADR - 1) << 16) |
		(LTDC_VSYNC + LTDC_VBP);

	layer->PFCR = 0x02;

	layer->DCCR = 0x00000000;

	layer->CFBAR = (uint32_t)(fbaddr);

	layer->CACR = 0xFF;

	layer->CFBLR = ((LTDC_HADR * 2) << 16) | (LTDC_HADR * 2);

	layer->CFBLNR = LTDC_VADR;

	layer->CR |= LTDC_LxCR_LEN | LTDC_LxCR_COLKEN;

}

static int vb_init(void)
{
	reload = 0;
	
	l1vb1.next = &l1vb2;
	l1vb2.next = &l1vb3;
	l1vb3.next = &l1vb1;
	l2vb1.next = &l2vb2;
	l2vb2.next = &l2vb3;
	l2vb3.next = &l2vb1;
	
	l1.vb = &l1vb1;
	l1.layer = LTDC_Layer1;
	l1.next = &l2;
	
	l2 .vb = &l2vb1;
	l2.layer = LTDC_Layer2;
	l2.next = &l1;
	
	l = &l1;
	
	l1vb1.vb = L1FB1_ADDR;
	l1vb2.vb = L1FB2_ADDR;
	l1vb3.vb = L1FB3_ADDR;
	l2vb1.vb = L2FB1_ADDR;
	l2vb2.vb = L2FB2_ADDR;
	l2vb3.vb = L2FB3_ADDR;
	
	VB_Addr = l1.vb->next->vb;

	DMA2D_Init();
	DMA2D_FillBuffer(320, 240, L1FB1_ADDR, 0);
	DMA2D_FillBuffer(320, 240, L1FB2_ADDR, 0);
	DMA2D_FillBuffer(320, 240, L1FB3_ADDR, 0);
	DMA2D_FillBuffer(320, 240, L2FB1_ADDR, 0);
	DMA2D_FillBuffer(320, 240, L2FB2_ADDR, 0);
	DMA2D_FillBuffer(320, 240, L2FB3_ADDR, 0);

	return 0;	
}

static void ltdc_config(void)
{
	vb_init();
	
	LTDC->SSCR = ((LTDC_HSYNC - 1) << 16) | LTDC_VSYNC;

	LTDC->BPCR = ((LTDC_HSYNC + LTDC_HBP - 1) << 16) |
		(LTDC_VSYNC + LTDC_VBP - 1);

	LTDC->AWCR = ((LTDC_HSYNC + LTDC_HBP + LTDC_HADR - 1) << 16) |
		(LTDC_VSYNC + LTDC_VBP + LTDC_VADR - 1);

	LTDC->TWCR = ((LTDC_HSYNC + LTDC_HBP + LTDC_HADR + LTDC_HFP - 1) << 16) |
		(LTDC_VSYNC + LTDC_VBP + LTDC_VADR + LTDC_VFP - 1);
	
	layer_config(l1.layer, L1FB1_ADDR);
	layer_config(l2.layer, L2FB1_ADDR);

	LTDC->IER |= LTDC_IER_RRIE;

	NVIC_InterruptEnable(LTDC_IRQn);

	LTDC->SRCR |= LTDC_SRCR_VBR;

	LTDC->GCR |= LTDC_GCR_LTDCEN;
}

void LTDC_Init(void)
{
	gpio_config();
	clock_config();
	ltdc_config();
}

void LTDC_SwapBuffer(void)
{
	while (reload == 0);
	reload = 0;
	l->vb = l->vb->next;
	l->layer->CFBAR = (uint32_t)l->vb->vb;
	LTDC->SRCR |= LTDC_SRCR_VBR;
	VB_Addr = l->vb->next->vb;
}

void LTDC_ToggleLayer(void)
{
	l = l->next;
	VB_Addr = l->vb->next->vb;
}
