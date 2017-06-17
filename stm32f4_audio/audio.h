#ifndef _AUDIO_H_
#define _AUDIO_H_

	#include "stm32f4xx_hal.h"

	extern volatile uint8_t outTransferComplete;
	extern volatile uint8_t outTransferHalfComplete;
	extern volatile uint8_t inTransferComplete;
	extern volatile uint8_t inTransferHalfComplete;

	/* 16 bites mintak kuldese az audio kimenetre */
	void audioOutInit(uint32_t samplingRate, uint16_t *buffer, uint32_t length);

	/* nyers PDM jel vetele a MEMS mikrofontol */
	void audioInInit(uint32_t samplingRate, uint16_t *buffer, uint32_t length);

#endif
