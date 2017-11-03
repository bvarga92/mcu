/***************************************************************************/
/*                                                                         */
/* Az LCD kijelzohoz az ekezetes karakterek kodjai (max. 7 db.):           */
/*    - aa   - 1                                                           */
/*    - ee   - 2                                                           */
/*    - ii   - 3                                                           */
/*    - oo   - 4                                                           */
/*    - oe   - 5                                                           */
/*    - ooee - 6                                                           */
/*    - uu   - 7                                                           */
/*    - ue   - x (ebben a peldaban nincs, de a definicio megvan!)          */
/*    - uuee - x (ebben a peldaban nincs, de a definicio megvan!)          */
/*                                                                         */
/***************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

	#include <avr/io.h>

	#define LCD_RS_PORT	PORTD
	#define LCD_RS_DDR	DDRD
	#define LCD_RS_PIN	7
	#define LCD_E_PORT	PORTD
	#define LCD_E_DDR	DDRD
	#define LCD_E_PIN	4
	#define LCD_D4_PORT	PORTD
	#define LCD_D4_DDR	DDRD
	#define LCD_D4_PIN	3
	#define LCD_D5_PORT	PORTD
	#define LCD_D5_DDR	DDRD
	#define LCD_D5_PIN	2
	#define LCD_D6_PORT	PORTD
	#define LCD_D6_DDR	DDRD
	#define LCD_D6_PIN	1
	#define LCD_D7_PORT	PORTD
	#define LCD_D7_DDR	DDRD
	#define LCD_D7_PIN	0

	void lcdInit(void);
	void lcdCursorGoto(uint8_t line, uint8_t pos);
	void lcdPutString(char* str);
	void lcdPutInt(int32_t n);

#endif
