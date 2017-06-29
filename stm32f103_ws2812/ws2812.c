#include "ws2812.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_dma.h"

static TIM_HandleTypeDef timer1;
static DMA_HandleTypeDef dma;
static uint8_t buffer[BUFFER_LEN]; //minden bitnek egy bajt (0=reset, 30="0", 60="1")

void DMA1_Channel3_IRQHandler(void){
	HAL_DMA_IRQHandler(&dma);
}

void ws2812Init(void){
	GPIO_InitTypeDef portInit;
	TIM_OC_InitTypeDef sConfigOC;
	uint32_t i;
	/* GPIO */
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	portInit.Pin=GPIO_PIN_9;
	portInit.Mode=GPIO_MODE_AF_PP;
	portInit.Speed=GPIO_SPEED_FREQ_HIGH;
	portInit.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&portInit);
	/* timer */
	__HAL_RCC_TIM1_CLK_ENABLE();
	timer1.Instance=TIM1;
	timer1.Init.Prescaler=0; //72 MHz alaporajel
	timer1.Init.CounterMode=TIM_COUNTERMODE_UP;
	timer1.Init.Period=89; //800 kHz, 0 = 0% kitoltes, 90 = 100% kitoltes
	timer1.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	timer1.Init.RepetitionCounter=0;
	HAL_TIM_PWM_Init(&timer1);
	sConfigOC.OCMode=TIM_OCMODE_PWM1;
	sConfigOC.OCPolarity=TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode=TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&timer1,&sConfigOC,TIM_CHANNEL_2);
	/* DMA */
	__HAL_RCC_DMA1_CLK_ENABLE();
	dma.Instance=DMA1_Channel3;
	dma.Init.Direction=DMA_MEMORY_TO_PERIPH;
	dma.Init.PeriphInc=DMA_PINC_DISABLE;
	dma.Init.MemInc=DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;
	dma.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
	dma.Init.Mode=DMA_CIRCULAR;
	dma.Init.Priority=DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&dma);
	__HAL_LINKDMA(&timer1,hdma[TIM_DMA_ID_CC2],dma);
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	/* inicializaljuk a buffert, majd elinditjuk az atvitelt */
	for(i=0;i<(BUFFER_LEN-RESET_LEN);i++) buffer[i]=30;
	for(;i<BUFFER_LEN;i++) buffer[i]=0;
	HAL_TIM_PWM_Start_DMA(&timer1,TIM_CHANNEL_2,(uint32_t*)buffer,BUFFER_LEN);
}

void ws2812SetLED(uint8_t ledNum, rgb_t *color){
	uint8_t i;
	if(ledNum>=NUM_LEDS) return;
	for(i=0;i<8;i++){
		buffer[ledNum*24+i]=(color->g)&(0x80>>i)?60:30;
		buffer[ledNum*24+i+8]=(color->r)&(0x80>>i)?60:30;
		buffer[ledNum*24+i+16]=(color->b)&(0x80>>i)?60:30;
	}
}

void hsv2rgb(uint16_t h, uint8_t s, uint8_t v, rgb_t *color){
	uint8_t r, g, b;
	if((h>359)||(s>100)||(v>100)) return;
	if(h<61){
		r=255;
		g=(425*h)/100;
		b=0;
	}else if(h<121){
		r=255-(425*(h-60))/100;
		g=255;
		b=0;
	}else if(h<181){
		r=0;
		g=255;
		b=(425*(h-120))/100;
	}else if(h<241){
		r=0;
		g=255-(425*(h-180))/100;
		b=255;
	}else if(h<301){
		r=(425*(h-240))/100;
		g=0;
		b=255;
	}else{
		r=255;
		g=0;
		b=255-((425*(h-300))/100);
	}
	s=100-s;
	color->r=((r+((255-r)*s)/100)*v)/100;
	color->g=((g+((255-g)*s)/100)*v)/100;
	color->b=((b+((255-b)*s)/100)*v)/100;
}
