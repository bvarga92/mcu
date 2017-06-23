#include "motors.h"
#include <avr/io.h>

void motorsInit(void){
	DDRB|=0x06;
	DDRD|=0xC0;
	PORTD&=0x3F;
	/* Timer1: Fast PWM, Clear on Compare Match, Set at Bottom, 8-as prescaler --> 977 Hz */
	TCCR1A=0xA2;
	TCCR1B=0x1A;
	ICR1=2048;
	OCR1A=OCR1B=0;
}

void motorsDrive(int16_t spd){
	if(spd>SPD_MAX)
		spd=SPD_MAX;
	else if(spd<SPD_MIN)
		spd=SPD_MIN;
	if(spd>=0){
		PORTD&=0x3F;
		if(spd&&(spd<SPD_ABS_MIN)) spd=SPD_ABS_MIN;
	}
	else{
		PORTD|=0xC0;
		if(spd>-SPD_ABS_MIN) spd=-SPD_ABS_MIN;
		spd+=2048;
	}
	OCR1A=spd;
	OCR1B=(uint16_t)((1+MOT_GAIN_ERROR)*spd+MOT_OFFSET_ERROR);
}
