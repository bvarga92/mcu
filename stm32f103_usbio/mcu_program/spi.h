#ifndef _SPI_H_
#define _SPI_H_

	#include "stm32f1xx_hal.h"

	void spiInit(void);
	void spiTransmit16Bit(uint8_t MSByte, uint8_t LSByte);

#endif
