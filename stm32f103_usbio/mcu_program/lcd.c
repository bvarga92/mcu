#include "lcd.h"

static const uint8_t aa[8]=  {0x02,0x04,0x0E,0x01,0x0F,0x11,0x0F,0x00};
static const uint8_t ee[8]=  {0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00};
static const uint8_t ii[8]=  {0x02,0x04,0x00,0x0C,0x04,0x04,0x0E,0x00};
static const uint8_t oo[8]=  {0x02,0x04,0x0E,0x11,0x11,0x11,0x0E,0x00};
static const uint8_t oe[8]=  {0x0A,0x00,0x0E,0x11,0x11,0x11,0x0E,0x00};
static const uint8_t ooee[8]={0x05,0x0A,0x0E,0x11,0x11,0x11,0x0E,0x00};
static const uint8_t uu[8]=  {0x02,0x04,0x11,0x11,0x11,0x13,0x0D,0x00};
static const uint8_t ue[8]=  {0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00};
static const uint8_t uuee[8]={0x05,0x0A,0x11,0x11,0x11,0x13,0x0D,0x00};

static void lcdWriteUpperNibble(uint8_t data){
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D4,(data&0x10)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D5,(data&0x20)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D6,(data&0x40)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D7,(data&0x80)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_E,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_E,GPIO_PIN_RESET);
	HAL_Delay(1);
}

static void lcdWriteByte(uint8_t data, uint8_t isInstruction){
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_RS,isInstruction?GPIO_PIN_RESET:GPIO_PIN_SET);
	lcdWriteUpperNibble(data);
	lcdWriteUpperNibble(data<<4);
}

static void lcdCreateChar(const uint8_t *ch, uint8_t n){
	uint8_t i;
	if(n>7) return;
	lcdWriteByte(0x40|(n<<3),1);
	HAL_Delay(5);
	for(i=0;i<8;i++){
		lcdWriteByte(ch[i],0);
		HAL_Delay(5);
	}
}

void lcdInit(void){
	GPIO_InitTypeDef portInit;
	/* GPIOB */
	__GPIOB_CLK_ENABLE();
	portInit.Pin=LCD_PIN_D4|LCD_PIN_D5|LCD_PIN_D6|LCD_PIN_D7|LCD_PIN_E|LCD_PIN_RS;
	portInit.Mode=GPIO_MODE_OUTPUT_PP;
	portInit.Pull=GPIO_NOPULL;
	portInit.Speed=GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&portInit);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_D7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_E,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LCD_PIN_RS,GPIO_PIN_RESET);
	HAL_Delay(50);
	/* HD44780 */
	lcdWriteUpperNibble(0x30); //reset1
	HAL_Delay(10);
	lcdWriteUpperNibble(0x30); //reset2
	HAL_Delay(5);
	lcdWriteUpperNibble(0x30); //reset3
	HAL_Delay(5);
	lcdWriteUpperNibble(0x20); //4 bites mod
	HAL_Delay(5);
	lcdWriteByte(0x28,1); //2 sor, 5x7-es karakterek
	HAL_Delay(5);
	lcdWriteByte(0x0C,1); //kijelzo be, kurzor ki
	HAL_Delay(5);
	lcdWriteByte(0x06,1); //auto kurzor
	HAL_Delay(5);
	lcdWriteByte(0x01,1); //torles
	HAL_Delay(5);
	lcdCreateChar(aa,1);
	HAL_Delay(5);
	lcdCreateChar(ee,2);
	HAL_Delay(5);
	lcdCreateChar(ii,3);
	HAL_Delay(5);
	lcdCreateChar(oo,4);
	HAL_Delay(5);
	lcdCreateChar(oe,5);
	HAL_Delay(5);
	lcdCreateChar(ooee,6);
	HAL_Delay(5);
	lcdCreateChar(uu,7);
	HAL_Delay(5);
	lcdWriteByte(0x80,1); //kurzor az elejere
	HAL_Delay(5);
}

void lcdCursorGoto(uint8_t line, uint8_t pos){
	if((line>1)||(pos>15)) return;
	lcdWriteByte(0x80|(line<<6)|pos,1);
}

void lcdPutString(char* str, uint8_t len){
	while(len--) lcdWriteByte((uint8_t)(*(str++)),0);
}
