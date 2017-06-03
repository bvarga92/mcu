/***************************************************************************/
/*                                                                         */
/*  RC szervo teszter                                   Varga Balazs 2015  */
/*                                                                         */
/*  Fuse bitek: 0xFF7A                                                     */
/*     - SPIEN bekapcs                                                     */
/*     - CKDIV8 kikapcs                                                    */
/*     - SUT_CKSEL: belso RC oszcillator, 9.6 MHz, startup 14CK+64ms       */
/*                                                                         */
/***************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

volatile uint8_t position;

void wdtFirst(void) __attribute__((naked)) __attribute__((section(".init3")));
void init(void);
uint16_t readADC(void);
int main(void);

/* watchdog flag a reset utan is aktiv marad, ezert a program legelejen (a main elott) torolni kell */
void wdtFirst(void){
	MCUSR=0;
	wdt_disable();
}

/* inicializalas */
void init(void){
	/* adatiranyok */
	DDRB=0x09;
	/* orajel kalibralasa (ha nem tudunk pontosan merni, kommentezzuk ki!) */
	OSCCAL+=2;
	/* ADC: 2-es csatorna, Vref=Vcc, 75 kHz, interrupt off */
	DIDR0=0x10;
	ADMUX=0x02;
	ADCSRA=0x87;
	/* Timer0: CTC mod, prescaler 1 */
	TCCR0A=0x02;
	TCCR0B=0x01;
	OCR0A=95;
	TIMSK0=0x04;
	/* LED bekapcs */
	PORTB&=0xF7;
}

/* potmeterallas beolvasasa, 8-as atlagolas */
uint16_t readADC(void){
	uint8_t i;
	uint16_t sum=0;
	for(i=0;i<8;i++){
		ADCSRA|=0x40;
		while(ADCSRA&0x40) ;
		sum+=ADCW;
	}
	return sum>>3;
}

/* Timer0 megszakitaskezelo rutin */
ISR(TIM0_COMPA_vect){
	static uint16_t cnt;
	if(cnt>position)
		PORTB&=0xFE;
	else
		PORTB|=0x01;
	cnt=(cnt<1999)?(cnt+1):0;
}

/* foprogram */
int main(void){
	init();
	position=readADC()>>2;
	wdt_enable(WDTO_120MS);
	sei();
	while(1){
		position=readADC()>>2;
		wdt_reset();
	}
	return 0;
}
