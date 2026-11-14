# Espressif Flash Download Tool

The kit ships with the Espressif **Flash Download Tool, version 3.9.5** (Windows). The executable is **not redistributed** in this repository (proprietary Espressif software). Download it from the official page:

- https://www.espressif.com/en/support/download/other-tools

This folder only keeps the tool's small configuration files (`configure/`, `combine/`) as they were delivered with the kit. The user guide is in [`docs/tool-manuals/`](../../../docs/tool-manuals/).

## Flashing the drone firmware (from the kit's assembly guide)

1. Install the CH340 USB-serial driver first (see [`software/drivers/ch340/`](../../drivers/ch340/README.md)).
2. Connect the drone's ESP32 board to the PC with a USB cable that supports data transfer (a charge-only USB-C cable is not recognized).
3. Start the Flash Download Tool, choose the chip type `ESP32`, and select the `Develop` / `SPI Download` mode.
4. Click `...` and select the firmware file [`firmware/binaries/XW-Drone.ino.merged.bin`](../../../firmware/binaries/XW-Drone.ino.merged.bin).
5. Enter the flash address `0x0000` (the merged image is flashed at address 0) and tick the file's checkbox.
6. Select the correct COM port and click `START`.

Steps 3 and 6 follow the tool's standard workflow described in the user guide; the kit's guide itself states: select the firmware with "...", enter the address `0x0000`, and be sure to select the COM port.
