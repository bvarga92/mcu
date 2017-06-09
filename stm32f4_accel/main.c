#include "stm32f4xx_hal.h"
#include "system.h"
#include "accel.h"

int main(void){
	accel_t acc;
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	accelInit();
	while(1){
		if(accelDataAvailable){
			accelDataAvailable=0;
			accelGetData(&acc);
			if(acc.x>2000){
				ledOn(LED_RED);
				ledOff(LED_GREEN);
			}
			else if(acc.x<-2000){
				ledOn(LED_GREEN);
				ledOff(LED_RED);
			}
			else{
				ledOff(LED_RED|LED_GREEN);
			}
			if(acc.y>2000){
				ledOn(LED_ORANGE);
				ledOff(LED_BLUE);
			}
			else if(acc.y<-2000){
				ledOff(LED_ORANGE);
				ledOn(LED_BLUE);
			}
			else{
				ledOff(LED_ORANGE|LED_BLUE);
			}
		}
	}
	return 0;
}
