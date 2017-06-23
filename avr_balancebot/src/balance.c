/***************************************************************************/
/*                                                                         */
/*  Ket kereken egyensulyozo robot                      Varga Balazs 2015  */
/*                                                                         */
/*  Az MPU6050 szenzorba integralt MEMS gyorsulasmero es giroszkop         */
/*  jeleibol Kalman-szuro algoritmus szamitja ki a dolesszog erteket,      */
/*  amelynek alapjan PID szabalyzo allitja elo a motorok PWM vezerlojelet. */
/*  A motorok elektronikajuktol megfosztott, korbefordulasra alkalmassa    */
/*  tett modellszervok.                                                    */
/*                                                                         */
/*  Fuse bitek: 0xFFD9F7                                                   */
/*     - SPIEN bekapcs                                                     */
/*     - CKDIV8 kikapcs                                                    */
/*     - SUT_CKSEL: external full-swing, startup 16kCK/14CK+64ms           */
/*                                                                         */
/***************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "mpu6050.h"
#include "motors.h"
#include "filter.h"
#include "pid.h"

#define KALMANF   0    //Kalman-szurot (1) vagy komplementer szurot (0) szeretnenk

volatile uint8_t interruptFlag=0; //ha az ISR beallitja ezt a flaget, akkor a foprogram vegrehajt egy aktualasi ciklust
volatile uint8_t ledFlash=60; //LED villogas gyorsasaga

/* hardver inicializalasa */
void init(void){
	motorsInit();
	imuInit();
	DDRC|=0x04;
	/* Timer0: CTC mod, prescaler 1024 --> 8 ms */
	TCCR0A=0x02;
	TCCR0B=0x05;
	OCR0A=124;
	TIMSK0=0x02;
	sei();
}

/* Timer0 megszakitaskezelo rutin, bekapcsolja a flaget, villogtatja a LED-et */
ISR(TIMER0_COMPA_vect){
	static uint8_t ledCnt;
	interruptFlag=1;
	ledCnt++;
	if(ledCnt>ledFlash){
		ledCnt=0;
		PORTC^=0x04;
	}
}

/* foprogram */
int main(void){
	float tilt=0, balancedAngle, motorSpeed, accDeg, gyrDegPerSec;
	pid_t pid;
	#if KALMANF
		kalman_t kalman;
		filterKalmanInit(&kalman);
		pidCreate(&pid,&balancedAngle,&tilt,&motorSpeed,1,650,0,2200,-2048,2048);
		balancedAngle=2.6f; //ez az a szog (fokban), ahol egyensulyban van a robot (PID szabalyzo alapjele)
	#else
		pidCreate(&pid,&balancedAngle,&tilt,&motorSpeed,1,900,0,900,-2048,2048);
		balancedAngle=3.2f;
	#endif
	init();
	while(1){
		if(interruptFlag){
			interruptFlag=0;
			imuMeasure(&accDeg,&gyrDegPerSec);
			#if KALMANF
				tilt=filterKalmanUpdate(&kalman,accDeg,gyrDegPerSec,0.008f);
			#else
				tilt=filterComplementary(tilt,accDeg,gyrDegPerSec,0.008f);
			#endif
			pidUpdate(&pid);
			if((tilt>45)||(tilt<-45)){ //ha nagyon eldolt, kikapcsoljuk a motorokat, es gyorsan villogtatjuk a LED-et
				motorSpeed=0;
				ledFlash=6;
			}
			else{
				ledFlash=60;
			}
			motorsDrive((int16_t)motorSpeed);
		}
	}
	return 1;
}
