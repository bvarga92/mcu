#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_cortex.h"
#include "system.h"
#include "lcd.h"
#include "graphics.h"
#include "courier20b_ext.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include <math.h>

/* taszkok veremteruleteinek merete (2048 bajt = 512 szo) */
#define STACK_BYTES(n) (n>>2)
#define STACK_SIZE STACK_BYTES(2048)

/* taszkok prioritasai (nagyobb szam=magasabb prioritas) */
#define TASK0_PRIO  3
#define TASK1_PRIO  2
#define TASK2_PRIO  1
#define TASK3_PRIO  0

/* taszkok prototipusai */
void task0(void*);
void task1(void*);
void task2(void*);
void task3(void*);

/* egy szemafor */
SemaphoreHandle_t s;

/* belepesi pont, inditja az oprendszert */
int main(void){
	xTaskCreate(task0,"Init Task",STACK_SIZE,NULL,TASK0_PRIO,NULL);
	vTaskStartScheduler(); // ez sosem ter vissza
	return 1;
}

/* inicializalasokat vegzo taszk */
void task0(void *pvParameters){
	/* hardver inicializalasa */
	configMPU();
	enableCache();
	HAL_Init();
	configSystemClock();
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	ledInit();
	buttonInit();
	lcdInit();
	lcdInitLayers();
	lcdSetLayer2();
	gFill(COLOR_WHITE);
	/* tovabbi taszkok es szemaforok letrehozasa */
	s=xSemaphoreCreateBinary();
	xTaskCreate(task1,"Blinker Task",STACK_SIZE,NULL,TASK1_PRIO,NULL);
	xTaskCreate(task2,"Semaphore Task",STACK_SIZE,NULL,TASK2_PRIO,NULL);
	xTaskCreate(task3,"LCD Task",STACK_SIZE,NULL,TASK3_PRIO,NULL);
	/* megszunteti magat */
	vTaskDelete(NULL);
}

/* villogtatja a LED-et */
void task1(void *pvParameters){
	while(1){
		ledToggle();
		vTaskDelay(200);
	}
}

/* gombnyomasra odaadja a szemafort */
void task2(void *pvParameters){
	while(!buttonRead()) vTaskDelay(20);
	xSemaphoreGive(s);
	vTaskDelete(NULL);
}

/* ir a kijelzore */
void task3(void *pvParameters){
	static uint32_t cntr=1;
	gPutString(5,8,"Hello from Task3! :)",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(5,48,"Waiting for semaphore...",courier20b_ext,20,COLOR_RED,COLOR_WHITE);
	if(xSemaphoreTake(s,portMAX_DELAY)){
		gPutString(5,48,"Semaphore is released!  ",courier20b_ext,20,COLOR_GREEN,COLOR_WHITE);
		while(1){
			char str[8]; //lokalis valtozoknak leheto legszukebb scope --> kisebb stack
			sprintf(str,"%lu",cntr); //jo nagy stack kell neki :(
			gPutString(240-7*(uint8_t)(log10f(cntr)+1),126,str,courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
			cntr++;
			vTaskDelay(1000);
		}
	}
}
