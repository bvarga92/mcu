#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "systick.h"

void SysTick_Handler(void){
	HAL_IncTick();
}
