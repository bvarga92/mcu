#include "servo.h"
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

volatile uint8_t pos[12]={225,225,125,125,25,25,225,225,125,125,25,25}; //kezdopoziciok (felemelt labak)

/* periodikusan lefut */
ISR(TIMER0_COMPA_vect){
	static uint8_t n=0;
	/* inditjuk az aktualis szervo impulzusat */
	if(n<8)
		PORTD|=0x01<<n;
	else if(n<12)
		PORTC|=0x01<<(n-8);
	/* inditjuk a masik timert az elozo szervo impulzusanak lehuzasahoz (0...1ms) */
	if(n && (n<=12)){
		OCR1A=pos[n-1]<<3;
		TCNT1=0;
		TIFR1|=0x02;
		TIMSK1|=0x02;
	}
	n=(n+1)%20;
}

/* az elozo szervo impulzusat kikapcsolja */
ISR(TIMER1_COMPA_vect){
	static uint8_t n=0;
	if(n<8)
		PORTD&=~(0x01<<n);
	else if(n<12)
		PORTC&=~(0x01<<(n-8));
	n=(n+1)%12;
	TIMSK1&=~(0x02);
}

void servoInit(void){
	/* I/O adatiranyok */
	DDRC=0x0F;
	DDRD=0xFF;
	/* Timer0: CTC, 1 kHz */
	TCCR0A=0x02;
	TCCR0B=0x03;
	OCR0A=249;
	TCNT0=0;
	/* Timer1: CTC, 8-as prescaler */
	TCCR1A=0x00;
	TCCR1B=0x0A;
	TCCR1C=0x00;
	TCNT1=0;
	/* Timer0 mehet, de Timer1 meg nem */
	TIMSK0=0x02;
	sei();
}

void servoMoveSmooth(uint8_t *newpos, uint8_t delay, uint8_t step){
	uint8_t i, neededChange;
	while(1){
		neededChange=0;
		for(i=0;i<12;i++){
			if(pos[i]<newpos[i]){
				pos[i]=(pos[i]+step>newpos[i])?(newpos[i]):(pos[i]+step);
				neededChange=1;
			}
			else if(pos[i]>newpos[i]){
				pos[i]=(pos[i]-step<newpos[i])?(newpos[i]):(pos[i]-step);
				neededChange=1;
			}
		}
		if(neededChange)
			_delay_ms(delay);
		else
			return;
	}
}
