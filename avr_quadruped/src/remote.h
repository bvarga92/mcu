#ifndef _REMOTE_H_
#define _REMOTE_H_

	#include <avr/io.h>

	#define BTN1    1
	#define BTN2    2
	#define BTN3    4
	#define BTN4    8

	void remoteInit(void);
	void ledOn(void);
	void ledOff(void);
	uint8_t getButtons(void);

#endif
