#include "veml6075.h"
#include "i2c.h"

static uint8_t *buffer;

void veml6075Init(uint8_t *i2cBuffer){
	buffer=i2cBuffer;
	/* chip ID ellenorzese */
	buffer[0]=0x20;
	buffer[1]=0x0C;
	buffer[2]=0x21;
	i2cTransfer(buffer,5,MEMREAD);
	if(buffer[3]!=0x26 || buffer[4]!=0x00) while(1) ;
	/* folytonos mod, 100 ms */
	buffer[0]=0x20;
	buffer[1]=0x00;
	buffer[2]=0x10;
	buffer[3]=0x00;
	i2cTransfer(buffer,4,0);
}

void veml6075Read(uvdata_t *uvdata){
	uint16_t uva_raw, uvb_raw, uvcomp1, uvcomp2;
	buffer[0]=0x20;
	buffer[1]=0x07;
	buffer[2]=0x21;
	i2cTransfer(buffer,5,MEMREAD);
	uva_raw=(buffer[4]<<8)|buffer[3];
	buffer[1]=0x09;
	i2cTransfer(buffer,5,MEMREAD);
	uvb_raw=(buffer[4]<<8)|buffer[3];
	buffer[1]=0x0A;
	i2cTransfer(buffer,5,MEMREAD);
	uvcomp1=(buffer[4]<<8)|buffer[3];
	buffer[1]=0x0B;
	i2cTransfer(buffer,5,MEMREAD);
	uvcomp2=(buffer[4]<<8)|buffer[3];
	uvdata->uvia_x10M=(100*(uint32_t)uva_raw-(222*(uint32_t)uvcomp1)-(133*(uint32_t)uvcomp2))*110;
	uvdata->uvib_x10M=(100*(uint32_t)uvb_raw-(295*(uint32_t)uvcomp1)-(174*(uint32_t)uvcomp2))*125;
}
