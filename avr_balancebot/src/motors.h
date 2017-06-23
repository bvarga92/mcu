#ifndef _MOTORS_H_
#define _MOTORS_H_

	#include <avr/io.h>

	/* hatarok a sebessegre */
	#define SPD_MAX 2048
	#define SPD_MIN -2048
	#define SPD_ABS_MIN 400

	/* a ket motor kozotti kulonbsegeket itt lehet figyelembe venni */
	#define MOT_OFFSET_ERROR  0
	#define MOT_GAIN_ERROR    0

	/* inicializalas */
	void motorsInit(void);

	/* motorok meghajtasa (-2048<=spd<=2048) */
	void motorsDrive(int16_t spd);

#endif
