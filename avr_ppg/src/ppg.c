/***************************************************************************/
/*                                                                         */
/*  Reflexios fotopletizmograf                          Varga Balazs 2015  */
/*                                                                         */
/*  Fuse bitek: 0xFFD9F7                                                   */
/*     - SPIEN bekapcs                                                     */
/*     - CKDIV8 kikapcs                                                    */
/*     - SUT_CKSEL: external full-swing, startup 16kCK/14CK+64ms           */
/*                                                                         */
/***************************************************************************/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ssd1306.h"

volatile uint8_t beatCount, bpm;

/* inicializalas */
void init(void){
	/* kijelzo */
	displayInit();
	/* LED */
	DDRC=0x02;
	/* ADC (5-os csatorna, Vref=Vcc, 8 bit, single mode, 125 kHz) */
	ADMUX=0x65;
	ADCSRA=0x87;
	/* kulso megszakitas (PCINT0) */
	PCICR=0x01;
	PCMSK0=0x01;
	/* Timer0: CTC mod, prescaler 256 */
	TCCR0A=0x02;
	TCCR0B=0x04;
	OCR0A=249;
	TIMSK0=0x02;
	/* megszakitasok be */
	sei();
}

/* ADC olvasasa */
uint8_t readADC(void){
	ADCSRA|=0x40;
	while(ADCSRA&0x40) ;
	return ADCH;
}

/* PCINT0 megszakitaskezelo rutin, felfuto elre noveli a szamlalot */
ISR(PCINT0_vect){
	if(PINB&0x01){
		beatCount++;
		PORTC|=0x02;
	}
	else
		PORTC&=~0x02;
}

/* Timer0 megszakitaskezelo rutin, 5s alatti szivvereseket szamolja */
ISR(TIMER0_COMPA_vect){
	static uint16_t cnt=0;
	cnt++;
	if(cnt==1250){
		cnt=0;
		bpm=beatCount*12;
		beatCount=0;
	}
}

/* foprogram */
int main(void){
	uint8_t val, i, buf[128];
	for(i=0;i<128;i++) buf[i]=8;
	init();
	while(1){
		val=readADC();
		if(val>55) val=55;
		displayClear();
		for(i=0;i<127;i++){
			buf[i]=buf[i+1];
			displaySetPixel(i,buf[i]);
		}
		displaySetPixel(127,buf[127]=63-val);
		displaySetCursor(0,0);
		displayPrintText("BPM: ");
		if((bpm>50)&&(bpm<150))
			displayPrintInteger(bpm);
		else
			displayPrintText("???");
		displayUpdate();
		_delay_ms(5);
	}
	return 0;
}
