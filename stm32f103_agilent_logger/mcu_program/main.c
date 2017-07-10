/************************************************************/
/*                                                          */
/* RS232-USB gateway az Agilent 34401A multimeterhez        */
/*                                                          */
/*     - a muszer legyen talk-only modban! (HP-IB cim: 31)  */
/*     - UART mod: 8N2 @ 4800 baud                          */
/*     - bekapcsolas: elobb legyen az USB csatlakoztatva a  */
/*       PC-hez, es csak azutan kapcsoljuk be a muszert     */
/*     - kikapcsolas: elobb a muszert kapcsoljuk ki, majd   */
/*       utana huzzuk ki az USB csatlakozot a PC-bol        */
/*                                                          */
/************************************************************/

#include "stm32f1xx_hal.h"
#include "system.h"
#include "uart.h"
#include "vcp.h"

void vcpReceived(uint8_t data){}

uint8_t validityCheck(uint8_t* buffer){
	uint8_t i;
	if((buffer[0]!='-') && (buffer[0]!='+')) return 0;
	if(buffer[2]!='.') return 0;
	if(buffer[11]!='E') return 0;
	if((buffer[12]!='-') && (buffer[12]!='+')) return 0;
	for(i=1;i<15;i++){
		if((i==2)||(i==11)||(i==12)) continue;
		if((buffer[i]<'0')||(buffer[i]>'9')) return 0;
	}
	return 1;
}

int main(void){
	uint8_t buffer[16], bufCntr=0, state=0, temp;
	HAL_Init();
	clockInit();
	ledInit();
	uartInit();
	vcpInit();
	buffer[15]='\n';
	while(1){
		if(!uartFIFOIsEmpty()){
			temp=uartFifoGet();
			if(temp==13){ //CR --> LF-nek kell jonnie
				state=1;
			}
			else if(temp==10){ //LF --> csomag vege
				if((state==1)&&(bufCntr==15)&&validityCheck(buffer)){ //SD.DDDDDDDDESDD formatum
					vcpTransmit(buffer,16);
					ledToggle();
				}
				state=0;
				bufCntr=0;
			}
			else if((state==0)&&(bufCntr<15)){ //minden mast mentunk a bufferbe
				buffer[bufCntr++]=temp;
			}
		}
	}
	return 1;
}
