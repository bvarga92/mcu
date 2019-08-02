#ifndef _BME280_H_
#define _BME280_H_

	#include <stdint.h>

	typedef struct{
		int32_t temperature_x100, humidity, pressure;
	} bme280data_t;

	void bme280Init(uint8_t *i2cBuffer);
	void bme280Read(bme280data_t *bme280data);

#endif
