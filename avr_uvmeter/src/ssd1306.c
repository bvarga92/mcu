#include "ssd1306.h"
#include "i2c.h"
#include "font.h"
#include <avr/pgmspace.h>

static const uint8_t initCommands[] PROGMEM ={
	0x78,0x00,0xAE,0xD5,0xF0,0xA8,0x1F,0xD3,0x00,
	0x40,0x8D,0x14,0x20,0x00,0xA1,0xC8,0xDA,0x02,
	0x81,0xCF,0xD9,0xF1,0xDB,0x40,0xA4,0xA6,0x2E,
	0xAF,0x22,0x00,0x3f,0x21,0x00,0x7f
};

static void setcursor(uint8_t x,uint8_t y){
	uint8_t buffer[5];
	buffer[0]=0x78;
	buffer[1]=0x00;
	buffer[2]=0xB0|(y&0x07);
	buffer[3]=x&0x0F;
	buffer[4]=0x10|(x>>4);
	i2cTransfer(buffer,5,0);
}

static void showchar(char c){
	uint8_t buffer[2];
	uint16_t base=(c<<2)+c-160;
	buffer[0]=0x78;
	buffer[1]=0x40;
	i2cTransfer(buffer,2,NOSTOP);
	i2cTransfer((uint8_t*)&font[base],4,NOSTART|NOSTOP|FLASH);
	i2cTransfer((uint8_t*)&font[base+4],1,NOSTART|FLASH);
}

void displayInit(void){
	i2cTransfer((uint8_t*)initCommands,sizeof(initCommands),FLASH);
	displayClear();
}

void displayClear(void){
	uint8_t buffer[10],i;
	buffer[0]=0x78;
	buffer[1]=0x40;
	for(i=2;i<10;i++) buffer[i]=0;
	i2cTransfer(buffer,2,NOSTOP);
	buffer[0]=buffer[1]=0;
	for(i=0;i<51;i++) i2cTransfer(buffer,10,NOSTART|NOSTOP);
	i2cTransfer(buffer,2,NOSTART);
}

uint8_t displayPrintText(char* str,uint8_t x,uint8_t y){
	while(*str){
		setcursor(x,y);
		showchar(*str);
		str++;
		x+=6;
	}
	return x;
}

uint8_t displayPrintInteger(int32_t n, uint8_t digits, uint8_t x, uint8_t y){
	int32_t div;
	uint8_t len;
	if(n==0){
		setcursor(x,y);
		showchar('0');
		x+=6;
		for(;digits>1;digits--){
			setcursor(x,y);
			showchar('0');
			x+=6;			
		}
		return x;
	}
	if(n<0){
		setcursor(x,y);
		showchar('-');
		n=-n;
		x+=6;
	}
	for(div=1,len=0;div<=n;div*=10,len++) ;
	for(;len<digits;digits--){
		setcursor(x,y);
		showchar('0');
		x+=6;
	}
	do{
		div/=10;
		setcursor(x,y);
		showchar(0x30+n/div);
		n%=div;
		x+=6;
	}while(div>1);
	return x;
}
