#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/boot.h>

int main(void){
	uint16_t i;
	uint8_t eepromBuffer[512];
	uint32_t pageAddr;
	DDRB=0x10; //LED, amit majd a foprogram villogtat
	DDRE=0x08; //buzzer
	DDRG=0x00; //nyomogomb
	for(i=0;i<512;i++) eepromBuffer[i]=eeprom_read_byte((uint8_t*)i); //beolvassuk az EEPROM elso 512 bajtjat
	for(pageAddr=0;pageAddr<512;pageAddr+=256){
		boot_page_erase_safe(pageAddr); //toroljuk a flash memorianak az aktualis lapjat
		for(i=0;i<256;i+=2) boot_page_fill_safe(i,eepromBuffer[pageAddr+i]+(eepromBuffer[pageAddr+i+1]<<8)); //az EEPROM tartalmat atmasoljuk a flash bufferbe
		boot_page_write_safe(pageAddr); //beirjuk a buffer tartalmat a flashbe
	}
	while(!(PING&0x01)){PORTE^=0x08; _delay_ms(2);} //hangjelzes gombnyomasig
	boot_rww_enable_safe(); //programozas vege
	asm("jmp 0x0000"); //ugras a foprogramra
	return 0;
}
