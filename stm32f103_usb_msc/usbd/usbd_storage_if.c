#include "usbd_storage_if.h"
#include "stm32f1xx_hal_flash.h"
#include "system.h"

/* a flash felso 30 kB-jaban a program van, az also 34 kB lesz a hattertar */
#define PGM_SIZE                30720
#define DISK_SIZE               34816
#define DISK_BLOCK_SIZE         512
#define DISK_BLOCK_NBR          (DISK_SIZE/DISK_BLOCK_SIZE)       //68
#define DISK_BASE_ADDR          (FLASH_BASE+PGM_SIZE)             //0x08007800
#define BLOCK_PER_FLASH_PAGE    (FLASH_PAGE_SIZE/DISK_BLOCK_SIZE) //2
#define FIRST_PAGE              (PGM_SIZE/FLASH_PAGE_SIZE)        //30


extern USBD_HandleTypeDef hUsbDeviceFS;

static const int8_t STORAGE_Inquirydata_FS[]={
	0x00,
	0x80,
	0x02,
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 5),
	0x00,
	0x00,
	0x00,
	'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ',
	'P', 'r', 'o', 'd', 'u', 'c', 't', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'0', '.', '0' ,'1'
}; 


static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);


USBD_StorageTypeDef USBD_Storage_Interface_fops_FS={
	STORAGE_Init_FS,
	STORAGE_GetCapacity_FS,
	STORAGE_IsReady_FS,
	STORAGE_IsWriteProtected_FS,
	STORAGE_Read_FS,
	STORAGE_Write_FS,
	STORAGE_GetMaxLun_FS,
	(int8_t *)STORAGE_Inquirydata_FS,
};


int8_t STORAGE_Init_FS(uint8_t lun){
	return USBD_OK;
}

int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size){
	*block_num=DISK_BLOCK_NBR;
	*block_size=DISK_BLOCK_SIZE;
	return USBD_OK;
}

int8_t STORAGE_IsReady_FS(uint8_t lun){
	return USBD_OK;
}

int8_t STORAGE_IsWriteProtected_FS(uint8_t lun){
	return USBD_OK;
}

int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len){
	ledOn();
	memcpy(buf,(void*)(DISK_BASE_ADDR+blk_addr*DISK_BLOCK_SIZE),blk_len*DISK_BLOCK_SIZE);
	ledOff();
	return USBD_OK;
}

int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len){
	uint32_t i, j, page, prevPage, pageError;
	uint8_t pageBuffer[FLASH_PAGE_SIZE];
	uint64_t* dwordptr;
	FLASH_EraseInitTypeDef eraseStruct;
	ledOn();
	eraseStruct.TypeErase=FLASH_TYPEERASE_PAGES;
	eraseStruct.NbPages=1;
	eraseStruct.Banks=0;
	prevPage=0xFFFFFFFF;
	HAL_FLASH_Unlock();
	for(i=0;i<blk_len;i++){
		page=FIRST_PAGE+((blk_addr+i)/BLOCK_PER_FLASH_PAGE);
		if(prevPage!=page){ //ha uj lapot kezdunk, akkor kimentjuk, majd toroljuk
			eraseStruct.PageAddress=FLASH_BASE+page*FLASH_PAGE_SIZE;
			memcpy(pageBuffer,(void*)(eraseStruct.PageAddress),FLASH_PAGE_SIZE);
			HAL_FLASHEx_Erase(&eraseStruct,&pageError);
			prevPage=page;
		}
		memcpy(&pageBuffer[((blk_addr+i)%BLOCK_PER_FLASH_PAGE)*DISK_BLOCK_SIZE],&buf[i*DISK_BLOCK_SIZE],DISK_BLOCK_SIZE);
		if((((blk_addr+i)%BLOCK_PER_FLASH_PAGE)==(BLOCK_PER_FLASH_PAGE-1)) || (i==(blk_len-1))) //ezt a lapot teleirtuk, vagy keszen vagyunk --> kiirjuk
			for(j=0,dwordptr=(uint64_t*)pageBuffer;j<FLASH_PAGE_SIZE;j+=8,dwordptr++)
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,page*FLASH_PAGE_SIZE+j,*dwordptr);
	}
	HAL_FLASH_Lock();
	ledOff();
	return USBD_OK;
}

int8_t STORAGE_GetMaxLun_FS(void){
	return 0;
}
