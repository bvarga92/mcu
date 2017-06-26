#ifndef _USB_H_
#define _USB_H_

	#include "stm32f1xx_hal.h"

	typedef struct{
		uint8_t id;
		uint8_t modifiers;
		uint8_t key1;
		uint8_t key2;
		uint8_t key3;
	} keyboard_t;

	typedef struct{
		uint8_t id;
		uint8_t keys;
	} media_t;

	void USB_LP_CAN1_RX0_IRQHandler(void);
	void usbInit(void);
	void usbSendKeyboardReport(keyboard_t *report);
	void usbSendMediaReport(media_t *report);

#endif
