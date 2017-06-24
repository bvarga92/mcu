#ifndef _LCD_H_
#define _LCD_H_

	#include "stm32f1xx_hal.h"

	#define LCD_PIN_D4    GPIO_PIN_7
	#define LCD_PIN_D5    GPIO_PIN_6
	#define LCD_PIN_D6    GPIO_PIN_5
	#define LCD_PIN_D7    GPIO_PIN_10
	#define LCD_PIN_E     GPIO_PIN_8
	#define LCD_PIN_RS    GPIO_PIN_9

	void lcdInit(void);
	void lcdCursorGoto(uint8_t line, uint8_t pos);
	void lcdPutString(char* str, uint8_t len);

#endif
