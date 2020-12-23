# USB-MIDI gateway

![photo_top](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/photo_top.jpg)

![photo_bottom](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/photo_bottom.jpg)

![schematic](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/schematic.png)

## Telepítés:

- `gcc setbaud.c -o setbaud`
- `sudo chmod +x midigw`
- `sudo crontab -e`, majd adjuk hozzá a következõ feladatot: `@reboot /home/debian/midigw`
