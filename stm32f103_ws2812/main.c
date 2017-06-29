#include "stm32f1xx_hal.h"
#include "system.h"
#include "ws2812.h"

void doAnimation(void){
	int32_t i, j, k;
	rgb_t color;
	/* 3x pulzalas feheren */
	for(i=0;i<3;i++){
		for(j=0;j<255;j++){
			color.r=color.g=color.b=j;
			for(k=0;k<NUM_LEDS;k++) ws2812SetLED(k,&color);
			HAL_Delay(4);
		}
		for(j=255;j>=0;j--){
			color.r=color.g=color.b=j;
			for(k=0;k<NUM_LEDS;k++) ws2812SetLED(k,&color);
			HAL_Delay(4);
		}
	}
	/* HSV kor */
	for(i=0;i<400;i++){
		for(j=0;j<NUM_LEDS;j++){
			hsv2rgb(j*360/NUM_LEDS,100,(j==(i%NUM_LEDS))?100:10,&color);
			ws2812SetLED(j,&color);
		}
		HAL_Delay(30);
	}
	/* sotet */
	color.r=color.g=color.b=0;
	for(i=0;i<NUM_LEDS;i++) ws2812SetLED(i,&color);
}

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	ws2812Init();
	doAnimation();
	while(1){

	}
	return 1;
}
