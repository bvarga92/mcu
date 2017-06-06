#include "kbus.h"
#include <avr/io.h>

static uint8_t lampState=0;
static uint8_t lampError=0;

/* az atadott bajtot kiirja a buszra */
static void kbusSendByte(uint8_t data){
	while(!(UCSR0A & 0x20)) ;
	UDR0=data;
}

/* inicializalas (USART: aszinkron, 9600 baud, 8E1 mod, csak adas) */
void kbusInit(void){
	DDRD|=0x02;
	UBRR0H=0;
	UBRR0L=103;
	UCSR0B=0x08;
	UCSR0C=0x26;
}

/* lampakat beallito fuggvenyek (utanuk a kbusUpdateLampState() fuggvenyt is hivni kell!) */
void kbusLeftSignalOn(void){lampState|=0x20;}
void kbusLeftSignalOff(void){lampState&=~0x20;}
void kbusRightSignalOn(void){lampState|=0x40;}
void kbusRightSignalOff(void){lampState&=~0x40;}
void kbusFogFrontOn(void){lampState|=0x08;}
void kbusFogFrontOff(void){lampState&=~0x08;}
void kbusFogRearOn(void){lampState|=0x10;}
void kbusFogRearOff(void){lampState&=~0x10;}
void kbusHighBeamOn(void){lampState|=0x04;}
void kbusHighBeamOff(void){lampState&=~0x04;}
void kbusErrorFLOn(void){lampError|=0x20;}
void kbusErrorFLOff(void){lampError&=~0x20;}
void kbusErrorFROn(void){lampError|=0x10;}
void kbusErrorFROff(void){lampError&=~0x10;}
void kbusErrorRLOn(void){lampError|=0x08;}
void kbusErrorRLOff(void){lampError&=~0x08;}
void kbusErrorRROn(void){lampError|=0x04;}
void kbusErrorRROff(void){lampError&=~0x04;}

/* frissiti a lampak allapotat (index, kodlampa, tavolsagi, lampahibak) */
void kbusUpdateLampState(void){
	kbusSendByte(0xD0); //forras
	kbusSendByte(0x07); //hossz (kovetkezo bajtok szama)
	kbusSendByte(0xBF); //cel
	kbusSendByte(0x5B);
	kbusSendByte(lampState); //index (0x20: bal, 0x40: jobb), kodlampa (0x10: hatso, 0x08: elso), tavolsagi (0x04)
	kbusSendByte(0x00);
	kbusSendByte(0x00);
	kbusSendByte(lampError); //lampa hiba (0x04: jobb hatso, 0x08: bal hatso, 0x10: jobb elso, 0x20: bal elso)
	kbusSendByte(0x33^lampState^lampError); //checksum (minden bajt bitenkent osszeXORolva)
}

/* beallitja a kijelzo fenyerejet (parameter: 0-255) */
void kbusSetDisplayBacklight(uint8_t bl){
	kbusSendByte(0xD0); //forras
	kbusSendByte(0x07); //hossz (kovetkezo bajtok szama)
	kbusSendByte(0xBF); //cel
	kbusSendByte(0x5C);
	kbusSendByte(0x00);
	kbusSendByte(bl); //kijelzo fenyero (0x00-0xFF)
	kbusSendByte(0x00);
	kbusSendByte(0x00);
	kbusSendByte(0x34^bl); //checksum (minden bajt bitenkent osszeXORolva)
}
