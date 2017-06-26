#include "vcp.h"
#include "stm32f1xx_hal.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_desc.h"

USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

void USB_LP_CAN1_RX0_IRQHandler(void){
	HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void vcpInit(void){
	__HAL_RCC_AFIO_CLK_ENABLE();
	USBD_Init(&hUsbDeviceFS,&FS_Desc,DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS,&USBD_CDC);
	USBD_CDC_RegisterInterface(&hUsbDeviceFS,&USBD_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);
}

void vcpTransmit(uint8_t* buffer, uint32_t length){
	CDC_Transmit_FS(buffer,length);
}
