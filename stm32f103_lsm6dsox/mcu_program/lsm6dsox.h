#ifndef _LSM6DSOX_H_
#define _LSM6DSOX_H_

	#include "stm32f1xx_hal.h"
	#include "stdbool.h"

	typedef struct{
		uint16_t temp;
		uint16_t acc_x;
		uint16_t acc_y;
		uint16_t acc_z;
		uint16_t gyr_x;
		uint16_t gyr_y;
		uint16_t gyr_z;
	} lsm_data_t;

	void lsmInit(bool configMLC, bool int2Latched, void (*int2RiseFcn)(void), void (*int2FallFcn)(void));
	bool lsmReadINT2(void);
	void lsmClearINT2(void);
	void lsmGetData(lsm_data_t *dataOut);

#endif
