#ifndef _CAN_H_
#define _CAN_H_

	#include "stm32f4xx_hal.h"

	/* inicializalja a CAN1 modult (TXD: PD1, RXD: PD0) */
	void canInit(void);

	/* beallitja a hutoviz homerseklelet (parameter celsiusban) */
	void canSetCoolantTemp(uint8_t temp);

	/* beallitja a fordulatszamot */
	void canSetRPM(uint16_t r);

	/* figyelmezteto lampak be- es kikapcsolasa */
	void canLightCheckEngineOn(void);
	void canLightCheckEngineOff(void);
	void canLightEMLOn(void);
	void canLightEMLOff(void);
	void canLightOverheatOn(void);
	void canLightOverheatOff(void);
	void canLightCruiseControlOn(void);
	void canLightCruiseControlOff(void);

#endif
