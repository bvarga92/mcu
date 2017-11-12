#include "board.h"
#include "uart.h"
#include "lcd.h"
#include "timer.h"

int main(void){
	uint16_t msCntr=0, sCntr=0;
	boardInit();
	lcdInit(false);
	uartInit();
	timerInit(24000); //1ms-onkent timer overflow flag
	lcdAlphaPrint("HELLO");
	while(1){
		if(uartReceived){
			uartReceived=false;
			if(uartRxChar==0x30){ //0 karakter bekapcsolja LED0-t
				ledToggle(0);
				uartPrint(ledGet(0)?"LED0 bekapcsolva.\r":"LED0 kikapcsolva.\r");
			}
			else if(uartRxChar==0x31){ //1 karakter bekapcsolja LED1-et
				ledToggle(1);
				uartPrint(ledGet(1)?"LED1 bekapcsolva.\r":"LED1 kikapcsolva.\r");
			}
		}
		if(timerIntrp){
			timerIntrp=false;
			msCntr++;
			if(msCntr==1000){
				msCntr=0;
				sCntr=(sCntr==9999)?0:(sCntr+1);
				lcdUpperNumberPrint(sCntr);
			}
		}
	}
}
