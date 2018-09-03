#include "lidar.h"
#include "stm32f1xx_hal_i2c.h"

I2C_HandleTypeDef i2c1;

static uint8_t lidarReadReg(uint8_t reg){
	uint8_t temp;
	HAL_I2C_Master_Transmit(&i2c1,0xC4,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2c1,0xC4,&temp,1,1000);
	return temp;
}

static void lidarWriteReg(uint8_t reg, uint8_t data){
	uint8_t temp[2]={reg,data};
	HAL_I2C_Master_Transmit(&i2c1,0xC4,temp,2,1000);
}

void lidarInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOB_CLK_ENABLE();
	__I2C1_CLK_ENABLE();
	__AFIO_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_8|GPIO_PIN_9;
	portInit.Mode=GPIO_MODE_AF_OD;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB,&portInit);
	__HAL_AFIO_REMAP_I2C1_ENABLE();
	i2c1.Instance=I2C1;
	i2c1.Init.ClockSpeed=100000;
	i2c1.Init.DutyCycle=I2C_DUTYCYCLE_2;
	i2c1.Init.OwnAddress1=0;
	i2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	i2c1.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	i2c1.Init.OwnAddress2=0;
	i2c1.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	i2c1.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c1);
	HAL_Delay(30);
	lidarWriteReg(0x02,0x80);
	lidarWriteReg(0x04,0x08);
	lidarWriteReg(0x1C,0x00);
}

uint16_t lidarGetDistanceCM(void){
	uint8_t temp[2], reg=0x8F;
	lidarWriteReg(0x00,0x04);
	while(lidarReadReg(0x01)&0x01) ;
	HAL_I2C_Master_Transmit(&i2c1,0xC4,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2c1,0xC4,temp,2,1000);
	return (temp[0]<<8)|temp[1];
}
