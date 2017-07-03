
#include "nvic.h"

int NVIC_InterruptEnable(IRQn_Type irqn)
{
	int n = (int) irqn;
	int nreg = 0;
	
	if ((n < 0) || (n > 97)) {
		return -1;
	}
	
	nreg = irqn / 32;
	n = irqn - (nreg * 32);

	NVIC->ISER[nreg] = ((uint32_t)1) << n;

	return 0;
}
