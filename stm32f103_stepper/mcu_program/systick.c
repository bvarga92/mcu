#include "systick.h"
#include "stm32f1xx_hal.h"

void SysTick_Handler(void){
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
