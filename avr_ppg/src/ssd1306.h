/***************************************************************************/
/*                                                                         */
/*  Driver az SSD1306 kijelzohoz (128x64)               Varga Balazs 2015  */
/*                                                                         */
/*  Minden fuggveny a frame bufferbe dolgozik, a kijelzo frissitesehez az  */
/*  updateDisplay() fuggvenyt kell meghivni!                               */
/*                                                                         */
/***************************************************************************/

#ifndef _SSD1306_H_
#define _SSD1306_H_

	#include <avr/io.h>
	#include <stdbool.h>

	/* kijelzo inicializalasa */
	void displayInit(void);

	/* megjeleniti a frame buffer tartalmat */
	void displayUpdate(void);

	/* torli a frame buffer tartalmat */
	void displayClear(void);

	/* bekapcsolja a megadott koordinataju pixelt */
	void displaySetPixel(uint8_t x, uint8_t y);

	/* kikapcsolja a megadott koordinataju pixelt */
	void displayClearPixel(uint8_t x, uint8_t y);

	/* invertalja a kepet */
	void displayInvert(void);

	/* beallitja a szovegkurzort megadott pozicioba */
	void displaySetCursor(uint8_t x, uint8_t y);

	/* kiirja az atadott karaktert a kurzor helyere */
	void displayPrintChar(char ch);

	/* kiirja az atadott nullterminalt sztringet a kurzor helyere */
	void displayPrintText(char* str);

	/* kiirja az atadott egesz szamot a kurzor helyere */
	void displayPrintInteger(int n);

#endif
