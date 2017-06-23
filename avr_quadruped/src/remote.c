#include "remote.h"

void remoteInit(void){
	DDRB|=0x20; //LED
	DDRB&=0xE1; //taviranyito
}

void ledOn(void){
	PORTB|=0x20;
}

void ledOff(void){
	PORTB&=0xDF;
}

uint8_t getButtons(void){
	return (PINB&0x1E)>>1;
}
