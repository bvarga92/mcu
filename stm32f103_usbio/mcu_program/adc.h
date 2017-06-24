#ifndef _ADC_H_
#define _ADC_H_

	#include "stm32f1xx_hal.h"

	void adcInit(void);
	uint8_t adcRead(uint8_t channel);

#endif
