#ifndef _KBUS_H_
#define _KBUS_H_

	#include "stm32f4xx_hal.h"

	/* USART1 TX (PB6): 9600 bps, 8E1 */
	void kbusInit(void);

	/* lampakat beallito fuggvenyek */
	void kbusLeftSignalOn(void);
	void kbusLeftSignalOff(void);
	void kbusRightSignalOn(void);
	void kbusRightSignalOff(void);
	void kbusFogFrontOn(void);
	void kbusFogFrontOff(void);
	void kbusFogRearOn(void);
	void kbusFogRearOff(void);
	void kbusHighBeamOn(void);
	void kbusHighBeamOff(void);
	void kbusErrorFLOn(void);
	void kbusErrorFLOff(void);
	void kbusErrorFROn(void);
	void kbusErrorFROff(void);
	void kbusErrorRLOn(void);
	void kbusErrorRLOff(void);
	void kbusErrorRROn(void);
	void kbusErrorRROff(void);

	/* beallitja a kijelzo fenyerejet (parameter: 0-255) */
	void kbusSetDisplayBacklight(uint8_t bl);

#endif
