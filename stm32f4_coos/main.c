#include "stm32f4xx_hal.h"
#include "system.h"
#include "CoOS.h"

/* taszkok prioritasai (nagyobb szam=magasabb prioritas) */
#define TASK0_PRIO 3
#define TASK1_PRIO 2
#define TASK2_PRIO 1
#define TASK3_PRIO 0

/* taszkok veremteruleteinek lefoglalasa (a meret 32 bites szoban ertendo) */
#define STACK_SIZE 128
OS_STK task0Stk[STACK_SIZE];
OS_STK task1Stk[STACK_SIZE];
OS_STK task2Stk[STACK_SIZE];
OS_STK task3Stk[STACK_SIZE];

/* taszkok prototipusai */
void task0(void*);
void task1(void*);
void task2(void*);
void task3(void*);

/* egy flag */
OS_FlagID f;

/* belepesi pont, inditja az oprendszert */
int main(void){
	CoInitOS();
	CoCreateTask(task0,0,TASK0_PRIO,&task0Stk[STACK_SIZE-1],STACK_SIZE);
	CoStartOS(); // ez sosem ter vissza
	return 1;
}

/* inicializalasokat vegzo taszk */
void task0(void* pdata){
	/* hardver inicializalasa */
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	/* tovabbi taszkok es flagek letrehozasa */
	f=CoCreateFlag(Co_TRUE,0);
	CoCreateTask(task1,0,TASK1_PRIO,&task1Stk[STACK_SIZE-1],STACK_SIZE);
	CoCreateTask(task2,0,TASK2_PRIO,&task2Stk[STACK_SIZE-1],STACK_SIZE);
	CoCreateTask(task3,0,TASK3_PRIO,&task3Stk[STACK_SIZE-1],STACK_SIZE);
	/* megszunteti magat magat */
	CoExitTask();
}

/* villogtat ket LED-et */
void task1(void* pdata){
	while(1){
		ledToggle(LED_GREEN|LED_RED);
		CoTimeDelay(0,0,1,0);
	}
}

/* gombnyomasra jelez a flaggel */
void task2(void* pdata){
	while(!buttonRead()) CoTimeDelay(0,0,0,20);
	CoSetFlag(f);
	CoExitTask();
}

/* var a flagre, majd villogtatja a masik ket LED-et */
void task3(void* pdata){
	CoWaitForSingleFlag(f,0);
	while(1){
		ledToggle(LED_ORANGE|LED_BLUE);
		CoTimeDelay(0,0,0,200);
	}
}
