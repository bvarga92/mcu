/*********************************************************************************/
/*                                                                               */
/* Parancsok:                                                                    */
/*    - r1:      rele be                                                         */
/*    - r0:      rele ki                                                         */
/*    - t1:      tranzisztor be                                                  */
/*    - t0:      tranzisztor ki                                                  */
/*    - mnssss:  az n-edik (1-3) motor tesz ssss lepest (0000-9999) CW iranyban  */
/*    - mn-ssss: az n-edik (1-3) motor tesz ssss lepest (0000-9999) CCW iranyban */
/*                                                                               */
/*********************************************************************************/

#include "stm32f1xx_hal.h"
#include "system.h"
#include "vcp.h"
#include "stepper.h"

typedef enum{IDLE, RELAY, TR, DRV, MOTx_0, MOTx_1, MOTx_2, MOTx_3, MOTx_4} state_t;

void vcpReceived(uint8_t data){
	static state_t state=IDLE;
	static uint8_t motor;
	static uint8_t dir;
	static int16_t steps;
	ledToggle();
	if(state==IDLE){
		switch(data){
			case 'r': state=RELAY; break;
			case 't': state=TR;    break;
			case 'd': state=DRV;  break;
			case 'm': state=MOTx_0;  break;
			default:  break;
		}
	}
	else if(state==RELAY){
		switch(data){
			case '0': stepperRelayOff(); state=IDLE; break;
			case '1': stepperRelayOn();  state=IDLE; break;
			default:  state=IDLE; break;
		}
	}
	else if(state==TR){
		switch(data){
			case '0': stepperTransistorOff(); state=IDLE; break;
			case '1': stepperTransistorOn();  state=IDLE; break;
			default:  state=IDLE; break;
		}
	}
	else if(state==DRV){
		switch(data){
			case '0': stepperSetSequence(0); state=IDLE; break;
			case '1': stepperSetSequence(1); state=IDLE; break;
			case '2': stepperSetSequence(2); state=IDLE; break;
			default:  state=IDLE; break;
		}
	}
	else if(state==MOTx_0){
		switch(data){
			case '1': motor=1; dir=0; state=MOTx_1; break;
			case '2': motor=2; dir=0; state=MOTx_1; break;
			case '3': motor=3; dir=0; state=MOTx_1; break;
			default:  state=IDLE; break;
		}
	}
	else if(state==MOTx_1){
		if(data<'0' || data>'9'){
			if(data=='-' && dir==0)
				dir=1;
			else
				state=IDLE;
		}
		else{
			steps=(data-0x30)*1000;
			state=MOTx_2;
		}
	}
	else if(state==MOTx_2){
		if(data<'0' || data>'9'){
			state=IDLE;
		}
		else{
			steps+=(data-0x30)*100;
			state=MOTx_3;
		}
	}
	else if(state==MOTx_3){
		if(data<'0' || data>'9'){
			state=IDLE;
		}
		else{
			steps+=(data-0x30)*10;
			state=MOTx_4;
		}
	}
	else if(state==MOTx_4){
		if(data<'0' || data>'9'){
			state=IDLE;
		}
		else{
			steps+=data-0x30;
			stepperStep(motor,dir?(-steps):steps);
			state=IDLE;
		}
	}
}

int main(void){
	HAL_Init();
	clockInit();
	ledInit();
	vcpInit();
	stepperInit();
	while(1){

	}
	return 1;
}
