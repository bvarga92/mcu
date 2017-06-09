#include "system.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

void configSystemClock(void){
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* PLL beallitasa, forras a HSE */
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM=8;
	RCC_OscInitStruct.PLL.PLLN=336;
	RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ=7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* buszorajelek a PLL-rol */
	RCC_ClkInitStruct.ClockType=RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5);
	/* systick */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void ledInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOD_CLK_ENABLE();
	portInit.Pin=LED_ALL;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOD,&portInit);
}

void ledOn(uint16_t ledPos){
	if(!(ledPos&LED_ALL)) assert_param(ledPos);
	ledPos&=LED_ALL;
	HAL_GPIO_WritePin(GPIOD,ledPos,GPIO_PIN_SET);
}

void ledOff(uint16_t ledPos){
	if(!(ledPos&LED_ALL)) assert_param(ledPos);
	ledPos&=LED_ALL;
	HAL_GPIO_WritePin(GPIOD,ledPos,GPIO_PIN_RESET);
}

void ledToggle(uint16_t ledPos){
	if(!(ledPos&LED_ALL)) assert_param(ledPos);
	ledPos&=LED_ALL;
	HAL_GPIO_TogglePin(GPIOD,ledPos);
}

void buttonInit(void){
	GPIO_InitTypeDef portInit;
	__GPIOA_CLK_ENABLE();
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	portInit.Pin=GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA,&portInit);
}

uint8_t buttonRead(void){
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))
		return 1;
	else
		return 0;
}
