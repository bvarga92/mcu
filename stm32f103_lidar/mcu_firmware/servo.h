#ifndef _SERVO_H_
#define _SERVO_H_

	#include "stm32f1xx_hal.h"

	void servoInit(uint16_t horz, uint16_t vert);
	void servoSet(uint16_t horz, uint16_t vert);

#endif
