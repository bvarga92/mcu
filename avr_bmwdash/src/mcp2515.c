#include <avr/io.h>
#include "mcp2515.h"

/* egy bajt atvitele SPI porton */
static void spiTransfer(uint8_t dataWrite, uint8_t* dataRead){
	SPDR=dataWrite;
	while(!(SPSR&0x80)) ;
	if(dataRead) *dataRead=SPDR;
}

/* reset */
static void mcp2515Reset(void){
	PORTB&=~0x02;
	spiTransfer(0xC0,0);
	PORTB|=0x02;
}

/* regiszter beirasa */
static void mcp2515Write(uint8_t addr, uint8_t data){
	PORTB&=~0x02;
	spiTransfer(0x02,0);
	spiTransfer(addr,0);
	spiTransfer(data,0);
	PORTB|=0x02;
}

/* regiszter irasa bitenkent */
static void mcp2515BitModify(uint8_t addr, uint8_t mask, uint8_t data){
	PORTB&=~0x02;
	spiTransfer(0x05,0);
	spiTransfer(addr,0);
	spiTransfer(mask,0);
	spiTransfer(data,0);
	PORTB|=0x02;	
}

/* regiszter kiolvasasa */
static uint8_t mcp2515Read(uint8_t addr){
	uint8_t data;
	PORTB&=~0x02;
	spiTransfer(0x03,0);
	spiTransfer(addr,0);
	spiTransfer(0,&data);
	PORTB|=0x02;	
	return data;
}

/* statuszbitek kiolvasasa */
static uint8_t mcp2515ReadStatus(void){
	uint8_t data;
	PORTB&=~0x02;
	spiTransfer(0xA0,0);
	spiTransfer(0,&data);
	spiTransfer(0,&data);
	PORTB|=0x02;	
	return data;
}

/* melyik szuro volt aktiv */
static uint8_t mcp2515ReadRxStatus(void){
	uint8_t data;
	PORTB&=~0x02;
	spiTransfer(0xB0,0);
	spiTransfer(0,&data);
	spiTransfer(0,&data);
	PORTB|=0x02;
	return data;
}

/* kuldes kezdemenyezese */
static void mcp2515RequestToSend(uint8_t nbuf){
	PORTB&=~0x02;
	spiTransfer(0x80|(1<<nbuf),0);
	PORTB|=0x02;
}

/* CAN vezerlo inicializalasa */
void mcp2515Init(void){
	/* GPIO */
	DDRB|=0x02;
	DDRC&=~0x01;
	/* SPI inicializalasa (master, 1 Mbps, mode 0, MSB first) */
	DDRB|=0x2C;
	DDRB&=0xEF;
	SPCR=0x51;
	/* konfiguralas */
	mcp2515Reset();
	mcp2515Write(0x0C,0x00);
	mcp2515Write(0x0D,0x00);
	mcp2515Write(0x2A,R_CNF1_500kbps);
	mcp2515Write(0x29,R_CNF2_500kbps);
	mcp2515Write(0x28,R_CNF3_500kbps);
	mcp2515Write(0x60,0x00);
	mcp2515Write(0x70,0x00);
	mcp2515Write(0x24,0x00);
	mcp2515Write(0x25,0x00);
	mcp2515Write(0x26,0x00);
	mcp2515Write(0x27,0x00);
	mcp2515Write(0x24,0xFF);
	mcp2515Write(0x25,0xFF);
	mcp2515Write(0x26,0xFF);
	mcp2515Write(0x27,0xFF);
	mcp2515Write(0x2B,0x01);
	mcp2515Write(0x0F,0x08);
}

/* CAN uzenet vetele (blokkol, amig nem jon ervenyes uzenet!) */
void mcp2515Recv(CanMsg_t *msgPtr){
	uint8_t data, i;
	while(PINC&0x01) ;
	PORTB&=~0x02;
	spiTransfer(0x90,0);
	spiTransfer(0,&data);
	msgPtr->id=((uint16_t)data)<<3;
	spiTransfer(0,&data);
	msgPtr->id|=((uint16_t)data)>>5;
	spiTransfer(0,&data);
	spiTransfer(0,&data);
	spiTransfer(0,&data);
	msgPtr->length=data&0x0F;
	for(i=0;i<msgPtr->length;i++){
		spiTransfer(0,&data);
		msgPtr->data[i]=data;
	}
	mcp2515BitModify(0x2C,0x01,0x00);
}

/* standard CAN uzenet elkuldese a buszra */
void mcp2515SendStd(CanMsg_t *msgPtr){
	uint8_t i;
	mcp2515Write(0x31,(uint8_t)(((msgPtr->id)>>3)&0xFF));
	mcp2515Write(0x32,(uint8_t)(((msgPtr->id)&0x07)<<5));
	if(msgPtr->rtr){
		mcp2515Write(0x35,0x40|(msgPtr->length));
	}
	else{
		mcp2515Write(0x35,(msgPtr->length));
		for(i=0;i<(msgPtr->length);i++) mcp2515Write(0x36+i,msgPtr->data[i]);
	}
	mcp2515RequestToSend(0);
}

/* veteli szurok maszkjainak beallitasa (ahol 1 van, ott vizsgal a szuro) */
void mcp2515SetStdMaskRx0(uint16_t mask){
	mcp2515Write(0x20,(uint8_t)((mask>>3)&0xFF));
	mcp2515Write(0x21,(uint8_t)((mask&0x07)<<5));
}
void mcp2515SetStdMaskRx1(uint16_t mask){
	mcp2515Write(0x24,(uint8_t)((mask>>3)&0xFF));
	mcp2515Write(0x25,(uint8_t)((mask&0x07)<<5));
}

/* veteli szurok beallitasa */
void mcp2515SetStdFilterRxF0(uint16_t filter){
	mcp2515Write(0x00,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x01,(uint8_t)((filter&0x07)<<5));
}
void mcp2515SetStdFilterRxF1(uint16_t filter){
	mcp2515Write(0x04,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x05,(uint8_t)((filter&0x07)<<5));
}
void mcp2515SetStdFilterRxF2(uint16_t filter){
	mcp2515Write(0x08,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x09,(uint8_t)((filter&0x07)<<5));
}
void mcp2515SetStdFilterRxF3(uint16_t filter){
	mcp2515Write(0x10,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x11,(uint8_t)((filter&0x07)<<5));
}
void mcp2515SetStdFilterRxF4(uint16_t filter){
	mcp2515Write(0x14,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x15,(uint8_t)((filter&0x07)<<5));
}
void mcp2515SetStdFilterRxF5(uint16_t filter){
	mcp2515Write(0x18,(uint8_t)((filter>>3)&0xFF));
	mcp2515Write(0x19,(uint8_t)((filter&0x07)<<5));
}
