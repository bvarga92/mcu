#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "systick.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void){
	xPortSysTickHandler();
	HAL_IncTick();
}
