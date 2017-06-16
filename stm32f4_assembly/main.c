#include "stm32f4xx_hal.h"
#include "system.h"

static inline __attribute__((always_inline)) void asm_blinky(void){
	asm volatile(
		"LOOP:    ldr    r0, =0x40020010     \n" //r0-ba betoltjuk a GPIOA_IDR regiszter cimet
		"         ldr    r1, [r0]            \n" //r1-be betoltjuk az itt tarolt adatot
		"         tst    r1, #0x00000001     \n" //flagek beallitasa r1&1 szerint
		"         bne    LOOP                \n" //ha nyomjak a gombot, akkor ugras a LOOP cimkehez
		"         ldr    r0, =0x40020C14     \n" //r0-ba betoltjuk a GPIOD_ODR regiszter cimet
		"         ldr    r1, [r0]            \n" //r1-be betoltjuk az itt tarolt adatot
		"         eor    r1, #0x0000F000     \n" //osszeXORoljuk az erteket 0xF000-val (ivertaljuk a 4 LED-et)
		"         str    r1, [r0]            \n" //visszairjuk
		"         mov    r1, #0x00400000     \n" //ennyiszer hajtjuk vegre a DELAY ciklust
		"DELAY:   nop                        \n" //nem csinalunk semmit
		"         subs   r1, #1              \n" //csokkentjuk r1-et
		"         bne    DELAY               \n" //ha nem 0, akkor ugras a DELAY cimkehez
		"         b      LOOP                \n" //ugras a LOOP cimkehez
	);
}

int main(void){
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	asm_blinky();
	return 1;
}
