/********************************************************/
/*                                                      */
/* uC/OS-II tesztprojekt a T-Bird 2 fejlesztokartyara.  */
/* Ket taszk 4-4 LED-et villogtat eltero frekvenciaval. */
/*                                                      */
/********************************************************/

#include "ucos_src/includes.h"

/* prioritasok (alacsonyabb szam --> magasabb prioritas) */
#define TASK1_PRIO 14
#define TASK2_PRIO 15
#define TASK3_PRIO 16

/* taszkok veremteruleteinek lefoglalasa */
#define TASK_STK_SIZE 128
OS_STK Task1Stk[TASK_STK_SIZE];
OS_STK Task2Stk[TASK_STK_SIZE];
OS_STK Task3Stk[TASK_STK_SIZE];

/* taszkok prototipusai */
void Task1(void*);
void Task2(void*);
void Task3(void*);
int main(void);

/* az OS inditasat vegzo foprogram */
int main(void){
	OS_ENTER_CRITICAL();
	OSInit();
	OSTaskCreate(Task1,0,&Task1Stk[TASK_STK_SIZE-1],TASK1_PRIO);
	OS_EXIT_CRITICAL();
	OSStart(); //ez a fuggveny soha nem ter vissza
	return 0;
}

/* inicializalast vegzo single-shot taszk */
void Task1(void* data) {
	/* system tick-ek gyakorisaganak beallitasa (Timer0) */
	TCCR0=0x07;
	TIMSK=_BV(TOIE0);
	TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/1024);
	/* labak adatiranyanak definialasa (2x4 LED) */
	DDRB=0xF0;
	DDRD=0xF0;
	/* inicializalas kesz, indithatjuk a tobbi taszkot */
	OSTaskCreate(Task2,0,&Task2Stk[TASK_STK_SIZE-1],TASK2_PRIO);
	OSTaskCreate(Task3,0,&Task3Stk[TASK_STK_SIZE-1],TASK3_PRIO);
	/* ez egy single-shot taszk volt, torli magat */
	OSTaskDel(OS_PRIO_SELF);
}

/* az also 4 LED villogtatasat vegzo taszk */
void Task2(void* data){
	while(1){
		PORTB=0xF0;
		OSTimeDlyHMSM(0,0,0,500);
		PORTB=0x00;
		OSTimeDlyHMSM(0,0,0,500);
	}
}

/* a felso 4 LED villogtatasat vegzo taszk */
void Task3(void* data){
	while(1){
		PORTD=0xF0;
		OSTimeDlyHMSM(0,0,0,200);
		PORTD=0x00;
		OSTimeDlyHMSM(0,0,0,200);
	}
}
