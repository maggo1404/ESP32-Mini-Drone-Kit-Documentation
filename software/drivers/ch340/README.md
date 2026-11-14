# CH340 / CH341 USB-serial driver

The ESP32 board of the kit uses a CH340 USB-to-serial chip. On Windows a driver is needed before the board shows up as a COM port. The installer `CH341SER.EXE` that came with the kit is **not redistributed** in this repository. Download it from the manufacturer (WCH):

- https://www.wch-ic.com/downloads/CH341SER_EXE.html

## Installation (from the kit's note, `使用说明.txt`)

1. Double-click the downloaded `.EXE` file and click "Install".
2. If the installation fails, it is likely due to a previous installation or a computer issue: uninstall the program first and install it again.
3. The driver is working when the Windows Device Manager lists a CH340 serial port (COM port).
4. If no COM port is recognized, try another USB cable (some USB-C cables do not support data transfer) or another computer.

The original Chinese note `使用说明.txt` is kept in this folder; an English translation is in [`docs/guides/ch340-driver-installation-note.txt`](../../../docs/guides/ch340-driver-installation-note.txt).
