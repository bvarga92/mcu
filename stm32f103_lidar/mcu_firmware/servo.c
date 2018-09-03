#include "servo.h"
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef timer2;

void servoInit(uint16_t horz, uint16_t vert){
	GPIO_InitTypeDef portInit;
	TIM_OC_InitTypeDef sConfigOC;
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__TIM2_CLK_ENABLE();
	__AFIO_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_3;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB,&portInit);
	portInit.Pin=GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA,&portInit);
	__HAL_AFIO_REMAP_SWJ_NOJTAG(); //PB3 miatt
	__HAL_AFIO_REMAP_TIM2_ENABLE();
	timer2.Instance=TIM2;
	timer2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	timer2.Init.CounterMode=TIM_COUNTERMODE_UP;
	timer2.Init.Period=44999;
	timer2.Init.Prescaler=31;
	timer2.Init.RepetitionCounter=0;
	HAL_TIM_PWM_Init(&timer2);
	sConfigOC.OCMode=TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity=TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode=TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&timer2,&sConfigOC,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&timer2,TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&timer2,&sConfigOC,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&timer2,TIM_CHANNEL_2);
	servoSet(horz,vert);
}

void servoSet(uint16_t horz, uint16_t vert){
	TIM2->CCR1=horz;
	TIM2->CCR2=vert;
}
