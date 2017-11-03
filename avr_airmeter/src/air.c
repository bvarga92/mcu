/***************************************************************************/
/*                                                                         */
/*  Levegominoseg-mero Sharp GP2Y1010AU szenzorral      Varga Balazs 2017  */
/*                                                                         */
/*  Fuse bitek: 0xFFD9F7                                                   */
/*     - SPIEN bekapcs                                                     */
/*     - CKDIV8 kikapcs                                                    */
/*     - SUT_CKSEL: external full-swing, startup 16kCK/14CK+64ms           */
/*                                                                         */
/***************************************************************************/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "sensor.h"
#include "lcd.h"

int main(void){
	float ugpm3;
	sensorInit();
	lcdInit();
	lcdCursorGoto(1,3);
	lcdPutString("ug/m3");
	while(1){
		ugpm3=sensorRead();
		lcdCursorGoto(0,0);
		lcdPutString("        ");
		lcdCursorGoto(0,0);
		lcdPutInt((int16_t)ugpm3);
		lcdPutString(".");
		lcdPutInt(((int16_t)(ugpm3*10))%10);
		_delay_ms(1000);
	}
	return 0;
}
