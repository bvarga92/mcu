#ifndef _WS2812_H_
#define _WS2812_H_

	#include "stm32f1xx_hal.h"

	#define NUM_LEDS     24
	#define RESET_LEN    45
	#define BUFFER_LEN   (24*NUM_LEDS+RESET_LEN)

	typedef struct{
		uint8_t r, g, b;
	} rgb_t;

	void DMA1_Channel3_IRQHandler(void);
	void ws2812Init(void);
	void ws2812SetLED(uint8_t ledNum, rgb_t *color);
	void hsv2rgb(uint16_t h, uint8_t s, uint8_t v, rgb_t *color);

#endif
