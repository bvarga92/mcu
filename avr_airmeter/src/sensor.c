#include "sensor.h"
#define F_CPU 16000000UL
#include <util/delay.h>

static uint16_t adcRead(void){
	ADCSRA|=0x40;
	while(ADCSRA&0x40) ;
	return ADC;
}

void sensorInit(void){
	/* LED (alacsony aktiv!) */
	DDRB|=0x02;
	/* ADC (0-s csatorna, Vref=Vcc, 10 bit, single mode, 125 kHz) */
	ADMUX=0x40;
	ADCSRA=0x87;
}

float sensorRead(void){
	uint16_t adc;
	float volt, ugpm3;
	PORTB&=~0x02;
	_delay_us(280);
	adc=adcRead();
	_delay_us(40);
	PORTB|=0x02;
	_delay_us(9680);
	volt=5*adc/1024.0f;
	ugpm3=(volt-0.9f)*200;
	return (ugpm3<0)?0:ugpm3;
}
