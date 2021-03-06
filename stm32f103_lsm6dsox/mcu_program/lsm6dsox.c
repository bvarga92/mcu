#include "lsm6dsox.h"
#include "lsm6dsox_conf.h"
#include "stm32f1xx_hal_i2c.h"

static I2C_HandleTypeDef i2c1;
static void (*lsmInt2RiseFcn)(void);
static void (*lsmInt2FallFcn)(void);

static uint8_t lsmReadReg(uint8_t reg){
	uint8_t temp;
	HAL_I2C_Master_Transmit(&i2c1,0xD6,&reg,1,1000);
	HAL_I2C_Master_Receive(&i2c1,0xD6,&temp,1,1000);
	return temp;
}

static void lsmWriteReg(uint8_t reg, uint8_t data){
	uint8_t temp[2]={reg,data};
	HAL_I2C_Master_Transmit(&i2c1,0xD6,temp,2,1000);
}

void lsmInit(bool configMLC, bool int2Latched, void (*int2RiseFcn)(void), void (*int2FallFcn)(void)){
	GPIO_InitTypeDef portInit;
	uint32_t i;
	uint8_t address, value;
	/* INT1 es INT2 bemenet, INT2-re kulso megszakitas */
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&portInit);
	portInit.Pin=GPIO_PIN_6;
	portInit.Mode=GPIO_MODE_IT_RISING_FALLING;
	HAL_GPIO_Init(GPIOB,&portInit);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,1);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	lsmInt2RiseFcn=int2RiseFcn;
	lsmInt2FallFcn=int2FallFcn;
	/* I2C */
	__HAL_AFIO_REMAP_I2C1_ENABLE();
	__I2C1_CLK_ENABLE();
	__AFIO_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_8|GPIO_PIN_9;
	portInit.Mode=GPIO_MODE_AF_OD;
	portInit.Speed=GPIO_SPEED_HIGH;
	portInit.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB,&portInit);
	i2c1.Instance=I2C1;
	i2c1.Init.ClockSpeed=400000; //400 kbps
	i2c1.Init.DutyCycle=I2C_DUTYCYCLE_2;
	i2c1.Init.OwnAddress1=0;
	i2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	i2c1.Init.DualAddressMode=I2C_DUALADDRESS_DISABLE;
	i2c1.Init.OwnAddress2=0;
	i2c1.Init.GeneralCallMode=I2C_GENERALCALL_DISABLE;
	i2c1.Init.NoStretchMode=I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c1);
	HAL_Delay(30);
	/* szenzor konfiguralasa */
	while(lsmReadReg(0x0F)!=0x6C) ; //WHO_AM_I regiszter ellenorzese
	if(configMLC){ //Machine Learning Core felkonfiguralasa
		for(i=0;i<CONF_SIZE;i++){
			address=conf[2*i];
			value=conf[2*i+1];
			if(address==0x00)
				HAL_Delay(value);
			else
				lsmWriteReg(address,value);
		}
		lsmWriteReg(0x5F,0x02); //EMB_FUNC esemenyek az INT2 kimeneten
		lsmWriteReg(0x01,0x80); //EMB_FUNC regiszterek kivalasztasa
		lsmWriteReg(0x11,0x01); //MLC1 megszakitas az INT2 kimenetre
		if(int2Latched) lsmWriteReg(0x17,0x80); //EMB_FUNC megszakitasok latch-elve
		lsmWriteReg(0x01,0x00); //EMB_FUNC regiszterek letiltasa
	}
	else{ //MLC kikapcsolva, csak minimalis mukodes
		lsmWriteReg(0x10,0x24); //gyorsulasmero: 26 Hz, 16 g
		lsmWriteReg(0x11,0x2C); //giroszkop: 26 Hz, 2000 dps
	}
}

void EXTI9_5_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)==GPIO_PIN_SET)
		lsmInt2RiseFcn();
	else
		lsmInt2FallFcn();
}

bool lsmReadINT2(void){
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)==GPIO_PIN_SET)
		return true;
	else
		return false;
}

void lsmClearINT2(void){
	lsmWriteReg(0x01,0x80); //EMB_FUNC regiszterek kivalasztasa
	lsmReadReg(0x15); //MLC statusz kiolvasasa
	lsmWriteReg(0x01,0x00); //EMB_FUNC regiszterek letiltasa
}

void lsmGetData(lsm_data_t *dataOut){
	while(lsmReadReg(0x1E)!=0x07) ; //varunk, amig lesz kiolvashato adat
	dataOut->temp =(lsmReadReg(0x21)<<8) | lsmReadReg(0x20);
	dataOut->acc_x=(lsmReadReg(0x29)<<8) | lsmReadReg(0x28);
	dataOut->acc_y=(lsmReadReg(0x2B)<<8) | lsmReadReg(0x2A);
	dataOut->acc_z=(lsmReadReg(0x2D)<<8) | lsmReadReg(0x2C);
	dataOut->gyr_x=(lsmReadReg(0x23)<<8) | lsmReadReg(0x22);
	dataOut->gyr_y=(lsmReadReg(0x25)<<8) | lsmReadReg(0x24);
	dataOut->gyr_z=(lsmReadReg(0x27)<<8) | lsmReadReg(0x26);
}
