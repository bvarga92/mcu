#ifndef _DISCOVERYBOARD_H_
#define _DISCOVERYBOARD_H_

	#include "stm32f4xx_hal.h"

	/* makrok a kulonbozo szinu LED-ekhez */
	#define LED_GREEN   (uint16_t)(1<<12)
	#define LED_ORANGE  (uint16_t)(1<<13)
	#define LED_RED     (uint16_t)(1<<14)
	#define LED_BLUE    (uint16_t)(1<<15)
	#define LED_ALL     (uint16_t)(0xf000)

	/* orajelek beallitasa (168 MHz, 42 MHz APB1, 84 MHz APB2) + 1 ms SysTick */
	void configSystemClock(void);

	/* a LED-eket kezelo fuggvenyek (PD12...15) */
	void ledInit(void);
	void ledOn(uint16_t led);
	void ledOff(uint16_t led);
	void ledToggle(uint16_t led);

	/* a kek nyomogombot kezelo fuggvenyek (PA0) */
	void buttonInit(void);
	uint8_t buttonRead(void);

#endif
