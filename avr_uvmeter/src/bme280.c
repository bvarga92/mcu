#include "bme280.h"
#include "i2c.h"
#define F_CPU 8000000UL
#include <util/delay.h>

static uint8_t *buffer, dig_h1, dig_h3;
static int8_t dig_h6;
static uint16_t dig_t1, dig_p1;
static int16_t dig_t2, dig_t3, dig_p2, dig_p3, dig_p4, dig_p5, dig_p6, dig_p7, dig_p8, dig_p9, dig_h2, dig_h4, dig_h5;

void bme280Init(uint8_t *i2cBuffer){
	buffer=i2cBuffer;
	/* chip ID ellenorzese */
	buffer[0]=0xEC;
	buffer[1]=0xD0;
	i2cTransfer(buffer,2,0);
	buffer[0]=0xED;
	i2cTransfer(buffer,2,0);
	if(buffer[1]!=0x60) while(1) ;
	/* soft reset */
	buffer[0]=0xEC;
	buffer[1]=0xE0;
	buffer[2]=0xB6;
	i2cTransfer(buffer,3,0);
	_delay_ms(300);
	/* kalibracios adatok kiolvasasa */
	buffer[0]=0xEC;
	buffer[1]=0x88;
	i2cTransfer(buffer,2,0);
	buffer[0]=0xED;
	i2cTransfer(buffer,25,0);
	dig_t1=(buffer[2]<<8)|buffer[1];
	dig_t2=(buffer[4]<<8)|buffer[3];
	dig_t3=(buffer[6]<<8)|buffer[5];
	dig_p1=(buffer[8]<<8)|buffer[7];
	dig_p2=(buffer[10]<<8)|buffer[9];
	dig_p3=(buffer[12]<<8)|buffer[11];
	dig_p4=(buffer[14]<<8)|buffer[13];
	dig_p5=(buffer[16]<<8)|buffer[15];
	dig_p6=(buffer[18]<<8)|buffer[17];
	dig_p7=(buffer[20]<<8)|buffer[19];
	dig_p8=(buffer[22]<<8)|buffer[21];
	dig_p9=(buffer[24]<<8)|buffer[23];
	buffer[0]=0xEC;
	buffer[1]=0xA1;
	i2cTransfer(buffer,2,0);
	buffer[0]=0xED;
	i2cTransfer(buffer,2,0);
	dig_h1=buffer[1];
	buffer[0]=0xEC;
	buffer[1]=0xE1;
	i2cTransfer(buffer,2,0);
	buffer[0]=0xED;
	i2cTransfer(buffer,8,0);
	dig_h2=(buffer[2]<<8)|buffer[1];
	dig_h3=buffer[3];
	dig_h4=(buffer[4]<<4)|(buffer[5]&0x0F);
	dig_h5=(buffer[6]<<4)|(buffer[5]>>4);
	dig_h6=buffer[7];
	/* paratartalom oversampling 16 */
	buffer[0]=0xEC;
	buffer[1]=0xF2;
	buffer[2]=0x05;
	i2cTransfer(buffer,3,0);
	/* nyomas oversampling 16, homerseklet oversampling 16, normal mod */
	buffer[0]=0xEC;
	buffer[1]=0xF4;
	buffer[2]=0xB7;
	i2cTransfer(buffer,3,0);
}

void bme280Read(bme280data_t *bme280data){
	int32_t temp1, temp2, temp3, t_fine;
	int64_t var1, var2, p;
	/* kiolvasas */
	buffer[0]=0xEC;
	buffer[1]=0xF7;
	i2cTransfer(buffer,2,0);
	buffer[0]=0xED;
	i2cTransfer(buffer,9,0);
	/* homerseklet */
	temp1=((((int32_t)(buffer[4]))<<16)|(((int32_t)(buffer[5]))<<8)|buffer[6])>>4;
	temp2=((((temp1>>3)-((int32_t)dig_t1<<1)))*((int32_t)dig_t2))>>11;
	temp3=(((((temp1>>4)-((int32_t)dig_t1))*((temp1>>4)-((int32_t)dig_t1)))>>12)*((int32_t)dig_t3))>>14;
	t_fine=temp2+temp3;
	bme280data->temperature_x100=(t_fine*5+128)>>8; //a homerseklet szazszorosa (celsiusban)
	/* paratartalom */
	temp1=(((uint32_t)(buffer[7]))<<8)|buffer[8];
	temp2=(t_fine-((int32_t)76800));
	temp2=(((((temp1<<14)-(((int32_t)dig_h4)<<20)-(((int32_t)dig_h5)*temp2))+((int32_t)16384))>>15)*(((((((temp2*((int32_t)dig_h6))>>10)*(((temp2*((int32_t)dig_h3))>>11)+((int32_t)32768)))>>10)+((int32_t)2097152))*((int32_t)dig_h2)+8192)>>14));
	temp2=(temp2-(((((temp2>>15)*(temp2>>15))>>7)*((int32_t)dig_h1))>>4));
	temp2=(temp2<0)?0:temp2;
	temp2=(temp2>419430400)?419430400:temp2;
	temp2=(temp2>>12); //22.10 formatum
	bme280data->humidity=temp2>>10; //egeszre kerekitve (szazalekban)
	/* nyomas */
	temp1=((((int32_t)(buffer[1]))<<16)|(((int32_t)(buffer[2]))<<8)|buffer[3])>>4;
	var1=((int64_t)t_fine)-128000;
	var2=var1*var1*(int64_t)dig_p6;
	var2=var2+((var1*(int64_t)dig_p5)<<17);
	var2=var2+(((int64_t)dig_p4)<<35);
	var1=((var1*var1*(int64_t)dig_p3)>>8)+((var1*(int64_t)dig_p2)<<12);
	var1=(((((int64_t)1)<<47)+var1))*((int64_t)dig_p1)>>33;
	p=1048576-temp1;
	p=(((p<<31)-var2)*3125)/var1;
	var1=(((int64_t)dig_p9)*(p>>13)*(p>>13))>>25;
	var2=(((int64_t)dig_p8)*p)>>19;
	p=((p+var1+var2)>>8)+(((int64_t)dig_p7)<<4); //24.8 formatum
	bme280data->pressure=p>>8; //egeszre kerekitve (pascalban)
}
