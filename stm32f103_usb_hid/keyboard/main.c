#include "stm32f1xx_hal.h"
#include "system.h"
#include "usb.h"
#include "math.h"

void toggleCapsLock(void){
	keyboard_t keyboard;
	keyboard.id=1;
	keyboard.modifiers=0;
	keyboard.key1=0x39;
	keyboard.key2=0;
	keyboard.key3=0;
	usbSendKeyboardReport(&keyboard);
	HAL_Delay(30);
	keyboard.key1=0;
	usbSendKeyboardReport(&keyboard);
}

void toggleMute(void){
	media_t media;
	media.id=2;
	media.keys=0x20;
	usbSendMediaReport(&media);
	HAL_Delay(30);
	media.keys=0;
	usbSendMediaReport(&media);
}

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	usbInit();
	while(1){
		ledToggle();
		toggleCapsLock();
		HAL_Delay(500);
		toggleMute();
		HAL_Delay(500);
	}
	return 1;
}
