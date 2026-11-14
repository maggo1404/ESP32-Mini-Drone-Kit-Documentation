# Hardware

## XW_Z1 V2.0 board

The kit's flight controller is an ESP32 board with

- ESP32 (standard version; newer variants with **ESP32-C3** or **ESP32-S3**),
- SPI attitude sensor MPU-6500 / MPU-9250,
- **CH340** USB-serial chip (driver required, see [Troubleshooting](troubleshooting.md#no-com-port)),
- four MOSFET drivers for coreless motors (PWM),
- battery voltage divider (ADC) and SBUS input.

Install the board with the **connector facing the same direction as the battery connector**; do not plug it in the wrong way round.

## Pin map

Pins of the `XW_Z1` target (`xw_config.h`), derived from `board_config.h` of the original `XW-Drone` firmware ([GitHub](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/firmware/source/XW-Drone/board_config.h)); they are selected automatically by chip type.

| Function | ESP32 | ESP32-C3 | ESP32-S3 |
|---|---|---|---|
| Motor rear left (RL) | GPIO 12 | GPIO 4 | GPIO 4 |
| Motor rear right (RR) | GPIO 13 | GPIO 5 | GPIO 5 |
| Motor front right (FR) | GPIO 15 | GPIO 8 | GPIO 6 |
| Motor front left (FL) | GPIO 14 | GPIO 3 | GPIO 7 |
| Battery voltage (ADC) | GPIO 36 | GPIO 1 | GPIO 1 |
| SBUS receiver (RX) | GPIO 16 (Serial1) | GPIO 9 (Serial1) | GPIO 8 (Serial1) |
| IMU SPI SCK | GPIO 18 | GPIO 6 | GPIO 12 |
| IMU SPI MISO | GPIO 19 | GPIO 2 | GPIO 13 |
| IMU SPI MOSI | GPIO 23 | GPIO 7 | GPIO 11 |
| IMU SPI CS | GPIO 5 | GPIO 10 | GPIO 10 |
| Status LED | GPIO 2 | none (disabled) | GPIO 2 |

The table matches the ported Flix firmware (`XW_Z1` target, `xw_config.h`). Note: FR and FL are swapped compared to stock Flix (14/15) on the ESP32. The RC receiver uses Serial1 on all chips. IMU: MPU-6500/9250, rotated by -PI/2 around yaw (`IMU_ROT_YAW`). The PWM frequency is the upstream default (38 kHz on C3/S3), not the 78 kHz of the original XW firmware. The pins can also be changed via parameters (`MOT_PIN_*`, `IMU_PIN_*`, `RC_RX_PIN`, `PWR_VOLT_PIN`). None of this has been tested on real hardware yet.

Notes from the original firmware: on the C3, GPIO 8 is a strapping pin (internal pull-up, PWM works anyway), GPIO 9 is the BOOT button.

## Schematic

[SCH_XW_Z1_V2.0.pdf](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/hardware/schematics/SCH_XW_Z1_V2.0.pdf) (PDF in the GitHub repository). According to the vendor the project is published on [OSHWHub](https://oshwhub.com/songge8/project_qqqyfdkm); the hardware license terms there have not been checked.

## 3D protective cover

A printable protective cover is provided as STL: [Drone protective cover.stl](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/hardware/3d-models/Drone%20protective%20cover.stl). The file carries no license information.
