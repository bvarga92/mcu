#include "can.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_tim.h"

static uint8_t coolantTemp=0; //hutoviz homerseklet=coolantTemp*0.75-48.373
static uint16_t rpm=0; //fordulatszam=rpm/6.4
static uint8_t warningLightStatus=0; //figyelmezteto lampak (motorhiba, tempomat, EML, tulmelegedes)
static uint8_t consumption=0; //fogyasztas=8*consumption/sebesseg
static CAN_HandleTypeDef can1;
static CanTxMsgTypeDef canTX;
static CanRxMsgTypeDef canRX;
static TIM_HandleTypeDef canTimer;

static void canSendSpeed(void){
	can1.pTxMsg->StdId= 0x0153; //csomag ID
	can1.pTxMsg->ExtId=0x0001; //extended ID, nem hasznaljuk
	can1.pTxMsg->IDE=CAN_ID_STD; //csak 11 bites ID
	can1.pTxMsg->RTR=CAN_RTR_DATA; //nem remote request
	can1.pTxMsg->DLC=8; //8 bajtos adat
	can1.pTxMsg->Data[0]=0x00;
	can1.pTxMsg->Data[1]=0x00; //LSB
	can1.pTxMsg->Data[2]=0x00; //MSB
	can1.pTxMsg->Data[3]=0x00;
	can1.pTxMsg->Data[4]=0x00;
	can1.pTxMsg->Data[5]=0x00;
	can1.pTxMsg->Data[6]=0x00;
	can1.pTxMsg->Data[7]=0x00;
	HAL_CAN_Transmit(&can1,1000);
}

static void canSendRPM(void){
	can1.pTxMsg->StdId= 0x0316; //csomag ID
	can1.pTxMsg->ExtId=0x0001; //extended ID, nem hasznaljuk
	can1.pTxMsg->IDE=CAN_ID_STD; //csak 11 bites ID
	can1.pTxMsg->RTR=CAN_RTR_DATA; //nem remote request
	can1.pTxMsg->DLC=8; //8 bajtos adat
	can1.pTxMsg->Data[0]=0x00;
	can1.pTxMsg->Data[1]=0x00;
	can1.pTxMsg->Data[2]=rpm&0x00FF; //RPM LSB
	can1.pTxMsg->Data[3]=rpm>>8; //RPM MSB
	can1.pTxMsg->Data[4]=0x00;
	can1.pTxMsg->Data[5]=0x00;
	can1.pTxMsg->Data[6]=0x00;
	can1.pTxMsg->Data[7]=0x00;
	HAL_CAN_Transmit(&can1,1000);
}

static void canSendCoolantTemp(void){
	can1.pTxMsg->StdId= 0x0329; //csomag ID
	can1.pTxMsg->ExtId=0x0001; //extended ID, nem hasznaljuk
	can1.pTxMsg->IDE=CAN_ID_STD; //csak 11 bites ID
	can1.pTxMsg->RTR=CAN_RTR_DATA; //nem remote request
	can1.pTxMsg->DLC=8; //8 bajtos adat
	can1.pTxMsg->Data[0]=0x00;
	can1.pTxMsg->Data[1]=coolantTemp; //homerseklet
	can1.pTxMsg->Data[2]=0x00;
	can1.pTxMsg->Data[3]=0x00;
	can1.pTxMsg->Data[4]=0x00;
	can1.pTxMsg->Data[5]=0x00; //gazpedal pozicio (0x00-0xFE)
	can1.pTxMsg->Data[6]=0x00; //fekpedal (0x01), kickdown (0x04)
	can1.pTxMsg->Data[7]=0x00;
	HAL_CAN_Transmit(&can1,1000);
}

