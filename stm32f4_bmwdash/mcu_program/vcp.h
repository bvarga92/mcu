#ifndef _VCP_H_
#define _VCP_H_

	#include "system.h"

	/* megszakitaskezelo */
	void OTG_FS_IRQHandler(void);

	/* inicializalja az USB VCP-t */
	void vcpInit(void);

	/* adatkuldes */
	void vcpTransmit(uint8_t* buffer, uint32_t length);

	/* vetelkor ez hivodik, implementalni kell tetszes szerint */
	extern void vcpReceived(uint8_t data);

#endif
