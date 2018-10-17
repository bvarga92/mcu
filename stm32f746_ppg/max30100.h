#ifndef _MAX30100_H_
#define _MAX30100_H_

	#include "stm32f7xx.h"
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_cortex.h"
	#include "stm32f7xx_hal_i2c.h"
	#include <stdbool.h>

	void max30100Init(void);
	float max30100ReadTemp(void);
	void max30100ResetFIFO(void);
	bool max30100FifoEmpty(void);
	void max30100ReadFIFO(uint16_t* ledRed, uint16_t* ledIR);

#endif
