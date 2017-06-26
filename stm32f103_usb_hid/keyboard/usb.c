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

void usbSendKeyboardReport(keyboard_t *report){
	uint8_t buf[5];
	buf[0]=report->id;
	buf[1]=report->modifiers;
	buf[2]=report->key1;
	buf[3]=report->key2;
	buf[4]=report->key3;
	USBD_HID_SendReport(&hUsbDeviceFS,buf,5);
}

void usbSendMediaReport(media_t *report){
	uint8_t buf[2];
	buf[0]=report->id;
	buf[1]=report->keys;
	USBD_HID_SendReport(&hUsbDeviceFS,buf,2);
}
