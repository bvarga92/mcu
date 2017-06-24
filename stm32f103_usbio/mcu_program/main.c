#include "stm32f1xx_hal.h"
#include "system.h"
#include "vcp.h"
#include "spi.h"
#include "adc.h"
#include "lcd.h"

char lcdBuffer[32];
volatile uint8_t updateLCD=0;

void vcpReceived(uint8_t data){
	static uint8_t recvingLCDData=0, lcdCntr=0;
	uint8_t adcVal, str[4];
	ledToggle();
	if(!recvingLCDData){ //nem LCD adat vetele kozben vagyunk
		if(data=='$'){ //start delimiter, LCD adat vetelere felkeszulunk
			recvingLCDData=1;
			lcdCntr=0;
		}
		else if((data>=0x30)&&(data<=0x39)){ //ADC lekerdezes jott
			adcVal=adcRead(data-0x30);
			str[0]=adcVal/100+0x30;
			str[1]=(adcVal/10)%10+0x30;
			str[2]=adcVal%10+0x30;
			str[3]='\n';
			vcpTransmit(str,4);
		}
	}
	else{ //LCD adat vetele kozben vagyunk
		if(lcdCntr==32){
			if(data=='|') updateLCD=1; //end delimiter, ervenyes csomagot vettunk
			lcdCntr=0;
			recvingLCDData=0;
		}
		else{
			lcdBuffer[lcdCntr]=(char)data;
			lcdCntr++;
		}
	}
}

int main(void){
	uint8_t spiMode, ch=0;
	HAL_Init();
	clockInit();
	spiMode=jumperIsOn()?0:1;
	ledInit();
	adcInit();
	if(!spiMode)
		vcpInit();
	else
		spiInit();
	HAL_Delay(100);
	lcdInit();
	lcdPutString("Ready...",8);
	while(1){
		if(updateLCD){
			updateLCD=0;
			lcdCursorGoto(0,0);
			lcdPutString(lcdBuffer,16);
			lcdCursorGoto(1,0);
			lcdPutString(lcdBuffer+16,16);
		}
		if(spiMode){
			spiTransmit16Bit(ch,adcRead(ch));
			ch=(ch+1)%10;
		}
	}
	return 1;
}
