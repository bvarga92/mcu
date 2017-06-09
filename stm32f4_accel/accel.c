#include "accel.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "system.h"
#include "stm32f4xx_hal_spi.h"

static SPI_HandleTypeDef hSPI;
volatile uint8_t accelDataAvailable=0;

static void accelSend(uint8_t address, uint8_t data){
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hSPI,&address,1,100);
	HAL_SPI_Transmit(&hSPI,&data,1,100);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
}

static uint8_t accelRead(uint8_t address){
	uint8_t temp;
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
	address|=0x80;
	HAL_SPI_Transmit(&hSPI,&address,1,100);
	HAL_SPI_Receive(&hSPI,&temp,1,100);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	return temp;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==GPIO_PIN_0) accelDataAvailable=1;
}

void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void accelInit(void){
	GPIO_InitTypeDef portInit;
	/* GPIO */
	__GPIOA_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7; //SCK, MISO, MOSI
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA,&portInit);
	__GPIOE_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_3; //CS
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOE,&portInit);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	portInit.Pin=GPIO_PIN_0; //INT1/DRDY
	portInit.Mode=GPIO_MODE_IT_RISING;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOE,&portInit);
	HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	/* SPI1 */
	__SPI1_CLK_ENABLE();
	hSPI.Instance=SPI1;
	hSPI.Init.Mode=SPI_MODE_MASTER;
	hSPI.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16; //5.25 MHz
	hSPI.Init.Direction=SPI_DIRECTION_2LINES;
	hSPI.Init.CLKPhase=SPI_PHASE_2EDGE;
	hSPI.Init.CLKPolarity=SPI_POLARITY_HIGH;
	hSPI.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLED;
	hSPI.Init.DataSize=SPI_DATASIZE_8BIT;
	hSPI.Init.FirstBit=SPI_FIRSTBIT_MSB;
	hSPI.Init.NSS=SPI_NSS_SOFT;
	hSPI.Init.TIMode=SPI_TIMODE_DISABLED;
	HAL_SPI_Init(&hSPI);
	__HAL_SPI_ENABLE(&hSPI);
	/* a szenzor konfiguralasa */
	HAL_Delay(50);
	if(accelRead(0x0F)==0x3F){ //LIS3DSH (ujabb kiadas)
		accelSend(0x20,0x5F); //50 Hz data rate, block update mode, mindharom irany
		accelSend(0x23,0x88); //data ready jel engedelyezve
		accelSend(0x24,0x80); //400 Hz szuro, +/- 2G full scale
		accelSend(0x2E,0x00); //nincs FIFO
		accelSend(0x25,0x10); //autoincrement
	}
	else if(accelRead(0x0F)==0x3B){ //LIS302DL (regebbi kiadas)
		while(1) ; //nem tamogatjuk
	}
	else{
		while(1) ;
	}
}

void accelGetData(accel_t* pData){
	pData->x=(int16_t)((accelRead(0x29)<<8)|accelRead(0x28));
	pData->y=(int16_t)((accelRead(0x2B)<<8)|accelRead(0x2A));
	pData->z=(int16_t)((accelRead(0x2D)<<8)|accelRead(0x2C));
}
