#ifndef _SPEED_H_
#define _SPEED_H_

	#include "stm32f4xx_hal.h"

	/* Timer2/Channel1, Output Compare mod, kimenet: PA5 */
	void speedInit(void);

	/* beallitja az atadott sebesseget */
	void speedSet(uint8_t spd);

	/* visszaadja az aktualis sebesseget kmph-ban (fogyasztashoz kell) */
	uint8_t speedGet(void);

#endif
