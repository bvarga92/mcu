#include "uart.h"
#include "stm32f1xx_hal_uart.h"

static UART_HandleTypeDef uart3;
static uint8_t buf;
static uint8_t fifo[FIFO_SIZE], fifoHead=0, fifoTail=0;

void USART3_IRQHandler(void){
	HAL_UART_IRQHandler(&uart3);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance!=USART3) return;
	uartFifoPut(buf);
	HAL_UART_Receive_IT(&uart3,&buf,1);
}

void uartInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOB_CLK_ENABLE();
	/* TX: PB10 */
	portInit.Pin=GPIO_PIN_10;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* RX: PB11 */
	portInit.Pin=GPIO_PIN_11;
	portInit.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* DSR: PB1 */
	portInit.Pin=GPIO_PIN_1;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* DTR: PB0 */
	portInit.Pin=GPIO_PIN_0;
	portInit.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* UART3 */
	__HAL_RCC_USART3_CLK_ENABLE();
	uart3.Instance=USART3;
	uart3.Init.BaudRate=4800;
	uart3.Init.WordLength=UART_WORDLENGTH_8B; //paritas is beleszamit!
	uart3.Init.StopBits=UART_STOPBITS_2;
	uart3.Init.Parity=UART_PARITY_NONE;
	uart3.Init.Mode=UART_MODE_TX_RX;
	uart3.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	uart3.Init.OverSampling=UART_OVERSAMPLING_16;
	HAL_UART_Init(&uart3);
	HAL_NVIC_SetPriority(USART3_IRQn,0,0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_UART_Receive_IT(&uart3,&buf,1);
	/* DSR=0 --> a muszer beszelhet */
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
}

void uartFifoPut(uint8_t data){
	if(fifoTail==(fifoHead+FIFO_SIZE-1)%FIFO_SIZE) return;
	fifo[fifoTail]=data;
	fifoTail=(fifoTail+1)%FIFO_SIZE;
}

uint8_t uartFifoGet(void){
	uint8_t temp;
	if(fifoHead==fifoTail) return 0;
	temp=fifo[fifoHead];
	fifoHead=(fifoHead+1)%FIFO_SIZE;
	return temp;
}

uint8_t uartFIFOIsEmpty(void){
	if(fifoHead==fifoTail)
		return 1;
	else
		return 0;
}

uint8_t uartFIFOIsFull(void){
	if(fifoTail==((fifoHead+FIFO_SIZE-1)%FIFO_SIZE))
		return 1;
	else
		return 0;
}
