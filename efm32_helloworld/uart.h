#ifndef _UART_H_
#define _UART_H_

	#include "stdbool.h"
	#include "stdint.h"

	extern volatile bool uartReceived;
	extern volatile uint8_t uartRxChar;

	void uartInit(void);
	void uartPrint(char* str);

#endif
