#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

int main(void){
	while(1){
		PORTB^=0x10;
		_delay_ms(200);
	}
	return 0;
}
