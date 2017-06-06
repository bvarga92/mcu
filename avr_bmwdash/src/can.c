#include "can.h"
#include "mcp2515.h"
#include <avr/io.h>

uint8_t coolantTemp; //hutoviz homerseklet=coolantTemp*0.75-48.373
uint16_t rpm; //fordulatszam=rpm/6.4
uint8_t lightCheckEngine; //motorhiba lampa (0/1)
uint8_t lightCruiseControl; //tempomat lampa (0/1)
uint8_t lightEML; //EML lampa (0/1)
uint8_t lightOverheat; //hutoviz forro lampa (0/1)
uint8_t consumption; //fogyasztas=8*consumption/sebesseg
static CanMsg_t msg;

/* CAN busz inicializalasa */
void canInit(void){
	mcp2515Init();
}

/* sebesseg (nem ezt mutatja a muszer, de enelkul check engine hiba van!) */
void canSendSpeed(void){
	msg.id=0x153;
	msg.rtr=0;
	msg.length=8;
	msg.data[0]=0x00;
	msg.data[1]=0x00; //LSB
	msg.data[2]=0x00; //MSB
	msg.data[3]=0x00;
	msg.data[4]=0x00;
	msg.data[5]=0x00;
	msg.data[6]=0x00;
	msg.data[7]=0x00;
	mcp2515SendStd(&msg);
}

/* fordulatszam */
void canSendRPM(void){
	msg.id=0x316;
	msg.rtr=0;
	msg.length=8;
	msg.data[0]=0x00;
	msg.data[1]=0x00;
	msg.data[2]=rpm&0x00FF; //RPM LSB
	msg.data[3]=rpm>>8; //RPM MSB
	msg.data[4]=0x00;
	msg.data[5]=0x00;
	msg.data[6]=0x00;
	msg.data[7]=0x00;
	mcp2515SendStd(&msg);
}

/* hutoviz homerseklet */
void canSendCoolantTemp(void){
	msg.id=0x329;
	msg.rtr=0;
	msg.length=8;
	msg.data[0]=0x00;
	msg.data[1]=coolantTemp; //homerseklet
	msg.data[2]=0x00;
	msg.data[3]=0x00;
	msg.data[4]=0x00;
	msg.data[5]=0x00; //gazpedal pozicio (0x00-0xFE)
	msg.data[6]=0x00; //fekpedal (0x01), kickdown (0x04)
	msg.data[7]=0x00;
	mcp2515SendStd(&msg);
}

/* fenyek (motorhiba, tempomat, EML, tulmelegedes), fogyasztas (valtozas sebessege szamit) */
void canSendLightsConsumption(void){
	static uint16_t d=0;
	d+=consumption;
	msg.id=0x545;
	msg.rtr=0;
	msg.length=8;
	msg.data[0]=(lightCheckEngine<<1)+(lightCruiseControl<<3)+(lightEML<<4);
	msg.data[1]=d&0x00FF; //fogyasztas LSB
	msg.data[2]=d>>8; //fogyasztas MSB
	msg.data[3]=lightOverheat<<3;
	msg.data[4]=0x7E;
	msg.data[5]=0x10;
	msg.data[6]=0x00;
	msg.data[7]=0x18;
	mcp2515SendStd(&msg);
}
