#include "spi.h"
#include "stm32f1xx_hal_spi.h"

static SPI_HandleTypeDef spi2;

void spiInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_13|GPIO_PIN_15;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&portInit);
	portInit.Pin=GPIO_PIN_14;
	portInit.Mode=GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB,&portInit);
	portInit.Pin=GPIO_PIN_12;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB,&portInit);
	__SPI2_CLK_ENABLE();
	spi2.Instance=SPI2;
	spi2.Init.Mode=SPI_MODE_MASTER;
	spi2.Init.Direction=SPI_DIRECTION_2LINES;
	spi2.Init.DataSize=SPI_DATASIZE_16BIT;
	spi2.Init.CLKPolarity=SPI_POLARITY_LOW;
	spi2.Init.CLKPhase=SPI_PHASE_1EDGE;
	spi2.Init.NSS=SPI_NSS_SOFT;
	spi2.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256; //140.625 kHz
	spi2.Init.FirstBit=SPI_FIRSTBIT_MSB;
	spi2.Init.TIMode=SPI_TIMODE_DISABLE;
	spi2.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
	spi2.Init.CRCPolynomial=10;
	HAL_SPI_Init(&spi2);
}

void spiTransmit16Bit(uint8_t MSByte, uint8_t LSByte){
	uint8_t data[2]={LSByte,MSByte};
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi2,data,1,1000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
}
