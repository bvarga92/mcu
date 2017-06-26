#include "usb.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

USBD_HandleTypeDef hUsbDeviceFS;
extern PCD_HandleTypeDef hpcd_USB_FS;

void USB_LP_CAN1_RX0_IRQHandler(void){
	HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void usbInit(void){
	__HAL_RCC_AFIO_CLK_ENABLE();
	USBD_Init(&hUsbDeviceFS,&FS_Desc,DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS,&USBD_HID);
	USBD_Start(&hUsbDeviceFS);
}

void usbSendMouseReport(mouse_t *report){
	uint8_t buf[4];
	buf[0]=report->buttons;
	buf[1]=report->x;
	buf[2]=report->y;
	buf[3]=report->wheel;
	USBD_HID_SendReport(&hUsbDeviceFS,buf,4);
}
