#include "lcd.h"
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>


static const uint8_t aa[]   PROGMEM ={0b00000010,0b00000100,0b00001110,0b00000001,0b00001111,0b00010001,0b00001111,0b00000000};
static const uint8_t ee[]   PROGMEM ={0b00000010,0b00000100,0b00001110,0b00010001,0b00011111,0b00010000,0b00001110,0b00000000};
static const uint8_t ii[]   PROGMEM ={0b00000010,0b00000100,0b00000000,0b00001100,0b00000100,0b00000100,0b00001110,0b00000000};
static const uint8_t oo[]   PROGMEM ={0b00000010,0b00000100,0b00001110,0b00010001,0b00010001,0b00010001,0b00001110,0b00000000};
static const uint8_t oe[]   PROGMEM ={0b00001010,0b00000000,0b00001110,0b00010001,0b00010001,0b00010001,0b00001110,0b00000000};
static const uint8_t ooee[] PROGMEM ={0b00000101,0b00001010,0b00001110,0b00010001,0b00010001,0b00010001,0b00001110,0b00000000};
static const uint8_t uu[]   PROGMEM ={0b00000010,0b00000100,0b00010001,0b00010001,0b00010001,0b00010011,0b00001101,0b00000000};
static const uint8_t ue[]   PROGMEM ={0b00001010,0b00000000,0b00010001,0b00010001,0b00010001,0b00010011,0b00001101,0b00000000};
static const uint8_t uuee[] PROGMEM ={0b00000101,0b00001010,0b00010001,0b00010001,0b00010001,0b00010011,0b00001101,0b00000000};


static void lcdWriteNibble(uint8_t data){
	if(data&0x10) LCD_D4_PORT|=1<<LCD_D4_PIN; else LCD_D4_PORT&=~(1<<LCD_D4_PIN);
	if(data&0x20) LCD_D5_PORT|=1<<LCD_D5_PIN; else LCD_D5_PORT&=~(1<<LCD_D5_PIN);
	if(data&0x40) LCD_D6_PORT|=1<<LCD_D6_PIN; else LCD_D6_PORT&=~(1<<LCD_D6_PIN);
	if(data&0x80) LCD_D7_PORT|=1<<LCD_D7_PIN; else LCD_D7_PORT&=~(1<<LCD_D7_PIN);
	LCD_E_PORT|=1<<LCD_E_PIN;
	_delay_us(1);
	LCD_E_PORT&=~(1<<LCD_E_PIN);
	_delay_us(1);
}

static void lcdWriteByte(uint8_t data, uint8_t instruction){
	if(instruction) LCD_RS_PORT&=~(1<<LCD_RS_PIN); else LCD_RS_PORT|=1<<LCD_RS_PIN;
	lcdWriteNibble(data);
	_delay_us(100);
	lcdWriteNibble(data<<4);
	_delay_us(100);
}

static void lcdCreateChar(const uint8_t* ch, uint8_t n){
	uint8_t i;
	if(n>7) return;
	lcdWriteByte(0x40|(n<<3),1);
	_delay_ms(5);
	for(i=0;i<8;i++) {lcdWriteByte(pgm_read_byte_near(ch+i),0); _delay_ms(5);}
}


void lcdInit(void){
	LCD_RS_DDR|=1<<LCD_RS_PIN;
	LCD_E_DDR|=1<<LCD_E_PIN;
	LCD_D4_DDR|=1<<LCD_D4_PIN;
	LCD_D5_DDR|=1<<LCD_D5_PIN;
	LCD_D6_DDR|=1<<LCD_D6_PIN;
	LCD_D7_DDR|=1<<LCD_D7_PIN;
	LCD_E_PORT&=~(1<<LCD_E_PIN);
	LCD_RS_PORT&=~(1<<LCD_RS_PIN);
	_delay_ms(50);
	lcdWriteNibble(0x30); //reset1
	_delay_ms(10);
	lcdWriteNibble(0x30); //reset2
	_delay_ms(5);
	lcdWriteNibble(0x30); //reset3
	_delay_ms(5);
	lcdWriteNibble(0x20); //4 bites mod
	_delay_ms(5);
	lcdWriteByte(0x28,1); //2 sor, 5x7-es karakterek
	_delay_ms(5);
	lcdWriteByte(0x0C,1); //kijelzo be, kurzor ki
	_delay_ms(5);
	lcdWriteByte(0x06,1); //auto kurzor
	_delay_ms(5);
	lcdWriteByte(0x01,1); //torles
	_delay_ms(5);
	lcdCreateChar(aa,1);
	_delay_ms(5);
	lcdCreateChar(ee,2);
	_delay_ms(5);
	lcdCreateChar(ii,3);
	_delay_ms(5);
	lcdCreateChar(oo,4);
	_delay_ms(5);
	lcdCreateChar(oe,5);
	_delay_ms(5);
	lcdCreateChar(ooee,6);
	_delay_ms(5);
	lcdCreateChar(uu,7);
	_delay_ms(5);
	lcdWriteByte(0x80,1); //kurzor az elejere
	_delay_ms(5);
}

void lcdCursorGoto(uint8_t line, uint8_t pos){
	if((line>1)||(pos>15)) return;
	lcdWriteByte(0x80|(line<<6)|pos,1);
}

void lcdPutString(char* str){
	while(*str) lcdWriteByte((uint8_t)(*(str++)),0);
}

void lcdPutInt(int32_t n){
	uint16_t div;
	if(n==0){
		lcdWriteByte(0x30,0);
		return;
	}
	if(n<0){
		n=-n;
		lcdPutString("-");
	}
	for(div=1;div<=n;div*=10) ;
	do{
		div/=10;
		lcdWriteByte(n/div+0x30,0);
		n%=div;
	}while(div!=1);
}