static void canSendLightsConsumption(void){
	static uint16_t d=0;
	d+=consumption;
	can1.pTxMsg->StdId= 0x0545; //csomag ID
	can1.pTxMsg->ExtId=0x0001; //extended ID, nem hasznaljuk
	can1.pTxMsg->IDE=CAN_ID_STD; //csak 11 bites ID
	can1.pTxMsg->RTR=CAN_RTR_DATA; //nem remote request
	can1.pTxMsg->DLC=8; //8 bajtos adat
	can1.pTxMsg->Data[0]=warningLightStatus&0xFE;
	can1.pTxMsg->Data[1]=d&0x00FF; //fogyasztas LSB
	can1.pTxMsg->Data[2]=d>>8; //fogyasztas MSB
	can1.pTxMsg->Data[3]=(warningLightStatus&0x01)<<3;
	can1.pTxMsg->Data[4]=0x7E;
	can1.pTxMsg->Data[5]=0x10;
	can1.pTxMsg->Data[6]=0x00;
	can1.pTxMsg->Data[7]=0x18;
	HAL_CAN_Transmit(&can1,1000);
}

void canInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOD_CLK_ENABLE();
	__HAL_RCC_CAN1_CLK_ENABLE();
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF9_CAN1;
	portInit.Pin=GPIO_PIN_1|GPIO_PIN_0;
	HAL_GPIO_Init(GPIOD,&portInit);
	can1.Instance=CAN1;
	can1.Init.Mode=CAN_MODE_NORMAL;
	can1.Init.Prescaler=4; //tq=2/21 us
	can1.Init.SJW=CAN_SJW_3TQ;
	can1.Init.BS1=CAN_BS1_14TQ;
	can1.Init.BS2=CAN_BS2_6TQ;
	can1.pTxMsg=&canTX;
	can1.pRxMsg=&canRX;
	can1.Init.TTCM=DISABLE;
	can1.Init.ABOM=DISABLE;
	can1.Init.AWUM=DISABLE;
	can1.Init.NART=DISABLE;
	can1.Init.RFLM=DISABLE;
	can1.Init.TXFP=DISABLE;
	HAL_CAN_Init(&can1);
	/* Timer4: 10ms-onkent megszakitas */
	__TIM4_CLK_ENABLE();
	canTimer.Instance=TIM4;
	canTimer.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	canTimer.Init.CounterMode=TIM_COUNTERMODE_UP;
	canTimer.Init.Prescaler=999;
	canTimer.Init.Period=839;
	HAL_TIM_Base_Init(&canTimer);
	HAL_NVIC_SetPriority(TIM4_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_TIM_Base_Start_IT(&canTimer);
}

void TIM4_IRQHandler(void){
	HAL_TIM_IRQHandler(&canTimer);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	static uint8_t currentMessage=0;
	if(htim->Instance==TIM4){
		if(currentMessage==0){
			currentMessage=1;
			canSendSpeed();
		}
		else if(currentMessage==1){
			currentMessage=2;
			canSendRPM();
		}
		else if(currentMessage==2){
			currentMessage=3;
			canSendCoolantTemp();
		}
		else if(currentMessage==3){
			currentMessage=0;
			canSendLightsConsumption();
		}
	}
}

void canSetCoolantTemp(uint8_t temp){
	if(temp>142) return;
	coolantTemp=(uint8_t)((temp+48.373f)/0.75f);
}

void canSetRPM(uint16_t r){
	if(r>7000) return;
	rpm=(uint16_t)(r*6.4f);
}

void canLightCheckEngineOn(void){warningLightStatus|=0x02;}
void canLightCheckEngineOff(void){warningLightStatus&=~0x02;}
void canLightEMLOn(void){warningLightStatus|=0x10;}
void canLightEMLOff(void){warningLightStatus&=~0x10;}
void canLightOverheatOn(void){warningLightStatus|=0x01;}
void canLightOverheatOff(void){warningLightStatus&=~0x01;}
void canLightCruiseControlOn(void){warningLightStatus|=0x08;}
void canLightCruiseControlOff(void){warningLightStatus&=~0x08;}
