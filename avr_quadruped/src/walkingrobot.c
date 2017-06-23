/***************************************************************************/
/*                                                                         */
/*  Negy labon setalo robot                             Varga Balazs 2015  */
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
#include "servo.h"
#include "remote.h"
#include "gait.h"

volatile uint32_t halfMillis;

void timer2Init(void){
	TCCR2A=0x02;
	TCCR2B=0x03;
	OCR2A=249;
	TCNT2=0;
	halfMillis=0;
	TIMSK2=0x02;
}

ISR(TIMER2_COMPA_vect){
	if(getButtons()!=0)
		halfMillis=0;
	else
		halfMillis++;
}

void rest(void){
	ledOff();
	sitDown();
	while(getButtons()==0) ;
	ledOn();
	standUp();
}

int main(void){
	uint8_t btn;
	servoInit();
	remoteInit();
	timer2Init();
	rest();
	while(1){
		btn=getButtons();
		if(btn){
			ledOn();
			if(btn&BTN1)
				stepForward();
			else if(btn&BTN2)
				turnLeft();
			else if(btn&BTN3)
				turnRight();
			else if(btn&BTN4)
				stepBackward();
		}
		else{
			ledOff();
		}
		if(halfMillis>20000){
			rest();
		}
	}
	return 1;
}
