# USB-MIDI gateway

![photo_top](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/photo_top.jpg)

![photo_bottom](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/photo_bottom.jpg)

![schematic](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/schematic.png)

## Telep�t�s:

- `gcc setbaud.c -o setbaud`
- `sudo chmod +x midigw`
- `sudo crontab -e`, majd adjuk hozz� a k�vetkez� feladatot: `@reboot /home/debian/midigw`
