#include "kbus.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_uart.h"

static UART_HandleTypeDef kbusUART;
static uint8_t lampState=0, lampError=0;
static uint8_t buffer[9];

void USART1_IRQHandler(){
	HAL_UART_IRQHandler(&kbusUART);
}

void kbusInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_6;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	portInit.Alternate=GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOB,&portInit);
	__USART1_CLK_ENABLE();
	kbusUART.Instance=USART1;
	kbusUART.Init.BaudRate=9600;
	kbusUART.Init.WordLength=UART_WORDLENGTH_9B; //ha paritasbit is van, akkor ez adatbitek szama +1 !!!
	kbusUART.Init.StopBits=UART_STOPBITS_1;
	kbusUART.Init.Parity=UART_PARITY_EVEN;
	kbusUART.Init.Mode=UART_MODE_TX;
	kbusUART.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	kbusUART.Init.OverSampling=UART_OVERSAMPLING_16;
	HAL_UART_Init(&kbusUART);
	HAL_NVIC_SetPriority(USART1_IRQn,1,0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

static void kbusUpdateLampState(void){
	while(HAL_UART_GetState(&kbusUART)!=HAL_UART_STATE_READY) ;
	buffer[0]=0xD0; //forras
	buffer[1]=0x07; //hossz (kovetkezo bajtok szama)
	buffer[2]=0xBF; //cel
	buffer[3]=0x5B;
	buffer[4]=lampState; //index (0x20: bal, 0x40: jobb), kodlampa (0x10: hatso, 0x08: elso), tavolsagi (0x04)
	buffer[5]=0x00;
	buffer[6]=0x00;
	buffer[7]=lampError; //lampa hiba (0x04: jobb hatso, 0x08: bal hatso, 0x10: jobb elso, 0x20: bal elso)
	buffer[8]=0x33^lampState^lampError; //checksum (minden bajt bitenkent osszeXORolva)
	HAL_UART_Transmit_IT(&kbusUART,buffer,9);
}

void kbusLeftSignalOn(void){lampState|=0x20; kbusUpdateLampState();}
void kbusLeftSignalOff(void){lampState&=~0x20; kbusUpdateLampState();}
void kbusRightSignalOn(void){lampState|=0x40; kbusUpdateLampState();}
void kbusRightSignalOff(void){lampState&=~0x40; kbusUpdateLampState();}
void kbusFogFrontOn(void){lampState|=0x08; kbusUpdateLampState();}
void kbusFogFrontOff(void){lampState&=~0x08; kbusUpdateLampState();}
void kbusFogRearOn(void){lampState|=0x10; kbusUpdateLampState();}
void kbusFogRearOff(void){lampState&=~0x10; kbusUpdateLampState();}
void kbusHighBeamOn(void){lampState|=0x04; kbusUpdateLampState();}
void kbusHighBeamOff(void){lampState&=~0x04; kbusUpdateLampState();}
void kbusErrorFLOn(void){lampError|=0x20; kbusUpdateLampState();}
void kbusErrorFLOff(void){lampError&=~0x20; kbusUpdateLampState();}
void kbusErrorFROn(void){lampError|=0x10; kbusUpdateLampState();}
void kbusErrorFROff(void){lampError&=~0x10; kbusUpdateLampState();}
void kbusErrorRLOn(void){lampError|=0x08; kbusUpdateLampState();}
void kbusErrorRLOff(void){lampError&=~0x08; kbusUpdateLampState();}
void kbusErrorRROn(void){lampError|=0x04; kbusUpdateLampState();}
void kbusErrorRROff(void){lampError&=~0x04; kbusUpdateLampState();}

void kbusSetDisplayBacklight(uint8_t bl){
	while(HAL_UART_GetState(&kbusUART)!=HAL_UART_STATE_READY) ;
	buffer[0]=0xD0; //forras
	buffer[1]=0x07; //hossz (kovetkezo bajtok szama)
	buffer[2]=0xBF; //cel
	buffer[3]=0x5C;
	buffer[4]=0x00;
	buffer[5]=bl; //kijelzo fenyero (0x00-0xFF)
	buffer[6]=0x00;
	buffer[7]=0x00;
	buffer[8]=0x34^bl; //checksum (minden bajt bitenkent osszeXORolva)
	HAL_UART_Transmit_IT(&kbusUART,buffer,9);
}
