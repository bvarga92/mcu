#include "usb.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"

USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

void USB_LP_CAN1_RX0_IRQHandler(void){
	HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void usbInit(void){
	__HAL_RCC_AFIO_CLK_ENABLE();
	USBD_Init(&hUsbDeviceFS,&FS_Desc,DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS,&USBD_MSC);
	USBD_MSC_RegisterStorage(&hUsbDeviceFS,&USBD_Storage_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);
}
