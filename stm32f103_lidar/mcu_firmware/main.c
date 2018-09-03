#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "system.h"
#include "vcp.h"
#include "servo.h"
#include "lidar.h"

bool freerun=true, single=false;
uint16_t horz=3050, vert=2750;

void vcpReceived(uint8_t data){
	if(data=='u'){      //szervo fel
		vert+=50;
		servoSet(horz,vert);
	}
	else if(data=='d'){ //szervo le
		vert-=50;
		servoSet(horz,vert);
	}
	else if(data=='l'){ //szervo balra
		horz-=50;
		servoSet(horz,vert);
	}
	else if(data=='r'){ //szervo jobbra
		horz+=50;
		servoSet(horz,vert);
	}
	else if(data=='c'){ //szervo kozepre
		horz=3050;
		vert=2750;
		servoSet(horz,vert);
	}
	else if(data=='f'){ //folyamatos meres
		freerun=true;
	}
	else if(data=='s'){ //egyszeri meres
		freerun=false;
		single=true;
	}
}

void printSerial(uint16_t data){
	uint8_t buf[6];
	buf[0]=(data<10000)?' ':((data/10000)%10+0x30);
	buf[1]=(data<1000) ?' ':((data/1000) %10+0x30);
	buf[2]=(data<100)  ?' ':((data/100)  %10+0x30);
	buf[3]=(data<10)   ?' ':((data/10)   %10+0x30);
	buf[4]=data%10+0x30;
	buf[5]='$';
	vcpTransmit(buf,6);
}

uint16_t makeMeasurement(void){
	uint32_t sum;
	uint8_t i;
	sum=0;
	for(i=0;i<4;i++) sum+=lidarGetDistanceCM();
	return (uint16_t)(sum>>2);
}

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	vcpInit();
	servoInit(horz,vert);
	lidarInit();
	while(1){
		if(freerun|single){
			printSerial(makeMeasurement());
			if(single) single=false;
			ledToggle();
			HAL_Delay(10);
		}
	}
	return 1;
}
