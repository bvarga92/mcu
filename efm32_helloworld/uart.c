#include "uart.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

volatile bool uartReceived=false;
volatile uint8_t uartRxChar;

void uartInit(void){
	USART_InitAsync_TypeDef uartInitStruct=USART_INITASYNC_DEFAULT;
	/* PF7 be --> UART0 engedelyezese a Board Controlleren */
	GPIO_PinModeSet(gpioPortF,7,gpioModePushPull,1);
	/* orajel */
	CMU_ClockEnable(cmuClock_UART0,true);
	/* 8N1 mod, 115200 bps */
	uartInitStruct.databits=usartDatabits8;
	uartInitStruct.parity=usartNoParity;
	uartInitStruct.stopbits=usartStopbits1;
	uartInitStruct.baudrate=115200;
	uartInitStruct.refFreq=0;
	uartInitStruct.mvdis=false;
	uartInitStruct.oversampling=usartOVS16;
	uartInitStruct.prsRxCh=0;
	uartInitStruct.prsRxEnable=false;
	uartInitStruct.enable=usartEnable;
	USART_InitAsync(UART0,&uartInitStruct);
	/* TX (PE0), RX (PE1) */
	GPIO_PinModeSet(gpioPortE,0,gpioModePushPull,1);
	GPIO_PinModeSet(gpioPortE,1,gpioModeInput,0);
	UART0->ROUTE|=USART_ROUTE_TXPEN|USART_ROUTE_RXPEN|USART_ROUTE_LOCATION_LOC1;
	/* veteli megszakitas */
	USART_IntEnable(UART0,USART_IF_RXDATAV);
	NVIC_EnableIRQ(UART0_RX_IRQn);
}

void uartPrint(char* str){
	uint32_t i=0;
	while(str[i]) USART_Tx(UART0,(uint8_t)str[i++]);
}

void UART0_RX_IRQHandler(void){
	uartReceived=true;
	uartRxChar=USART_RxDataGet(UART0);
}
