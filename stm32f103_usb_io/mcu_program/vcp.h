#ifndef _VCP_H_
#define _VCP_H_

	#include "stm32f1xx_hal.h"

	/* megszakitaskezelo */
	void USB_LP_CAN1_RX0_IRQHandler(void);

	/* inicializalja az USB VCP-t */
	void vcpInit(void);
	
	/* adatkuldes */
	void vcpTransmit(uint8_t* buffer, uint32_t length);

	/* vetelkor ez hivodik, implementalni kell tetszes szerint */
	extern void vcpReceived(uint8_t data);

#endif
