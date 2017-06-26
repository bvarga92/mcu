#ifndef _USB_H_
#define _USB_H_

	#include "stm32f1xx_hal.h"

	typedef struct{
		uint8_t buttons;
		int8_t x;
		int8_t y;
		int8_t wheel;
	} mouse_t;

	void USB_LP_CAN1_RX0_IRQHandler(void);
	void usbInit(void);
	void usbSendMouseReport(mouse_t *report);

#endif
