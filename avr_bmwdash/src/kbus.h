#ifndef _KBUS_H_
#define _KBUS_H_

	#include <avr/io.h>

	/* inicializalas (USART: aszinkron, 9600 baud, 8E1 mod, csak adas) */
	void kbusInit(void);

	/* lampakat beallito fuggvenyek (utanuk a kbusUpdateLampState() fuggvenyt is hivni kell!) */
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

	/* frissiti a lampak allapotat (index, kodlampa, tavolsagi, lampahibak) */
	void kbusUpdateLampState(void);

	/* beallitja a kijelzo fenyerejet (parameter: 0-255) */
	void kbusSetDisplayBacklight(uint8_t bl);

#endif
