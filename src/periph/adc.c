#include "adc.h"

#define ADC_BUF_SIZE	2048

struct adc_db_t {
	uint16_t *bufptr;
	struct adc_db_t *next;
};

struct adc_dsc_t {
	int bufsize;
	struct adc_db_t *adcdb;
};


volatile int ADC_Ready;
volatile uint16_t *ADC_BufferPointer;

uint16_t adc_buffer1[ADC_BUF_SIZE];
uint16_t adc_buffer2[ADC_BUF_SIZE];

struct adc_db_t adc_b1;
struct adc_db_t adc_b2;
struct adc_dsc_t adcdsc;


void DMA2_Stream0_IRQHandler(void)
{
	if (DMA2->LISR & DMA_LISR_TCIF0) {
		DMA2->LIFCR |=
			DMA_LIFCR_CFEIF0 |
			DMA_LIFCR_CDMEIF0 |
			DMA_LIFCR_CTEIF0 |
			DMA_LIFCR_CHTIF0 |
			DMA_LIFCR_CTCIF0;
			
		ADC_BufferPointer = adcdsc.adcdb->bufptr;
		adcdsc.adcdb = adcdsc.adcdb->next;

		DMA2_Stream0->M0AR = (uint32_t)(adcdsc.adcdb->bufptr);
		DMA2_Stream0->NDTR = ADC_BUF_SIZE;

		DMA2_Stream0->CR |=
			DMA_SxCR_EN;
		ADC_Ready = 1;
	}
}

static void prepare_adc_buffer(void)
{
	adc_b1.bufptr = adc_buffer1;
	adc_b1.next = &adc_b2;
	adc_b2.bufptr = adc_buffer2;
	adc_b2.next = &adc_b1;

	adcdsc.bufsize = ADC_BUF_SIZE;
	adcdsc.adcdb = &adc_b1;

	ADC_BufferPointer = adcdsc.adcdb->next->bufptr;
}

static void config_timer(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->PSC = 174;
	TIM1->ARR = 19;
	TIM1->CCR2 = 10;
	
	TIM1->CR1 |=
		TIM_CR1_ARPE;

	TIM1->CCMR1 |=
		TIM_CCMR1_OC2M_2 |
		TIM_CCMR1_OC2M_1;

	TIM1->CCER = 0;

	TIM1->CCER |= TIM_CCER_CC2E;
	TIM1->BDTR |= TIM_BDTR_AOE;
}

/*
 DMA2 Channel0 Stream0
*/
void adc_init(void)
{
	prepare_adc_buffer();
	config_timer();
	
	GPIOA->MODER |=
		GPIO_MODER_MODER7_0 |
		GPIO_MODER_MODER7_1;
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	ADC1->SMPR1 |=
		ADC_SMPR2_SMP7_0 |
		ADC_SMPR2_SMP7_1;
	
	ADC1->SQR3 = 7;
	
	ADC1->CR2 |= 
		ADC_CR2_EXTEN_0 | 
		ADC_CR2_EXTSEL_0 |
		ADC_CR2_DDS |
		ADC_CR2_DMA;

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	
	DMA2->LIFCR |=
		DMA_LIFCR_CFEIF0 |
		DMA_LIFCR_CDMEIF0 |
		DMA_LIFCR_CTEIF0 |
		DMA_LIFCR_CHTIF0 |
		DMA_LIFCR_CTCIF0;

	DMA2_Stream0->PAR = ADC1_BASE + 0x4C;
	DMA2_Stream0->M0AR = (uint32_t)(adcdsc.adcdb->bufptr);
	DMA2_Stream0->NDTR = ADC_BUF_SIZE;
	DMA2_Stream0->CR |=
		DMA_SxCR_MSIZE_0 |
		DMA_SxCR_PSIZE_0 |
		DMA_SxCR_MINC |
		DMA_SxCR_TCIE;

	NVIC_InterruptEnable(DMA2_Stream0_IRQn);

	DMA2_Stream0->CR |=
		DMA_SxCR_EN;

	ADC1->CR2 |=
		ADC_CR2_ADON;

	TIM1->CR1 |=
		TIM_CR1_CEN;
}

//	GPIOA->MODER |= GPIO_MODER_MODER9_1;
//	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
//	GPIOA->AFR[1] |= 0x00000010;
