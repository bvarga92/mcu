A fejlesztés menete:
1. Adatok gyűjtése a MATLAB vagy a C# GUI-val. A kimenet a nyers jeleket tartalmazó TXT fájl.
2. A jel szegmentálása és feature-ök generálása a process.m szkripttel. A kimenet a feature mátrixot tartalmazó ARFF fájl.
3. Az ARFF fájl beolvasása a Weka nevű szoftverrel, döntési fa illesztése. A kimenet a döntési szabályokat tartalmazó TXT fájl.
4. A fát tartalmazó TXT fájlból a konfigurációs UCF fájl előállítása az Unico nevű segédprogrammal.
5. A process.m szkripttel generálható a konfigurációs adattömböt tartalmazó lsm6dsox_conf.h fájl.
6. A firmware újrafordítása az lsm6dsox_conf.h fejléccel.

![gui_screenshot](https://raw.githubusercontent.com/bvarga92/mcu/main/stm32f103_lsm6dsox/gui_screenshot.png)

![blokkvazlat](https://raw.githubusercontent.com/bvarga92/mcu/main/stm32f103_lsm6dsox/blokkvazlat.png)