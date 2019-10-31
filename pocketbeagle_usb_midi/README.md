# USB-MIDI gateway

![photo](https://raw.githubusercontent.com/bvarga92/mcu/main/pocketbeagle_usb_midi/photo.jpg)

## Telepítés:

- `gcc setbaud.c -o setbaud`
- `sudo chmod +x midigw`
- `sudo crontab -e`, majd adjuk hozzá a következõ feladatot: `@reboot /home/debian/midigw`
