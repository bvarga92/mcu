#ifndef _UART_H_
#define _UART_H_

	#include "stm32f1xx_hal.h"

	/* ennel eggyel kevesebb elemet tud tarolni a veteli FIFO */
	#define FIFO_SIZE 64

	/* megszakitaskezelo */
	void USART3_IRQHandler(void);

	/* inicializalas (UART3) */
	void uartInit(void);

	/* a veteli FIFO szokasos fuggvenyei */
	void uartFifoPut(uint8_t data);
	uint8_t uartFifoGet(void);
	uint8_t uartFIFOIsEmpty(void);
	uint8_t uartFIFOIsFull(void);

#endif
