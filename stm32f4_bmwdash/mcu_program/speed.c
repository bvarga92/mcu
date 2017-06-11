#include "speed.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_tim.h"

static uint8_t speed=0;

void speedInit(void){
	GPIO_InitTypeDef portInit;
	TIM_HandleTypeDef speedTimer;
	TIM_OC_InitTypeDef speedTimerOC;
	__GPIOA_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_5;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA,&portInit);
	__TIM2_CLK_ENABLE();
	speedTimer.Instance=TIM2;
	speedTimer.Channel=TIM_CHANNEL_1;
	speedTimer.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	speedTimer.Init.CounterMode=TIM_COUNTERMODE_UP;
	speedTimer.Init.Prescaler=0; //alapfrekvencia 84MHz
	speedTimer.Init.Period=9999; //sebesseg=f/6.91=6078147.612/(period+1)
	HAL_TIM_OC_Init(&speedTimer);
	speedTimerOC.Pulse=0;
	speedTimerOC.OCMode=TIM_OCMODE_TOGGLE;
	speedTimerOC.OCPolarity=TIM_OCPOLARITY_HIGH;
	speedTimerOC.OCNPolarity=TIM_OCNPOLARITY_HIGH;
	speedTimerOC.OCIdleState=TIM_OCIDLESTATE_SET;
	speedTimerOC.OCNIdleState=TIM_OCNIDLESTATE_RESET;
	speedTimerOC.OCFastMode=TIM_OCFAST_DISABLE;
	HAL_TIM_OC_ConfigChannel(&speedTimer,&speedTimerOC,TIM_CHANNEL_1);
	HAL_TIM_OC_Start(&speedTimer,TIM_CHANNEL_1);
}

void speedSet(uint8_t spd){
	speed=spd;
	TIM2->ARR=(spd==0)?0:((uint32_t)(6078147.612f/spd-1));
	TIM2->CNT=0;
}

uint8_t speedGet(void){
	return speed;
}
