#include "usb.h"
#include "stm32f4xx_hal.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

extern HCD_HandleTypeDef hhcd_USB_OTG_FS;
USBH_HandleTypeDef hUsbHostFS;
static char path[4];

static void usbCallback(USBH_HandleTypeDef *phost, uint8_t id){
	switch(id){
		case HOST_USER_SELECT_CONFIGURATION:  break;
		case HOST_USER_DISCONNECTION:         usbOnDisconnect(); break;
		case HOST_USER_CLASS_ACTIVE:          usbOnClassActive(); break;
		case HOST_USER_CONNECTION:            usbOnConnect(); break;
		default: break;
	}
}

void OTG_FS_IRQHandler(void){
	HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
}

void usbHostProcess(void){
	USBH_Process(&hUsbHostFS);
}

void usbInit(void){
	GPIO_InitTypeDef portInit;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* VBUS ki */
	portInit.Pin=GPIO_PIN_0;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&portInit);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	/* overcurrent bemenet */
	portInit.Pin=GPIO_PIN_5;
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD,&portInit);
	/* STM32 USBH */
	USBH_Init(&hUsbHostFS,usbCallback,HOST_FS);
	USBH_RegisterClass(&hUsbHostFS,USBH_MSC_CLASS);
	USBH_Start(&hUsbHostFS);
	/* FATFS */
	if(FATFS_LinkDriver(&USBH_Driver,path)!=0) while(1) ;
}

char* usbGetPath(void){
	return path;
}
