#include "audio.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_i2s.h"

static I2C_HandleTypeDef i2c;
static I2S_HandleTypeDef i2sOut, i2sIn;
volatile uint8_t outTransferComplete=0;
volatile uint8_t outTransferHalfComplete=0;
volatile uint8_t inTransferComplete=0;
volatile uint8_t inTransferHalfComplete=0;

static void codecWriteReg(uint8_t reg, uint8_t data){
	HAL_I2C_Mem_Write(&i2c,0x94,reg,I2C_MEMADD_SIZE_8BIT,&data,1,10000);
}

static uint8_t codecReadReg(uint8_t reg){
	uint8_t data;
	HAL_I2C_Mem_Read(&i2c,0x94,reg,I2C_MEMADD_SIZE_8BIT,&data,1,10000);
	return data;
}

void audioOutInit(uint32_t samplingRate, uint16_t *buffer, uint32_t length){
	GPIO_InitTypeDef portInit;
	RCC_PeriphCLKInitTypeDef rccInit;
	static DMA_HandleTypeDef dma;
	uint16_t pllN, pllR;
	/* GPIO: reset */
	__GPIOD_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_PULLDOWN;
	portInit.Speed=GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD,&portInit);
	/* GPIO: I2C */
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_6|GPIO_PIN_9;
	portInit.Mode=GPIO_MODE_AF_OD;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB,&portInit);
	/* GPIO: I2S */
	__GPIOA_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_4;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOA,&portInit);
	__GPIOC_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_12;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC,&portInit);
	/* reset */
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_Delay(5);
	/* I2C */
	__I2C1_CLK_ENABLE();
	i2c.Instance=I2C1;
	i2c.Init.ClockSpeed=100000;
	i2c.Init.OwnAddress1=0x33;
	i2c.Init.DutyCycle=I2C_DUTYCYCLE_2;
	i2c.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
	HAL_I2C_Init(&i2c);
	/* konfiguralas */
	if((codecReadReg(0x01)&0xF8)!=0xE0) while(1) ; //chip ID
	codecWriteReg(0x02,0x01); //power down
	codecWriteReg(0x04,0xAF); //HP be, SPKR ki
	codecWriteReg(0x05,0x81); //clock autodetect, MCLK/2
	codecWriteReg(0x06,0x07); //I2S, 16 bit
	codecWriteReg(0x0A,0x00); //soft ramp ki
	codecWriteReg(0x0E,0x04); //soft ramp ki
	codecWriteReg(0x27,0x00); //soft ramp ki
	codecWriteReg(0x1F,0x88); //treble, bass 0 dB
	codecWriteReg(0x02,0x9E); //power on
	/* orajel */
	switch(samplingRate){
		case I2S_AUDIOFREQ_8K:  pllN=256; pllR=5; break;
		case I2S_AUDIOFREQ_11K: pllN=429; pllR=4; break;
		case I2S_AUDIOFREQ_16K: pllN=213; pllR=4; break;
		case I2S_AUDIOFREQ_22K: pllN=429; pllR=4; break;
		case I2S_AUDIOFREQ_32K: pllN=426; pllR=4; break;
		case I2S_AUDIOFREQ_44K: pllN=271; pllR=6; break;
		case I2S_AUDIOFREQ_48K: pllN=258; pllR=3; break;
		case I2S_AUDIOFREQ_96K: pllN=344; pllR=2; break;
		default: while(1) ;
	}
	HAL_RCCEx_GetPeriphCLKConfig(&rccInit);
	rccInit.PeriphClockSelection=RCC_PERIPHCLK_I2S;
	rccInit.PLLI2S.PLLI2SN=pllN;
	rccInit.PLLI2S.PLLI2SR=pllR;
	HAL_RCCEx_PeriphCLKConfig(&rccInit);
	/* I2S */
	__SPI3_CLK_ENABLE();
	i2sOut.Instance=SPI3;
	i2sOut.Init.ClockSource=I2S_CLOCK_PLL;
	i2sOut.Init.AudioFreq=samplingRate;
	i2sOut.Init.Standard=I2S_STANDARD_PHILLIPS;
	i2sOut.Init.DataFormat=I2S_DATAFORMAT_16B;
	i2sOut.Init.CPOL=I2S_CPOL_LOW;
	i2sOut.Init.Mode=I2S_MODE_MASTER_TX;
	i2sOut.Init.MCLKOutput=I2S_MCLKOUTPUT_ENABLE;
	HAL_I2S_Init(&i2sOut);
	/* DMA */
	__DMA1_CLK_ENABLE();
	dma.Instance=DMA1_Stream7;
	dma.Init.Channel=DMA_CHANNEL_0;
	dma.Init.Direction=DMA_MEMORY_TO_PERIPH;
	dma.Init.PeriphInc=DMA_PINC_DISABLE;
	dma.Init.MemInc=DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dma.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dma.Init.Mode=DMA_CIRCULAR;
	dma.Init.Priority=DMA_PRIORITY_HIGH;
	dma.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
	dma.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dma.Init.MemBurst=DMA_MBURST_SINGLE;
	dma.Init.PeriphBurst=DMA_PBURST_SINGLE;
	HAL_DMA_Init(&dma);
	__HAL_LINKDMA(&i2sOut,hdmatx,dma);
	HAL_NVIC_SetPriority(DMA1_Stream7_IRQn,5,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
	/* elinditjuk az atvitelt */
	HAL_I2S_Transmit_DMA(&i2sOut,buffer,length);
}

