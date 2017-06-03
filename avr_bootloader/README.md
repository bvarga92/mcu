**Bootloader példaprogram a T-Bird 2 fejlesztőkártyához**

Reset után a bootloader az EEPROM memória első 512 bájtját a flash memóriába másolja. Miután ez elkészült, hangjelzést hallunk. Ekkor az első nyomógomb megnyomásával befejezhetjük a bootolást, és elindíthatjuk a főprogram futását. A főprogram egy LED-et villogtat.

A *toFlash* projekt tartalmazza a bootloadert, ezt a kontroller flash memóriájának BLS (Boot Loader Section) részébe kell beírni, amelynek kezdőcímét ATmega128 esetén a BOOTSZ0 és BOOTSZ1 fuse bitek határozzák meg. Most válasszunk olyan konfigurációt, amelyben a BLS mérete a maximális 4kB! Ekkor a BLS kezdőcíme 0x1E000. Úgy érhetjük el, hogy a teljes kód ide forduljon, hogy a makefile-ban megadjuk a linkernek a `-Wl,--section-start=.text=0x1E000` flaget. A fuse biteket úgy kell beállítani, hogy a 0x0000 cím helyett a BLS kezdőcímétől induljon a futás (BOOTRST, BOOTSZ0 és BOOTSZ1 bekapcs).

A *toEEPROM* projekt tartalmazza az EEPROM memóriába írandó kódot.

![programozás](https://github.com/bvarga92/mcu/raw/main/avr_bootloader/program.png)
	
Természetesen a valóságban nem sok értelme van a belső EEPROM-ból betölteni a programot, azonban ezen demonstráció alapján könnyen implementálható UART-ról, USB-ről, vagy SD kártyáról való bootolás is.
