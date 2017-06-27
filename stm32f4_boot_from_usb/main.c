/******************************************************************/
/*                                                                */
/*  Linker beallitasok:                                           */
/*      - Minimum heap size:  0x400                               */
/*      - Minimum stack size: 0x800                               */
/*                                                                */
/*  A bootloader a pendrive-rol beolvasott, program.bin nevu      */
/*  fajllal programozza fel a flash memoria megadott reszet.      */
/*  Fontos, hogy a program.bin forditasakor is megadjuk a helyes  */
/*  memoriacimet. Ezt ket helyen kell megtennunk:                 */
/*      - a linker szkriptben:                                    */
/*              FLASH (rx) : ORIGIN = 0x08010000, LENGTH = 192K   */
/*      - es a SystemInit() fuggvenyben (system_stm32f4xx.c)      */
/*              SCB->VTOR = 0x08010000U | VECT_TAB_OFFSET;        */
/*                                                                */
/******************************************************************/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash.h"
#include "system.h"
#include "usb.h"
#include "ff.h"

/* a 4-es es 5-os szektorba irjuk a programot (64+128 kB) */
#define FLASH_START_ADDR      0x08010000
#define FLASH_END_ADDR        0x08040000
#define FIRST_SECTOR          4
#define LAST_SECTOR           5

void usbOnConnect(void){
	ledOn(LED_ORANGE);
}

void usbOnClassActive(void){
	FATFS fs;
	FIL file;
	uint32_t size, sectorError, i, bytesRead;
	uint8_t buffer[4];
	FLASH_EraseInitTypeDef eraseStruct;
	/* fajl megnyitasa */
	ledOn(LED_GREEN);
	if(f_mount(&fs,(const TCHAR*)usbGetPath(),0)!=FR_OK) while(1) ;
	if(f_open(&file,"program.bin",FA_READ)!=FR_OK) while(1) ;
	size=f_size(&file);
	if(size>(FLASH_END_ADDR-FLASH_START_ADDR)) while(1) ;
	/* flash torlese */
	HAL_FLASH_Unlock();
	eraseStruct.TypeErase=FLASH_TYPEERASE_SECTORS;
	eraseStruct.Sector=FIRST_SECTOR;
	eraseStruct.NbSectors=LAST_SECTOR-FIRST_SECTOR+1;
	eraseStruct.Banks=0;
	eraseStruct.VoltageRange=FLASH_VOLTAGE_RANGE_3;
	HAL_FLASHEx_Erase(&eraseStruct,&sectorError);
	__HAL_FLASH_DATA_CACHE_DISABLE();
	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
	__HAL_FLASH_DATA_CACHE_RESET();
	__HAL_FLASH_INSTRUCTION_CACHE_RESET();
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_DATA_CACHE_ENABLE();
	/* fajl masolasa a flashbe 32 bites hozzaferesekkel (bajtonkent lassabb) */
	for(i=0;i<size;i+=4){
		if((i+4)<=size){
			f_read(&file,buffer,4,(UINT*)&bytesRead);
		}
		else{
			f_read(&file,buffer,size-i,(UINT*)&bytesRead);
			memset(&buffer[size-i],0xFF,i+4-size);
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_START_ADDR+i,*((uint32_t*)buffer));
	}
	HAL_FLASH_Lock();
	f_close(&file);
	ledOn(LED_BLUE);
}

void usbOnDisconnect(void){
	ledOff(LED_ORANGE|LED_GREEN|LED_BLUE);
}

void jumpToApp(uint32_t addr){
	__asm(
		"ldr sp, [r0]       \n"
		"ldr pc, [r0, #4]   \n"
	);
}

int main(void){
	buttonInit();
	if(buttonRead()){ //ha nyomva tartott gombbal reseteltunk, akkor inditjuk a bootloadert
		HAL_Init();
		configSystemClock();
		ledInit();
		usbInit();
		while(1){
			usbHostProcess();
		}
	}
	else{ //kulonben betoltjuk a programot
		jumpToApp(FLASH_START_ADDR);
	}
	return 1;
}
