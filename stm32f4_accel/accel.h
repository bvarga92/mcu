#ifndef _ACCEL_H_
#define _ACCEL_H_

	#include "stm32f4xx_hal.h"

	/* ez a flag jelzi, ha ki lehet olvasni a szenzort */
	extern volatile uint8_t accelDataAvailable;

	/* tipus a 3 iranyu gyorsulas tarolasahoz */
	typedef struct{int16_t x, y, z;} accel_t;

	/* inicializalja a gyorsulasmerot */
	void accelInit(void);

	/* kiolvassa a szenzorbol az adatokat, es beleirja az atadott strukturaba */
	void accelGetData(accel_t* pData);

#endif
