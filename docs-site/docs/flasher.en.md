# Web flasher

!!! warning "Unofficial firmware, at your own risk"
    These binaries are **unofficial** and provided without warranty for kit owners. Flix has no license (all rights reserved by Oleg Kalachev), see [License](license.md). **Remove the propellers** before flashing or testing. Flashing overwrites the existing firmware; you can restore the original using `firmware/binaries/XW-Drone.ino.merged.bin`.

## Install the firmware

<div id="flasher"><noscript>The flasher requires JavaScript.</noscript></div>

Choose your board and click "Install firmware". Select the drone's COM port in the dialog. The "Erase device" option (default) wipes the flash before installing, which deletes stored parameters and calibrations.

| Choice | When? |
|---|---|
| **XW-Z1 (ESP32)** | Standard kit board XW_Z1 V2.0 |
| **XW-Z1 C3** | Board variant with ESP32-C3 |
| **XW-Z1 S3** | Board variant with ESP32-S3 |
| **Flix stock (ESP32)** | Unmodified Flix firmware for ESP32 boards with Flix pinout; **not** meant for the kit board (different pins) |

## Requirements

- **Chrome or Edge on a desktop** (Windows, macOS, Linux, ChromeOS). Firefox, Safari and mobile browsers do not support Web Serial.
- **CH340 driver** installed on Windows ([instructions and download link](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). The board must show up as a COM port.
- **USB data cable** (not charge-only).
- Page opened via HTTPS (GitHub Pages: yes).
- **Propellers removed**; no battery needed (USB is enough for flashing).

If connecting fails, hold the **BOOT** button, tap **EN/RST** briefly (or replug USB) and release BOOT once flashing starts. Usually the CH340 circuit does this automatically.

## If your browser is not supported

The button then shows a warning. Alternatives:

1. **Espressif Flash Download Tool** (Windows): chip `ESP32`, mode *Develop / SPI Download*, select `flix.ino.merged.bin` (download above under "Direct downloads"), address `0x0000`, choose the right COM port, *START*. Download and guide: [Flash Download Tool](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/tools/flash-download-tool/README.md).
2. **esptool** (Python):

   ```bash
   pip install esptool
   esptool.py --chip esp32 write_flash 0x0 flix.ino.merged.bin
   ```

   Use `--chip esp32c3` / `--chip esp32s3` for C3/S3. Add `--port COM5` or `--port /dev/ttyUSB0` if needed.

## After flashing

Power the drone from the battery, run the calibration (`ca`) and connect via [remote control](remote-control.md). Problems: [Troubleshooting](troubleshooting.md).
