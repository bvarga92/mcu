#include "stm32f1xx_hal.h"
#include "system.h"
#include "usb.h"

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	usbInit();
	while(1){

	}
	return 1;
}
