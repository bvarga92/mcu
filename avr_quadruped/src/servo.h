#ifndef _SERVO_H_
#define _SERVO_H_

	#include <avr/io.h>

	/* pozicioindexek (Front/Rear, Left/Right) */
	#define FL1 2  //PD2
	#define FL2 1  //PD1
	#define FL3 0  //PD0
	#define FR1 9  //PC1
	#define FR2 10 //PC2
	#define FR3 11 //PC3
	#define RL1 3  //PD3
	#define RL2 4  //PD4
	#define RL3 5  //PD5
	#define RR1 8  //PC0
	#define RR2 6  //PD6
	#define RR3 7  //PD7


	/* poziciok (1...240), csak 1 bajt --> modositaskor nem kell ATOMIC blokk! */
	extern volatile uint8_t pos[12];


	/* szervovezerles inicializalasa */
	void servoInit(void);

	/* a pos tomb kozvetlen irasa helyett igy adott sebesseggel mozoghatunk */
	void servoMoveSmooth(uint8_t *newpos, uint8_t delay, uint8_t step);

#endif
