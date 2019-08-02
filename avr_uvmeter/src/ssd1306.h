#ifndef _SSD_1306_H_
#define _SSD_1306_H_

	#include <stdint.h>

	void displayInit(void);
	void displayClear(void);
	uint8_t displayPrintText(char* str, uint8_t x, uint8_t y);
	uint8_t displayPrintInteger(int32_t n, uint8_t digits, uint8_t x, uint8_t y);

#endif