void audioInInit(uint32_t samplingRate, uint16_t *buffer, uint32_t length){
	GPIO_InitTypeDef portInit;
	RCC_PeriphCLKInitTypeDef rccInit;
	static DMA_HandleTypeDef dma;
	uint16_t pllN, pllR;
	/* GPIO: SPI */
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_10;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB,&portInit);
	__GPIOC_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_3;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_FAST;
	portInit.Alternate=GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC,&portInit);
	/* orajel */
	switch(samplingRate){
		case I2S_AUDIOFREQ_16K: pllN=213; pllR=4; break;
		case I2S_AUDIOFREQ_22K: pllN=429; pllR=4; break;
		case I2S_AUDIOFREQ_32K: pllN=426; pllR=4; break;
		case I2S_AUDIOFREQ_44K: pllN=271; pllR=6; break;
		case I2S_AUDIOFREQ_48K: pllN=258; pllR=3; break;
		case I2S_AUDIOFREQ_96K: pllN=344; pllR=2; break;
		default: while(1) ;
	}
	HAL_RCCEx_GetPeriphCLKConfig(&rccInit);
	rccInit.PeriphClockSelection=RCC_PERIPHCLK_I2S;
	rccInit.PLLI2S.PLLI2SN=pllN;
	rccInit.PLLI2S.PLLI2SR=pllR;
	HAL_RCCEx_PeriphCLKConfig(&rccInit);
	/* I2S */
	__SPI2_CLK_ENABLE();
	i2sIn.Instance=SPI2;
	i2sIn.Init.ClockSource=I2S_CLOCK_PLL;
	i2sIn.Init.AudioFreq=samplingRate;
	i2sIn.Init.Standard=I2S_STANDARD_LSB;
	i2sIn.Init.DataFormat=I2S_DATAFORMAT_16B;
	i2sIn.Init.CPOL=I2S_CPOL_HIGH;
	i2sIn.Init.Mode=I2S_MODE_MASTER_RX;
	i2sIn.Init.MCLKOutput=I2S_MCLKOUTPUT_DISABLE;
	HAL_I2S_Init(&i2sIn);
	/* DMA */
	__DMA1_CLK_ENABLE();
	dma.Instance=DMA1_Stream3;
	dma.Init.Channel=DMA_CHANNEL_0;
	dma.Init.Direction=DMA_PERIPH_TO_MEMORY;
	dma.Init.PeriphInc=DMA_PINC_DISABLE;
	dma.Init.MemInc=DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;
	dma.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;
	dma.Init.Mode=DMA_CIRCULAR;
	dma.Init.Priority=DMA_PRIORITY_HIGH;
	dma.Init.FIFOMode=DMA_FIFOMODE_DISABLE;
	dma.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
	dma.Init.MemBurst=DMA_MBURST_SINGLE;
	dma.Init.PeriphBurst=DMA_PBURST_SINGLE;
	HAL_DMA_Init(&dma);
	__HAL_LINKDMA(&i2sIn,hdmarx,dma);
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn,6,0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	/* elinditjuk az atvitelt */
	HAL_I2S_Receive_DMA(&i2sIn,buffer,length);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){outTransferComplete=1;}
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){outTransferHalfComplete=1;}
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s){inTransferComplete=1;}
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s){inTransferHalfComplete=1;}

void DMA1_Stream7_IRQHandler(void){HAL_DMA_IRQHandler(i2sOut.hdmatx);}
void DMA1_Stream3_IRQHandler(void){HAL_DMA_IRQHandler(i2sIn.hdmarx);}
