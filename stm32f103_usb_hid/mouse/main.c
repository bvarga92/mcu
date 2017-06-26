#include "stm32f1xx_hal.h"
#include "system.h"
#include "usb.h"
#include "math.h"

/* korbe-korbe mozgatja az egeret */
void moveMouse(void){
	static mouse_t mouse;
	static float phase=0;
	mouse.buttons=0;
	mouse.wheel=0;
	mouse.x=20*cosf(6.28319f*phase);
	mouse.y=20*sinf(6.28319f*phase);
	usbSendMouseReport(&mouse);
	phase+=0.05f;
	if(phase>1) phase-=1;
}

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	usbInit();
	while(1){
		ledToggle();
		moveMouse();
		HAL_Delay(50);
	}
	return 1;
}
