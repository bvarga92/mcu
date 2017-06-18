#include "stm32f4xx_hal.h"
#include "system.h"
#include "audio.h"
#include <math.h>

#define FS  44100

int main(void){
	int16_t outBuffer[2];
	uint16_t inBuffer[1];
	float phase=0;
	uint8_t ones;
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	audioOutInit(FS,(uint16_t*)outBuffer,2);
	audioInInit(FS,(uint16_t*)inBuffer,1);
	while(1){
		if(inTransferComplete){
			inTransferComplete=0;
			ones=0;
			while(inBuffer[0]){
				if(inBuffer[0]&0x0001) ones++;
				inBuffer[0]>>=1;
			}
			if(ones> 9) ledOn(LED_BLUE);   else ledOff(LED_BLUE);
			if(ones>10) ledOn(LED_GREEN);  else ledOff(LED_GREEN);
			if(ones>11) ledOn(LED_ORANGE); else ledOff(LED_ORANGE);
			if(ones>12) ledOn(LED_RED);    else ledOff(LED_RED);
		}
		if(outTransferHalfComplete){
			outTransferHalfComplete=0;
			outBuffer[0]=sinf(6.28319f*phase)*32767;
		}
		if(outTransferComplete){
			outTransferComplete=0;
			outBuffer[1]=cosf(6.28319f*phase)*32767;
			phase+=440.0f/((float)FS);
			if(phase>1) phase-=1;
		}
	}
	return 1;
}
