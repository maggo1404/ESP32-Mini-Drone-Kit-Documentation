# ESP32 Mini Drone Kit

Documentation, firmware and web flasher for the **ESP32 Mini Drone Kit** (board `XW_Z1 V2.0`, original firmware `XW-Drone` by 琛光 / ChenGuang). Most of the kit's original documents are in Chinese; this site summarizes them in German and English.

!!! danger "Safety"
    **Always remove the propellers** when flashing, configuring or running motor tests. Drones are dangerous; you use everything at your own risk. See [License & liability](license.md).

## What is the kit?

A small quadcopter with coreless motors, an MPU-6500/9250 attitude sensor and an ESP32 board. The drone opens its own Wi-Fi network and is controlled from a smartphone browser (web RC) or via MAVLink (QGroundControl). This repository also contains a newly built firmware based on [Flix](https://github.com/okalachev/flix) with support for the kit board.

## Quick start

<div class="grid cards" markdown>

-   **[Flash now](flasher.md)**

    Install the firmware directly from the browser (Chrome/Edge, USB cable).

-   **[Assembly](assembly.md)**

    Motors, propellers, board and first power-up.

-   **[Web remote control](remote-control.md)**

    Fly with your smartphone over the drone's Wi-Fi.

-   **[Troubleshooting](troubleshooting.md)**

    No COM port, drone won't take off, battery empty?

</div>

## Contents

| Page | Contents |
|---|---|
| [Hardware](hardware.md) | XW_Z1 V2.0 board, pin map (ESP32/C3/S3), schematic, 3D protective cover |
| [Assembly & usage](assembly.md) | Translated kit instructions |
| [Firmware](firmware.md) | Flix vs. XW-Drone, build targets, local build, parameters |
| [Web flasher](flasher.md) | Flash the firmware in the browser, alternatives |
| [Remote control](remote-control.md) | Web RC, QGroundControl, MAVLink |
| [Troubleshooting](troubleshooting.md) | Common problems |
| [License](license.md) | Licensing situation, unofficial distribution, disclaimer |

Sources and all original files: [GitHub repository](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation).
