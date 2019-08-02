#include "i2c.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

static uint8_t transferData(){
	uint8_t data;
	do{
		_delay_us(DELAY_LONG);
		USICR=0x2B;
		while(!(I2C_PIN&(1<<SCL_PIN))) ;
		_delay_us(DELAY_SHORT);
		USICR=0x2B;
	}while(!(USISR&0x40)) ;
	_delay_us(DELAY_LONG);
	data=USIDR;
	USIDR=0xFF;
	I2C_DDR|=(1<<SDA_PIN);
	return data;
}

static void generateStart(void){
	I2C_PORT|=(1<<SCL_PIN);
	while(!(I2C_PORT&(1<<SCL_PIN))) ;
	_delay_us(DELAY_LONG);
	I2C_PORT&=~(1<<SDA_PIN);
	_delay_us(DELAY_SHORT);
	I2C_PORT&=~(1<<SCL_PIN);
	I2C_PORT|=(1<<SDA_PIN);
}

static void generateStop(void){
	I2C_PORT&=~(1<<SDA_PIN);
	I2C_PORT|=(1<<SCL_PIN);
	while(!(I2C_PIN&(1<<SCL_PIN))) ;
	_delay_us(DELAY_SHORT);
	I2C_PORT|=(1<<SDA_PIN);
	_delay_us(DELAY_LONG);
}

void i2cInit(void){
	I2C_PORT|=(1<<SDA_PIN)|(1<<SCL_PIN);
	I2C_DDR|=(1<<SCL_PIN)|(1<<SDA_PIN);
	USIDR=0xFF;
	USICR=0x2A;
	USISR=0xF0;
}

void i2cTransfer(uint8_t *buffer, uint8_t size, uint8_t flags){
	uint8_t address=(flags&FLASH)?pgm_read_byte(buffer):(*buffer);
	uint8_t writeData=((address&1)&(!(flags&FLASH)))?0:1;
	uint8_t writeAddress=1;
	if(!(flags&NOSTART)) generateStart();
	do{
		if(writeAddress||writeData){
			I2C_PORT&=~(1<<SCL_PIN);
			USIDR=(flags&FLASH)?pgm_read_byte(buffer):(*buffer);
			buffer++;
			USISR=0xF0;
			transferData();
			I2C_DDR&=~(1<<SDA_PIN);
			USISR=0xFE;
			transferData();
			if((!writeAddress)&&(flags&MEMREAD)){ //regisztercim kiirva, most repeated start es olvasasi cim kell
				flags&=~MEMREAD;
				writeData=0;
				writeAddress=1;
				generateStart(); //repeated start
			}
			else{
				writeAddress=0;
			}
		}
		else{
			I2C_DDR&=~(1<<SDA_PIN);
			USISR=0xF0;
			*(buffer++)=transferData();
			USIDR=(size==1)?0xFF:0x00; //NACK/ACK	
			USISR=0xFE;
			transferData();
		}
	}while(--size) ;
	if(!(flags&NOSTOP)) generateStop();
}
