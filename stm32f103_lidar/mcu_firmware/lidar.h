#ifndef _LIDAR_H_
#define _LIDAR_H_

	#include "stm32f1xx_hal.h"

	void lidarInit(void);
	uint16_t lidarGetDistanceCM(void);

#endif
