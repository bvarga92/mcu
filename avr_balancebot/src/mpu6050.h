#ifndef _MPU6050_H_
#define _MPU6050_H_

	#include <avr/io.h>

	/* inicializalas */
	void imuInit(void);

	/* megmeri a gyorsulasmero alapjan a szoget (fokban) es a giroszkop alapjan a szogsebesseget (fok/s-ban) */
	void imuMeasure(float *accDeg, float *gyrDegPerSec);

#endif
