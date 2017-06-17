#include "stm32f4xx_hal.h"
#include "system.h"
#include "audio.h"
#include <math.h>

#define FS  44100

int main(void){
	int16_t outBuffer[2];
	uint16_t inBuffer[2];
	float phase=0;
	int32_t inSampleCntr=0, outSampleCntr=0;
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	audioOutInit(FS,(uint16_t*)outBuffer,2);
	audioInInit(FS,(uint16_t*)inBuffer,2);
	while(1){
		if(inTransferComplete){
			inTransferComplete=0;
			if(inBuffer[0]>0xF000)
				ledOn(LED_RED|LED_GREEN);
			else
				ledOff(LED_RED|LED_GREEN);
			inSampleCntr++;
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
			outSampleCntr++;
		}
		if(inSampleCntr==FS){
			inSampleCntr=0;
			ledToggle(LED_BLUE);
		}
		if(outSampleCntr==FS){
			outSampleCntr=0;
			ledToggle(LED_ORANGE);
		}
	}
	return 1;
}
