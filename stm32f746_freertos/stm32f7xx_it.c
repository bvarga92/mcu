#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_cortex.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void){
	xPortSysTickHandler();
	HAL_IncTick();
}
