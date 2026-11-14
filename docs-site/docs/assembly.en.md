# Assembly & usage

Based on the [kit's instructions (Chinese PDF)](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/docs/guides/Drone%20Kit%20Assembly%20and%20Usage%20Instructions.pdf) and its [unofficial English translation](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/docs/guides/drone-kit-assembly-and-usage-instructions.en.md). The original PDF is authoritative; the photos and diagrams (motor positions, wire colors, propeller markings) are only there. The detailed soldering video is not in the repository because of its size.

!!! danger "Remove the propellers"
    Always remove the propellers when flashing, calibrating and testing motors. Never run the motors from USB power, use the battery.

## 1. Motors and propellers

1. **Motors**: pay attention to the positions. If you bought the finished kit, install the motors at the positions marked in the instructions and note the **wire colors**. For loose parts, follow the wire order you used when soldering (see the video).
2. **Propellers**: observe the markings (A/B). Wrongly installed propellers prevent stable flight.

## 2. Install the board and flash the firmware

1. Install the board with the **connector facing the same direction as the battery connector**, not the wrong way round.
2. Install the **CH340 driver** (see the [driver page](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). If the installer reports an error the driver may already be installed; it is enough if the Device Manager lists a CH340 COM port. Otherwise try another computer.
3. Flash the firmware: easiest with the [web flasher](flasher.md). Alternatively, in the Espressif Flash Download Tool select the firmware with "…", enter address `0x0000` and choose the **correct COM port**.
4. If no COM port appears despite the driver, your USB-C cable may be charge-only. Use another cable.

## 3. Setup and flight

1. Power on the drone. On your phone, find the Wi-Fi `XW_Drone_WiFi` and connect (password `12345678`), then open `192.168.4.1:8080` in the browser (type the colon in English keyboard mode).
2. Calibrate before the first flight: tap "Debug" in the interface, enter `ca` and follow the prompts (accelerometer calibration).
3. Test flight: first tap "Unlock" (arm); the propellers start. Left stick up raises throttle, left/right yaws. The right stick controls pitch/roll.
4. Landing: reduce throttle **slowly**, otherwise the drone drops. Tap "Lock" (disarm) to stop the motors. Do not fly too high on test flights.

!!! note "TODO(verify)"
    SSID, password and URL apply to the original `XW-Drone` firmware. Confirm them for the ported Flix firmware (`XW_Z1`), see [Remote control](remote-control.md).
