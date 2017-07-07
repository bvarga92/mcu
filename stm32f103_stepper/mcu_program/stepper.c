#include "stepper.h"
#include "stm32f1xx_hal_tim.h"

static TIM_HandleTypeDef timer1;
static int16_t motCntr[3]={0,0,0};

/* 2038 lepes egy teljes fordulat, kis nyomatek */
static const uint8_t sequenceWaveDrive[4][8]={{1,0,0,0,1,0,0,0},
                                              {0,1,0,0,0,1,0,0},
                                              {0,0,1,0,0,0,1,0},
                                              {0,0,0,1,0,0,0,1}};

/* 2038 lepes egy teljes fordulat, nagy nyomatek */
static const uint8_t sequenceFullStep[4][8]= {{1,0,0,1,1,0,0,1},
                                              {1,1,0,0,1,1,0,0},
                                              {0,1,1,0,0,1,1,0},
                                              {0,0,1,1,0,0,1,1}};

/* 4076 lepes egy teljes fordulat, kozepes nyomatek */
static const uint8_t sequenceHalfStep[4][8]= {{1,1,0,0,0,0,0,1},
                                              {0,1,1,1,0,0,0,0},
                                              {0,0,0,1,1,1,0,0},
                                              {0,0,0,0,0,1,1,1}};

/* pointer az eppen hasznalt szekvenciara */
static const uint8_t (*sequence)[4][8];

void TIM1_UP_IRQHandler(void){
	HAL_TIM_IRQHandler(&timer1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static uint8_t i[3]={0,0,0};
	if(htim->Instance!=TIM1) return;
	if(motCntr[0]!=0){
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN1,(*sequence)[0][i[0]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN2,(*sequence)[1][i[0]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN3,(*sequence)[2][i[0]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN4,(*sequence)[3][i[0]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		if(motCntr[0]>0){
			i[0]=(i[0]+1)%8;
			motCntr[0]--;
		}
		else{
			i[0]=i[0]?(i[0]-1):7;
			motCntr[0]++;
		}
	}
	else{
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT1_PORT,MOT1_PIN4,GPIO_PIN_RESET);
	}
	if(motCntr[1]!=0){
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN1,(*sequence)[0][i[1]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN2,(*sequence)[1][i[1]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN3,(*sequence)[2][i[1]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN4,(*sequence)[3][i[1]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		if(motCntr[1]>0){
			i[1]=(i[1]+1)%8;
			motCntr[1]--;
		}
		else{
			i[1]=i[1]?(i[1]-1):7;
			motCntr[1]++;
		}
	}
	else{
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT2_PORT,MOT2_PIN4,GPIO_PIN_RESET);
	}
	if(motCntr[2]!=0){
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN1,(*sequence)[0][i[2]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN2,(*sequence)[1][i[2]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN3,(*sequence)[2][i[2]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN4,(*sequence)[3][i[2]]?GPIO_PIN_SET:GPIO_PIN_RESET);
		if(motCntr[2]>0){
			i[2]=(i[2]+1)%8;
			motCntr[2]--;
		}
		else{
			i[2]=i[2]?(i[2]-1):7;
			motCntr[2]++;
		}
	}
	else{
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOT3_PORT,MOT3_PIN4,GPIO_PIN_RESET);
	}
}

void stepperInit(void){
	GPIO_InitTypeDef portInit;
	/* GPIO */
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	portInit.Pin=MOT1_PIN1|MOT1_PIN2|MOT1_PIN3|MOT1_PIN4;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOT1_PORT,&portInit);
	portInit.Pin=MOT2_PIN1|MOT2_PIN2|MOT2_PIN3|MOT2_PIN4;
	HAL_GPIO_Init(MOT2_PORT,&portInit);
	portInit.Pin=MOT3_PIN1|MOT3_PIN2|MOT3_PIN3|MOT3_PIN4;
	HAL_GPIO_Init(MOT3_PORT,&portInit);
	portInit.Pin=GPO_RL_PIN|GPO_TR_PIN;
	HAL_GPIO_Init(GPO_PORT,&portInit);
	stepperRelayOff();
	stepperTransistorOff();
	/* timer */
	__HAL_RCC_TIM1_CLK_ENABLE();
	timer1.Instance=TIM1;
	timer1.Init.Prescaler=35999; //2 kHz alaporajel
	timer1.Init.CounterMode=TIM_COUNTERMODE_UP;
	timer1.Init.Period=3; //500 Hz
	timer1.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	timer1.Init.RepetitionCounter=0;
	HAL_TIM_Base_Init(&timer1);
	HAL_NVIC_SetPriority(TIM1_UP_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	HAL_TIM_Base_Start_IT(&timer1);
	/* alapertelmezett meghajtas: half step */
	stepperSetSequence(1);
}

void stepperRelayOn(void){
	HAL_GPIO_WritePin(GPO_PORT,GPO_RL_PIN,GPIO_PIN_SET);
}

void stepperRelayOff(void){
	HAL_GPIO_WritePin(GPO_PORT,GPO_RL_PIN,GPIO_PIN_RESET);
}

void stepperTransistorOn(void){
	HAL_GPIO_WritePin(GPO_PORT,GPO_TR_PIN,GPIO_PIN_SET);
}

void stepperTransistorOff(void){
	HAL_GPIO_WritePin(GPO_PORT,GPO_TR_PIN,GPIO_PIN_RESET);
}

void stepperStep(uint8_t motor, int16_t steps){
	if(motor!=1 && motor!=2 && motor!=3) return;
	motCntr[motor-1]=steps;
}

void stepperSetSequence(uint8_t s){
	switch(s){
		case 0: sequence=&sequenceWaveDrive; break;
		case 1: sequence=&sequenceHalfStep; break;
		case 2: sequence=&sequenceFullStep; break;
	}
}
