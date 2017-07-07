#ifndef _STEPPER_H_
#define _STEPPER_H_

	#include "stm32f1xx_hal.h"

	#define MOT1_PORT     GPIOB
	#define MOT1_PIN1     GPIO_PIN_11
	#define MOT1_PIN2     GPIO_PIN_10
	#define MOT1_PIN3     GPIO_PIN_1
	#define MOT1_PIN4     GPIO_PIN_0
	#define MOT2_PORT     GPIOA
	#define MOT2_PIN1     GPIO_PIN_7
	#define MOT2_PIN2     GPIO_PIN_6
	#define MOT2_PIN3     GPIO_PIN_5
	#define MOT2_PIN4     GPIO_PIN_4
	#define MOT3_PORT     GPIOA
	#define MOT3_PIN1     GPIO_PIN_3
	#define MOT3_PIN2     GPIO_PIN_2
	#define MOT3_PIN3     GPIO_PIN_1
	#define MOT3_PIN4     GPIO_PIN_0
	#define GPO_PORT      GPIOB
	#define GPO_RL_PIN    GPIO_PIN_8
	#define GPO_TR_PIN    GPIO_PIN_9

	void stepperInit(void);
	void stepperRelayOn(void);
	void stepperRelayOff(void);
	void stepperTransistorOn(void);
	void stepperTransistorOff(void);
	void stepperStep(uint8_t motor, int16_t steps);
	void stepperSetSequence(uint8_t s);

#endif
