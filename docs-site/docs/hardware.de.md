# Hardware

## Platine XW_Z1 V2.0

Die Flugsteuerung des Kits ist eine ESP32-Platine mit

- ESP32 (Standardausführung; neuere Varianten mit **ESP32-C3** oder **ESP32-S3**),
- SPI-Lagesensor MPU-6500 / MPU-9250,
- USB-Seriell-Chip **CH340** (Treiber nötig, siehe [Fehlerbehebung](troubleshooting.md#kein-com-port)),
- vier MOSFET-Treibern für Hohlbechermotoren (PWM),
- Akku-Spannungsteiler (ADC) und SBUS-Eingang.

Die Platine wird so eingebaut, dass der **Stecker in dieselbe Richtung zeigt wie der Akkustecker**; nicht verkehrt herum einstecken.

## Pinbelegung

Quelle: `board_config.h` der Original-Firmware `XW-Drone` ([GitHub](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/firmware/source/XW-Drone/board_config.h)). Die Belegung wird dort per Chip-Typ automatisch gewählt.

| Funktion | ESP32 | ESP32-C3 | ESP32-S3 |
|---|---|---|---|
| Motor hinten links (RL) | GPIO 12 | GPIO 4 | GPIO 4 |
| Motor hinten rechts (RR) | GPIO 13 | GPIO 5 | GPIO 5 |
| Motor vorne rechts (FR) | GPIO 15 | GPIO 8 | GPIO 6 |
| Motor vorne links (FL) | GPIO 14 | GPIO 3 | GPIO 7 |
| Akku-Spannung (ADC) | GPIO 36 | GPIO 1 | GPIO 1 |
| SBUS-Empfänger (RX) | GPIO 16 (Serial2) | GPIO 9 (Serial1) | GPIO 8 (Serial2) |
| IMU SPI SCK | GPIO 18 | GPIO 6 | GPIO 12 |
| IMU SPI MISO | GPIO 19 | GPIO 2 | GPIO 13 |
| IMU SPI MOSI | GPIO 23 | GPIO 7 | GPIO 11 |
| IMU SPI CS | GPIO 5 | GPIO 10 | GPIO 10 |
| Status-LED | GPIO 2 | keine (deaktiviert) | GPIO 2 |

!!! note "TODO(verify)"
    Pinbelegung der portierten Flix-Firmware (`XW_Z1`-Target) gegen `firmware/flix/flix/config.h` bzw. die Board-Definition prüfen; die Tabelle gibt die Belegung der Original-Firmware wieder. Die Pins lassen sich in Flix auch per Parameter ändern (`MOT_PIN_*`, `IMU_PIN_*`, `RC_RX_PIN`, `PWR_VOLT_PIN`).

Hinweise der Original-Firmware: Beim C3 ist GPIO 8 ein Strapping-Pin (interner Pull-up, PWM funktioniert trotzdem), GPIO 9 ist die BOOT-Taste.

## Schaltplan

[SCH_XW_Z1_V2.0.pdf](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/hardware/schematics/SCH_XW_Z1_V2.0.pdf) (PDF im GitHub-Repository). Das Projekt wird laut Hersteller auf [OSHWHub](https://oshwhub.com/songge8/project_qqqyfdkm) veröffentlicht; die dortigen Lizenzbedingungen für die Hardware wurden nicht geprüft.

## 3D-Schutzhülle

Eine druckbare Schutzhülle liegt als STL bei: [Drone protective cover.stl](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/hardware/3d-models/Drone%20protective%20cover.stl). Die Datei enthält keine Lizenzangabe.
