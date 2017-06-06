/***************************************************************************/
/*                                                                         */
/*  BMW muszeregyseg vezerlese                          Varga Balazs 2016  */
/*                                                                         */
/*  Fuse bitek: 0xFFD9F7                                                   */
/*     - SPIEN bekapcs                                                     */
/*     - CKDIV8 kikapcs                                                    */
/*     - SUT_CKSEL: external full-swing, startup 16kCK/14CK+64ms           */
/*                                                                         */
/***************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdbool.h>
#include "can.h"
#include "kbus.h"

void init(void){
	canInit(); //CAN busz
	kbusInit(); //KBus
	DDRD|=0x08; //sebessegjel
	DDRC&=~0x3E; //gombok es potmeterek
	PORTC|=0x38; //belso felhuzoellenallasok a gombokhoz
	/* ADC (Vref=Vcc, 10 bit, single mode, 125 kHz) */
	ADMUX=0x40;
	ADCSRA=0x87;
	/* Timer1: Fast PWM, prescaler 256 */
	TCCR1A=0x03;
	TCCR1B=0x1C;
	OCR1A=2000;
	TIMSK1=0x02;
}

/* Timer1 megszakitaskezelo, eloallitja a sebessegjelet */
ISR(TIMER1_COMPA_vect){
	PORTD^=0x08;
}

/* egy csatorna ADC olvasasa */
uint16_t adcRead(uint8_t ch){
	ADMUX&=0xF0;
	ADMUX|=ch;
	ADCSRA|=0x40;
	while(ADCSRA&0x40) ;
	return ADC;
}

int main(void){
	bool btn1State=false, btn2State=false, btn3State=false;
	init();
	_delay_ms(1000);
	sei();
	coolantTemp=184; //90 fok
	lightCheckEngine=lightCruiseControl=lightEML=lightOverheat=0; //minden figyelmeztetes ki
	kbusUpdateLampState();
	kbusSetDisplayBacklight(0x55); //kozepes fenyero
	while(1){
		if((!(PINC&0x20))&&(btn1State==false)){ //megnyomtak az elso gombot
			btn1State=true;
			kbusLeftSignalOn();
			kbusUpdateLampState();
		}
		else if((PINC&0x20)&&(btn1State==true)){ //felengedtek az elso gombot
			btn1State=false;
			kbusLeftSignalOff();
			kbusUpdateLampState();
		}
		if((!(PINC&0x10))&&(btn2State==false)){ //megnyomtak a masodik gombot
			btn2State=true;
			kbusRightSignalOn();
			kbusUpdateLampState();
		}
		else if((PINC&0x10)&&(btn2State==true)){ //felengedtek a masodik gombot
			btn2State=false;
			kbusRightSignalOff();
			kbusUpdateLampState();
		}
		if((!(PINC&0x08))&&(btn3State==false)){ //megnyomtak a harmadik gombot
			btn3State=true;
			kbusHighBeamOn();
			kbusUpdateLampState();
		}
		else if((PINC&0x08)&&(btn3State==true)){ //felengedtek a harmadik gombot
			btn3State=false;
			kbusHighBeamOff();
			kbusUpdateLampState();
		}
		rpm=(1024-adcRead(2))<<5;
		OCR1A=adcRead(1)+16; //sebesseg=f/6.91=4522.43/(OCR1A+1)
		consumption=4000/(OCR1A+1); //fogyasztas=8*consumption/sebesseg
		canSendRPM();
		_delay_ms(10);
		canSendCoolantTemp();
		_delay_ms(10);
		canSendLightsConsumption();
		_delay_ms(10);
		canSendSpeed();
		_delay_ms(10);
	}
	return 0;
}
