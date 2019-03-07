#include "buzzer_button.h"
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef timer1, timer2;

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
	/* buzzer PWM 1 kHz (TIM1/CH1) */
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
	timer1.Init.RepetitionCounter=0;
	timer1.Init.Period=2249; //32 kHz alaporajel
	timer1.Init.Prescaler=31; //1 kHz
	HAL_TIM_PWM_Init(&timer1);
	sConfigOC.OCMode=TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity=TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode=TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&timer1,&sConfigOC,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&timer1,TIM_CHANNEL_1);
	buzzerOff();
	/* idozito az 1 masodperces sipolashoz (TIM2) */
	__TIM2_CLK_ENABLE();
	timer2.Instance=TIM2;
	timer2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	timer2.Init.CounterMode=TIM_COUNTERMODE_UP;
	timer2.Init.RepetitionCounter=0;
	timer2.Init.Prescaler=35999; //2 kHz alaporajel
	timer2.Init.Period=1999; //1 Hz
	HAL_TIM_Base_Init(&timer2);
	HAL_NVIC_SetPriority(TIM2_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&timer2);
	TIM2->CR1&=(~((uint32_t)TIM_CR1_CEN));
	TIM2->CNT=0;
}

void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&timer2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance!=TIM2) return;
	buzzerOff();
	TIM2->CR1&=(~((uint32_t)TIM_CR1_CEN));
	TIM2->CNT=0;
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

void buzzerBeep(void){
	buzzerOn();
	TIM2->CR1|=TIM_CR1_CEN;
}
