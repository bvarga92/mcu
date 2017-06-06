#ifndef _CAN_H_
#define _CAN_H_

	#include <avr/io.h>

	extern uint8_t coolantTemp; //hutoviz homerseklet=coolantTemp*0.75-48.373
	extern uint16_t rpm; //fordulatszam=rpm/6.4
	extern uint8_t lightCheckEngine; //motorhiba lampa (0/1)
	extern uint8_t lightCruiseControl; //tempomat lampa (0/1)
	extern uint8_t lightEML; //EML lampa (0/1)
	extern uint8_t lightOverheat; //hutoviz forro lampa (0/1)
	extern uint8_t consumption; //fogyasztas=8*consumption/sebesseg

	/* CAN busz inicializalasa */
	void canInit(void);

	/* sebesseg (nem ezt mutatja a muszer, de enelkul check engine hiba van!) */
	void canSendSpeed(void);

	/* fordulatszam */
	void canSendRPM(void);

	/* hutoviz homerseklet */
	void canSendCoolantTemp(void);

	/* fenyek (motorhiba, tempomat, EML, tulmelegedes), fogyasztas (valtozas sebessege szamit) */
	void canSendLightsConsumption(void);

#endif
