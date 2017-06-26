#include "adc.h"
#include "stm32f1xx_hal_adc.h"

static ADC_HandleTypeDef adcHandle;

void adcInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	portInit.Mode=GPIO_MODE_ANALOG;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&portInit);
	portInit.Pin=GPIO_PIN_0|GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB,&portInit);
	__ADC1_CLK_ENABLE();
	adcHandle.Instance=ADC1;
	adcHandle.Init.ContinuousConvMode=DISABLE;
	adcHandle.Init.DataAlign=ADC_DATAALIGN_RIGHT;
	adcHandle.Init.DiscontinuousConvMode=DISABLE;
	adcHandle.Init.ExternalTrigConv=ADC_SOFTWARE_START;
	adcHandle.Init.NbrOfConversion=1;
	adcHandle.Init.NbrOfDiscConversion=0;
	adcHandle.Init.ScanConvMode=ADC_SCAN_DISABLE;
	HAL_ADC_Init(&adcHandle);
}

uint8_t adcRead(uint8_t channel){
	ADC_ChannelConfTypeDef adcConfig;
	if(channel>9) return 0;
	adcConfig.Channel=channel;
	adcConfig.Rank=1;
	adcConfig.SamplingTime=ADC_SAMPLETIME_28CYCLES_5;
	HAL_ADC_ConfigChannel(&adcHandle,&adcConfig);
	HAL_ADC_Start(&adcHandle);
	HAL_ADC_PollForConversion(&adcHandle,10);
	return HAL_ADC_GetValue(&adcHandle)>>4;
}
