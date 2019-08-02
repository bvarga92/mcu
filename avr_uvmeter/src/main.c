/************************************************************************/
/*                                                                      */
/*   Fuse bitek: low=0xE2, high=0xDF, extended=0xFF                     */
/*   Optimalizacio: -Os                                                 */
/*                                                                      */
/************************************************************************/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "i2c.h"
#include "veml6075.h"
#include "bme280.h"
#include "ssd1306.h"

int main(void){
	uint8_t i2cBuffer[25], x;
	uvdata_t uvdata;
	bme280data_t bme280data;
	i2cInit();
	_delay_ms(200);
	veml6075Init(i2cBuffer);
	bme280Init(i2cBuffer);
	displayInit();
	while(1){
		_delay_ms(1000);
		/* kiolvasas */
		veml6075Read(&uvdata);
		bme280Read(&bme280data);
		/* UVA */
		x=displayPrintText("UVA Index: ",0,2);
		x=displayPrintInteger(uvdata.uvia_x10M/10000000,0,x,2);
		x=displayPrintText(".",x,2);
		x=displayPrintInteger((uvdata.uvia_x10M%10000000)/10000,3,x,2);
		displayPrintText("  ",x,2);
		/* UVB */
		x=displayPrintText("UVB Index: ",0,3);
		x=displayPrintInteger(uvdata.uvib_x10M/10000000,0,x,3);
		x=displayPrintText(".",x,3);
		x=displayPrintInteger((uvdata.uvib_x10M%10000000)/10000,3,x,3);
		displayPrintText("  ",x,3);
		/* homerseklet */
		x=displayPrintInteger(bme280data.temperature_x100/100,0,0,0);
		x=displayPrintText(".",x,0);
		x=displayPrintInteger((bme280data.temperature_x100%100)/10,0,x,0);
		x=displayPrintText("\x7F""C ",x,0)+2;
		/* paratartalom */
		x=displayPrintInteger(bme280data.humidity,0,x,0);
		x=displayPrintText("% ",x,0)+2;
		/* nyomas */
		x=displayPrintInteger(bme280data.pressure/100,0,x,0)+3;
		displayPrintText("hPa  ",x,0);
	}
}
