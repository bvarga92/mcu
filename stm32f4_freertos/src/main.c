#include "stm32f4xx_hal.h"
#include "system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* taszkok veremteruleteinek merete (hany szo) */
#define STACK_SIZE 128

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
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	/* tovabbi taszkok es szemaforok letrehozasa */
	s=xSemaphoreCreateBinary();
	xTaskCreate(task1,"Blinker Task 1",STACK_SIZE,NULL,TASK1_PRIO,NULL);
	xTaskCreate(task2,"Semaphore Task",STACK_SIZE,NULL,TASK2_PRIO,NULL);
	xTaskCreate(task3,"Blinker Task 2",STACK_SIZE,NULL,TASK3_PRIO,NULL);
	/* megszunteti magat */
	vTaskDelete(NULL);
}

/* villogtat ket LED-et */
void task1(void *pvParameters){
	while(1){
		ledToggle(LED_GREEN|LED_RED);
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}

/* gombnyomasra odaadja a szemafort */
void task2(void *pvParameters){
	while(!buttonRead()) vTaskDelay(20/portTICK_RATE_MS);
	xSemaphoreGive(s);
	vTaskDelete(NULL);
}

/* var a szemaforra, majd villogtatja a masik ket LED-et */
void task3(void *pvParameters){
	if(xSemaphoreTake(s,portMAX_DELAY)){
		while(1){
			ledToggle(LED_ORANGE|LED_BLUE);
			vTaskDelay(200/portTICK_RATE_MS);
		}
	}
}
