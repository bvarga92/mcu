#ifndef _I2C_H_
#define _I2C_H_

	#include <stdint.h>

	#define DELAY_LONG   5
	#define DELAY_SHORT  4
	#define I2C_DDR      DDRB
	#define I2C_PORT     PORTB
	#define I2C_PIN      PINB
	#define SDA_PIN      PB0
	#define SCL_PIN      PB2

	#define NOSTART      1
	#define NOSTOP       2
	#define MEMREAD      4
	#define FLASH        8

	void i2cInit(void);
	void i2cTransfer(uint8_t *buffer, uint8_t size, uint8_t flags);

#endif
