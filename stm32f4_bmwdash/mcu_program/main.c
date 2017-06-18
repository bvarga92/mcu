/******************************************************************************/
/*                                                                            */
/*  A muszeregyseg a virtualis COM porton elkuldott uzenetekkel vezerelheto.  */
/*                                                                            */
/*   - uzenetformatum: $rrrr sss l r h|                                       */
/*   - minden csomag $ karakterrel kezdodik                                   */
/*   - rrrr: fordulatszam (pl. 700 RPM --> 0700)                              */
/*   - sss:  sebesseg (pl. 40 km/h --> 040)                                   */
/*   - l:    bal index (0: nincs valtozas, 1: bekapcs, 2: kikapcs)            */
/*   - r:    jobb index (0: nincs valtozas, 1: bekapcs, 2: kikapcs)           */
/*   - h:    tavolsagi fenyszoro (0: nincs valtozas, 1: bekapcs, 2: kikapcs)  */
/*   - minden csomag | karakterrel vegzodik, ez juttatja ervenyre             */
/*                                                                            */
/******************************************************************************/

#include "stm32f4xx_hal.h"
#include "system.h"
#include "vcp.h"
#include "can.h"
#include "speed.h"
#include "kbus.h"

static uint16_t rpm;
static uint8_t speed;
static uint8_t signalLeft;
static uint8_t signalRight;
static uint8_t highBeam;
static uint8_t updateFlag=0;

void vcpReceived(uint8_t data){
	static char buffer[256];
	static uint8_t cntr=0;
	ledToggle(LED_BLUE);
	if(data=='$') //start delimiter
		cntr=0;
	else if(cntr==0) //ha cntr==0, de nem $ jott, akkor ez hiba
		return;
	buffer[cntr++]=data;
	if(data=='|'){ //end delimiter
		if(cntr==16){ //az ervenyes csomag 16 karakter hosszu
			rpm=(buffer[1]-0x30)*1000+(buffer[2]-0x30)*100+(buffer[3]-0x30)*10+(buffer[4]-0x30);
			speed=(buffer[6]-0x30)*100+(buffer[7]-0x30)*10+(buffer[8]-0x30);
			signalLeft=buffer[10]-0x30;
			signalRight=buffer[12]-0x30;
			highBeam=buffer[14]-0x30;
			updateFlag=1;
		}
	}
}

int main(void){
	uint8_t prevSpeed=0;
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	vcpInit();
	canInit();
	speedInit();
	kbusInit();
	kbusSetDisplayBacklight(0x55);
	speedSet(0);
	canSetCoolantTemp(90);
	canLightCheckEngineOff();
	canLightEMLOff();
	canLightOverheatOff();
	canLightCruiseControlOff();
	vcpTransmit((uint8_t*)"Ready.\r\n",8);
	while(1){
		if(updateFlag){ //ervenyes adatot vettunk
			updateFlag=0;
			ledToggle(LED_ORANGE);
			vcpTransmit((uint8_t*)"Data received.\r\n",16);
			canSetRPM(rpm);
			if(speed!=prevSpeed){
				speedSet(speed);
				prevSpeed=speed;
			}
			if(signalLeft==1)
				kbusLeftSignalOn();
			else if(signalLeft==2)
				kbusLeftSignalOff();
			if(signalRight==1)
				kbusRightSignalOn();
			else if(signalRight==2)
				kbusRightSignalOff();
			if(highBeam==1)
				kbusHighBeamOn();
			else if(highBeam==2)
				kbusHighBeamOff();
		}
	}
	return 0;
}
