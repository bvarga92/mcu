#include "buzzer_button.h"
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef timer1;

void btnBzrInit(void){
	GPIO_InitTypeDef portInit;
	TIM_OC_InitTypeDef sConfigOC;
	/* nyomogomb */
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_12;
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* buzzer PWM 1 kHz 50% (TIM1/CH1) */
	__GPIOA_CLK_ENABLE();
	__TIM1_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_8;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA,&portInit);
	timer1.Instance=TIM1;
	timer1.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	timer1.Init.CounterMode=TIM_COUNTERMODE_UP;
	timer1.Init.Period=2249;
	timer1.Init.Prescaler=31;
	timer1.Init.RepetitionCounter=0;
	HAL_TIM_PWM_Init(&timer1);
	sConfigOC.OCMode=TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity=TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode=TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&timer1,&sConfigOC,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&timer1,TIM_CHANNEL_1);
	buzzerOff();
}

uint8_t btnRead(void){
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==GPIO_PIN_RESET)
		return 1;
	else
		return 0;
}

void buzzerOn(void){
	TIM1->CCR1=1125;
}

void buzzerOff(void){
	TIM1->CCR1=0;
}
