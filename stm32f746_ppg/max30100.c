#include "max30100.h"

static I2C_HandleTypeDef i2cHandle;

static uint8_t max30100ReadReg(uint8_t reg){
	uint8_t temp;
	HAL_I2C_Master_Transmit(&i2cHandle,0xAE,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2cHandle,0xAE,&temp,1,1000);
	return temp;
}

static void max30100WriteReg(uint8_t reg, uint8_t data){
	uint8_t temp[2]={reg,data};
	HAL_I2C_Master_Transmit(&i2cHandle,0xAE,temp,2,1000);
}

void max30100Init(void){
	GPIO_InitTypeDef portInit;
	RCC_PeriphCLKInitTypeDef clkInit;
	/* GPIO beallitasa (PB8 - SCL, PB9 - SDA) */
	__GPIOB_CLK_ENABLE();
	__I2C1_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_8;
	portInit.Mode=GPIO_MODE_AF_OD;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Alternate=GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB,&portInit);
	portInit.Pin=GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* orajel beallitasa*/
	clkInit.PeriphClockSelection=RCC_PERIPHCLK_I2C1;
	clkInit.I2c1ClockSelection=RCC_I2C1CLKSOURCE_PCLK1;
	HAL_RCCEx_PeriphCLKConfig(&clkInit);
	/* I2C1 konfiguralasa (100 kbps) */
	i2cHandle.Instance=I2C1;
	i2cHandle.Init.Timing=0x40912732;
	i2cHandle.Init.OwnAddress1=0;
	i2cHandle.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.OwnAddress2=0;
	i2cHandle.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2cHandle);
	HAL_I2CEx_ConfigAnalogFilter(&i2cHandle,I2C_ANALOGFILTER_ENABLE);
	HAL_Delay(200);/* device ID ellenorzese */
	while(max30100ReadReg(0xFF)!=0x11) ;
	/* konfiguracio */
	max30100WriteReg(0x06,0x03); //mod: SpO2+HR
	max30100WriteReg(0x07,0x47); //mintaveteli frekvencia: 100 Hz, impulzusszelesseg: 1.6 ms, ADC felbontas: 16 bit, hi-res mod
	max30100WriteReg(0x09,0x8F); //voros: 27.1 mA, IR: 50 mA
}

float max30100ReadTemp(void){
	max30100WriteReg(0x06,max30100ReadReg(0x06)|0x08);
	while(max30100ReadReg(0x06)&0x08) ;
	return (int8_t)max30100ReadReg(0x16)+0.0625f*max30100ReadReg(0x17);
}

void max30100ResetFIFO(void){
	max30100WriteReg(0x02,0);
	max30100WriteReg(0x03,0);
	max30100WriteReg(0x04,0);
}

bool max30100FifoEmpty(void){
	return max30100ReadReg(0x02)==max30100ReadReg(0x04);
}

void max30100ReadFIFO(uint16_t* ledRed, uint16_t* ledIR){
	uint8_t buf[4], reg=0x05;
	HAL_I2C_Master_Transmit(&i2cHandle,0xAE,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2cHandle,0xAE,buf,4,1000);
	*ledIR =(buf[0]<<8)|buf[1];
	*ledRed=(buf[2]<<8)|buf[3];
}
