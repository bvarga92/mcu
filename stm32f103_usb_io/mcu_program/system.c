#include "system.h"

void clockInit(void){
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;
	/* PLL beallitasa, forras a HSE, 72 MHz */
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=RCC_HSE_ON;
	RCC_OscInitStruct.HSIState=RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.HSEPredivValue=RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL9;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* buszorajelek a PLL-rol */
	RCC_ClkInitStruct.ClockType=RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_2);
	/* USB: 48 MHz */
	PeriphClkInit.PeriphClockSelection=RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection=RCC_USBCLKSOURCE_PLL_DIV1_5;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	/* ADC: 12 MHz */
	PeriphClkInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	/* systick */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
}


uint8_t jumperIsOn(void){
	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef portInit;
	portInit.Pin=GPIO_PIN_8;
	portInit.Mode=GPIO_MODE_INPUT;
	portInit.Pull=GPIO_PULLUP;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&portInit);
	return (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)==GPIO_PIN_RESET)?1:0;
}

void ledInit(void){
	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef portInit;
	portInit.Pin=GPIO_PIN_13;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC,&portInit);
	ledOff();
}

void ledOff(void){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}

void ledOn(void){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
}

void ledToggle(void){
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
