#include "stm32f1xx_hal.h"
#include "system.h"
#include "vcp.h"
#include "lsm6dsox.h"

void vcpReceived(uint8_t data){}

void printSerial(const lsm_data_t *data){
	uint8_t buf[36];
	buf[ 0]='|';
	buf[ 1]=((data->temp  & 0xF000)>>12)+'0'; if(buf[ 1]>'9') buf[ 1]+=7;
	buf[ 2]=((data->temp  & 0x0F00)>> 8)+'0'; if(buf[ 2]>'9') buf[ 2]+=7;
	buf[ 3]=((data->temp  & 0x00F0)>> 4)+'0'; if(buf[ 3]>'9') buf[ 3]+=7;
	buf[ 4]= (data->temp  & 0x000F)     +'0'; if(buf[ 4]>'9') buf[ 4]+=7;
	buf[ 5]=',';
	buf[ 6]=((data->acc_x & 0xF000)>>12)+'0'; if(buf[ 6]>'9') buf[ 6]+=7;
	buf[ 7]=((data->acc_x & 0x0F00)>> 8)+'0'; if(buf[ 7]>'9') buf[ 7]+=7;
	buf[ 8]=((data->acc_x & 0x00F0)>> 4)+'0'; if(buf[ 8]>'9') buf[ 8]+=7;
	buf[ 9]= (data->acc_x & 0x000F)     +'0'; if(buf[ 9]>'9') buf[ 9]+=7;
	buf[10]=',';
	buf[11]=((data->acc_y & 0xF000)>>12)+'0'; if(buf[11]>'9') buf[11]+=7;
	buf[12]=((data->acc_y & 0x0F00)>> 8)+'0'; if(buf[12]>'9') buf[12]+=7;
	buf[13]=((data->acc_y & 0x00F0)>> 4)+'0'; if(buf[13]>'9') buf[13]+=7;
	buf[14]= (data->acc_y & 0x000F)     +'0'; if(buf[14]>'9') buf[14]+=7;
	buf[15]=',';
	buf[16]=((data->acc_z & 0xF000)>>12)+'0'; if(buf[16]>'9') buf[16]+=7;
	buf[17]=((data->acc_z & 0x0F00)>> 8)+'0'; if(buf[17]>'9') buf[17]+=7;
	buf[18]=((data->acc_z & 0x00F0)>> 4)+'0'; if(buf[18]>'9') buf[18]+=7;
	buf[19]=(data->acc_z & 0x000F)      +'0'; if(buf[19]>'9') buf[19]+=7;
	buf[20]=',';
	buf[21]=((data->gyr_x & 0xF000)>>12)+'0'; if(buf[21]>'9') buf[21]+=7;
	buf[22]=((data->gyr_x & 0x0F00)>> 8)+'0'; if(buf[22]>'9') buf[22]+=7;
	buf[23]=((data->gyr_x & 0x00F0)>> 4)+'0'; if(buf[23]>'9') buf[23]+=7;
	buf[24]= (data->gyr_x & 0x000F)     +'0'; if(buf[24]>'9') buf[24]+=7;
	buf[25]=',';
	buf[26]=((data->gyr_y & 0xF000)>>12)+'0'; if(buf[26]>'9') buf[26]+=7;
	buf[27]=((data->gyr_y & 0x0F00)>> 8)+'0'; if(buf[27]>'9') buf[27]+=7;
	buf[28]=((data->gyr_y & 0x00F0)>> 4)+'0'; if(buf[28]>'9') buf[28]+=7;
	buf[29]= (data->gyr_y & 0x000F)     +'0'; if(buf[29]>'9') buf[29]+=7;
	buf[30]=',';
	buf[31]=((data->gyr_z & 0xF000)>>12)+'0'; if(buf[31]>'9') buf[31]+=7;
	buf[32]=((data->gyr_z & 0x0F00)>> 8)+'0'; if(buf[32]>'9') buf[32]+=7;
	buf[33]=((data->gyr_z & 0x00F0)>> 4)+'0'; if(buf[33]>'9') buf[33]+=7;
	buf[34]= (data->gyr_z & 0x000F)     +'0'; if(buf[34]>'9') buf[34]+=7;
	buf[35]='$';
	vcpTransmit(buf,36);
}

int main(void){
	lsm_data_t lsmData;
	HAL_Init();
	clockInit();
	ledInit();
	vcpInit();
	lsmInit();
	while(1){
		ledToggle();
		lsmGetData(&lsmData);
		printSerial(&lsmData);
	}
	return 1;
}
