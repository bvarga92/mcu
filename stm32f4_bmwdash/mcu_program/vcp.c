#include "vcp.h"
#include "stm32f4xx_hal.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_desc.h"

USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

void OTG_FS_IRQHandler(void){
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

void vcpInit(void){
	GPIO_InitTypeDef portInit;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* VBUS ki */
	portInit.Pin = GPIO_PIN_0;
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &portInit);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	/* overcurrent bemenet */
	portInit.Pin = GPIO_PIN_5;
	portInit.Mode = GPIO_MODE_INPUT;
	portInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &portInit);
	/* STM32 USBD */
	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);
}

void vcpTransmit(uint8_t* buffer, uint32_t length){
	CDC_Transmit_FS(buffer,length);
}
