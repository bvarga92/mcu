#ifndef _BUZZER_BUTTON_H_
#define _BUZZER_BUTTON_H_

	#include "stm32f1xx_hal.h"

	void btnBzrInit(void);
	uint8_t btnRead(void);
	void buzzerOn(void);
	void buzzerOff(void);
	void buzzerBeep(void);

#endif
