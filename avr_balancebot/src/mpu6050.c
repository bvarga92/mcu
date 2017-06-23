#include "mpu6050.h"
#include <avr/io.h>
#include <math.h>

static uint8_t imuWrite(uint8_t reg, uint8_t data){
	/* START feltetel */
	TWCR=0xA4;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x08) return 0;
	/* slave megcimzese */
	TWDR=0xD0;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x18) return 0;
	/* regiszter kivalasztasa */
	TWDR=reg;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x28) return 0;
	/* adat elkuldese */
	TWDR=data;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x28) return 0;
	/* STOP feltetel */
	TWCR=0x94;
	while(TWCR&0x10) ;
	return 1;
}

static uint8_t imuRead(uint8_t start, uint8_t *buffer, uint8_t n){
	uint8_t i;
	/* START feltetel */
	TWCR=0xA4;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x08) return 0;
	/* slave megcimzese */
	TWDR=0xD0;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x18) return 0;
	/* kezdocim kivalasztasa */
	TWDR=start;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x28) return 0;
	/* RESTART feltetel */
	TWCR=0xA4;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x10) return 0;
	/* slave megcimzese */
	TWDR=0xD1;
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x40) return 0;
	/* adatok kiolvasasa (utolsonal nincs ACK!) */
	for(i=0;i<n-1;i++){
		TWCR=0xC4;
		while(!(TWCR&0x80)) ;
		if((TWSR&0xF8)!=0x50) return 0;
		buffer[i]=TWDR;
	}
	TWCR=0x84;
	while(!(TWCR&0x80)) ;
	if((TWSR&0xF8)!=0x58) return 0;
	buffer[n-1]=TWDR;
	/* STOP feltetel */
	TWCR=0x94;
	while(TWCR&0x10) ;
	return 1;
}

void imuInit(void){
	/* I2C busz: belso felhuzoellenallasok be, 100 kHz */
	PORTC|=0x30;
	TWSR=0xF8;
	TWBR=0x48;
	/* PWR_MGMT_1 regiszter 0 --> wake-up */
	imuWrite(0x6B,0x00);
}

void imuMeasure(float *accDeg, float *gyrDegPerSec){
	uint8_t buffer[14];
	int16_t accX, accY, accZ, gyrY;
	imuRead(0x3B,buffer,14);
	accX=(buffer[0]<<8)|buffer[1];
	accY=(buffer[2]<<8)|buffer[3];
	accZ=(buffer[4]<<8)|buffer[5];
	gyrY=(buffer[10]<<8)|buffer[11];
	*accDeg=atan2(-(float)accX,hypot((float)accY,(float)accZ))*180/3.141592654f;
	*gyrDegPerSec=gyrY/131.0f;
}
