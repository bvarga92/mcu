#ifndef _BOARD_H_
#define _BOARD_H_

	#include "stdint.h"

	void boardInit(void);
	void ledOn(uint8_t n);
	void ledOff(uint8_t n);
	void ledToggle(uint8_t n);
	uint8_t ledGet(uint8_t n);
	uint8_t pbGet(uint8_t n);

#endif
