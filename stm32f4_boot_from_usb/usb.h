#ifndef _USB_H_
#define _USB_H_

	/* megszakitaskezelo */
	void OTG_FS_IRQHandler(void);

	/* vegtelen ciklusban hivogassuk */
	void usbHostProcess(void);

	/* inicializalas */
	void usbInit(void);

	/* sikeres inicializalas eseten visszaadja a hattertar gyokerenek eleresi utjat */
	char* usbGetPath(void);

	/* automatikusan hivodnak, implementalni kell tetszes szerint */
	extern void usbOnConnect(void);
	extern void usbOnClassActive(void);
	extern void usbOnDisconnect(void);

#endif
