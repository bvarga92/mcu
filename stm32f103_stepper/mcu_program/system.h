#ifndef _SYSTEM_H_
#define _SYSTEM_H_

	#include "stm32f1xx_hal.h"

	void clockInit(void);
	uint8_t jumperIsOn(void);
	void ledInit(void);
	void ledOff(void);
	void ledOn(void);
	void ledToggle(void);

#endif
