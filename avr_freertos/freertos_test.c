/********************************************************/
/*                                                      */
/* FreeRTOS tesztprojekt a T-Bird 2 fejlesztokartyara.  */
/* Ket taszk 4-4 LED-et villogtat eltero frekvenciaval. */
/*                                                      */
/********************************************************/

#include <avr/io.h>
#include "freertos_src/kernel/FreeRTOS.h"
#include "freertos_src/kernel/task.h"
#include "freertos_src/kernel/timers.h"
#include "freertos_src/kernel/semphr.h"

void Task1(void*);
void Task2(void*);
void Task3(void*);
int main(void);

/* az OS inditasat vegzo foprogram */
int main(void){
	xTaskCreate(Task1,"InitTask",configMINIMAL_STACK_SIZE,0,tskIDLE_PRIORITY+1,0);
	vTaskStartScheduler(); //ez a fuggveny soha nem ter vissza
	return 0;
}

/* inicializalast vegzo single-shot taszk */
void Task1(void *pvParameters){
	DDRB=0xF0;
	DDRD=0xF0;
	xTaskCreate(Task2,"Villogo1",configMINIMAL_STACK_SIZE,0,tskIDLE_PRIORITY+2,0);
	xTaskCreate(Task3,"Villogo2",configMINIMAL_STACK_SIZE,0,tskIDLE_PRIORITY+3,0);
	vTaskDelete(0);
}

/* also 4 LED villogtatasat vegzo taszk */
void Task2(void *pvParameters){
	while(1){
		PORTB=0xF0;
		vTaskDelay(500/portTICK_RATE_MS);
		PORTB=0x00;
		vTaskDelay(500/portTICK_RATE_MS);
	}
}

/* felso 4 LED villogtatasat vegzo taszk */
void Task3(void *pvParameters){
	while(1){
		PORTD=0xF0;
		vTaskDelay(200/portTICK_RATE_MS);
		PORTD=0x00;
		vTaskDelay(200/portTICK_RATE_MS);
	}
}
