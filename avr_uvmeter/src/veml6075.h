#ifndef _VEML6075_H_
#define _VEML6075_H_

	#include <stdint.h>

	typedef struct{
		uint32_t uvia_x10M, uvib_x10M;
	} uvdata_t;

	void veml6075Init(uint8_t *i2cBuffer);
	void veml6075Read(uvdata_t *uvdata);

#endif
