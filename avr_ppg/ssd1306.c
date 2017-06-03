/***************************************************************************/
/*                                                                         */
/*  Driver az SSD1306 kijelzohoz (128x64)               Varga Balazs 2015  */
/*                                                                         */
/*  Minden fuggveny a frame bufferbe dolgozik, a kijelzo frissitesehez az  */
/*  updateDisplay() fuggvenyt kell meghivni!                               */
/*                                                                         */
/***************************************************************************/

#include "ssd1306.h"
#include "font.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#define F_CPU 16000000UL
#include <util/delay.h>


/* a kepernyo tartalma sorfolytonosan (1: vilagos pixel, 0: sotet pixel) */
static uint8_t frameBuffer[1024];

/* kurzor szoveg irasahoz */
static uint8_t cursorX, cursorY;

/* inicializalo parancsok */
static const uint8_t initCommands[25] PROGMEM={0xAE,0xD5,0x80,0xA8,0x3F,
                                               0xD3,0x00,0x40,0x8D,0x14,
                                               0x20,0x00,0xA1,0xC8,0xDA,
                                               0x12,0x81,0xCF,0xD9,0xF1,
                                               0xDB,0x40,0xA4,0xA6,0xAF};

											   
/* az atadott bajtot elkuldi a kijelzonek (ha a masodik parameter true, akkor parancs, kulonben adat) */
static void displaySend(uint8_t data, bool command){
	if(command)
		PORTB&=~0x02;
	else
		PORTB|=0x02;
	SPDR=data;
	while(!(SPSR&0x80)) ;
}

/* kijelzo inicializalasa */
void displayInit(void){
	uint8_t i;
	/* SPI: master, 4 Mbps, MSB first, mode 3 */
	DDRB|=0x2E;
	SPCR=0x5C;
	/* reset */
	PORTB|=0x04;
	_delay_ms(2);
	PORTB&=~0x04;
	_delay_ms(2);
	PORTB|=0x04;
	_delay_ms(2);
	/* init parancsok kikuldese */
	for(i=0;i<25;i++) displaySend(pgm_read_byte(&initCommands[i]),true);
}

/* megjeleniti a frame buffer tartalmat */
void displayUpdate(void){
	uint16_t i;
	displaySend(0x00,true);
	displaySend(0x10,true);
	displaySend(0x40,true);
	for(i=0;i<1024;i++) displaySend(frameBuffer[i],false);
}

/* torli a frame buffer tartalmat */
void displayClear(void){
	memset(frameBuffer,0,1024);
}

/* bekapcsolja a megadott koordinataju pixelt */
void displaySetPixel(uint8_t x, uint8_t y){
	if((x>127)||(y>63)) return;
	frameBuffer[x+(y/8)*128]|=(1<<(y&7));
}

/* kikapcsolja a megadott koordinataju pixelt */
void displayClearPixel(uint8_t x, uint8_t y){
	if((x>127)||(y>63)) return;
	frameBuffer[x+(y/8)*128]&=~(1<<(y&7));
}

/* invertalja a kepet */
void displayInvert(void){
	static uint8_t negative=0;
	if(negative){
		displaySend(0xA6,true);
		negative=0;
	}
	else{
		displaySend(0xA7,true);
		negative=1;
	}
}

/* beallitja a szovegkurzort megadott pozicioba */
void displaySetCursor(uint8_t x, uint8_t y){
	cursorX=(x>123)?123:x;
	cursorY=(y>56)?56:y;
}

/* kirja az atadott karaktert a kurzor helyere */
void displayPrintChar(char ch){
	uint16_t byte;
	uint8_t i;
	byte=(cursorY/8==0)?(cursorX):(cursorX+128*(cursorY/8));
	for(i=0;i<5;i++) frameBuffer[byte+i]=pgm_read_byte(&font[(ch-0x20)*5+i]);
	cursorX+=6;
	if((cursorX>123)&&(cursorY>=56)){
		cursorX=128;
		cursorY=56;
	}
	else{
		if(cursorX>123){
			cursorX=0;
			cursorY+=8;
		}
		if(cursorY>56)
			cursorY=56;
	}
}

/* kirja az atadott nullterminalt sztringet a kurzor helyere */
void displayPrintText(char* str){
	uint8_t i;
	for(i=0;str[i];i++) displayPrintChar(str[i]);
}

/* kirja az atadott egesz szamot a kurzor helyere */
void displayPrintInteger(int n){
	int div;
	if(n==0){
		displayPrintChar('0');
		return;
	}
	if(n<0){
		n=-n;
		displayPrintChar('-');
	}
	for(div=1;div<=n;div*=10) ;
	do{
		div/=10;
		displayPrintChar(0x30+n/div);
		n%=div;
	}while(div>1);
}
