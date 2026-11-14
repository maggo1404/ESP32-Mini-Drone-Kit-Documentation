# ESP32 Drone Flight Controller Firmware

> Unofficial English translation of [`README.MD`](README.MD) (original in Chinese: "ESP32无人机飞控固件"). The original is authoritative.

# Hardware information

## Supported hardware

Adapted for ESP32 / ESP32-S3 / ESP32-C3.
Supports the 6500/9250 attitude sensors (MPU-6500 / MPU-9250).

Perfectly adapted to the 琛光 (ChenGuang) E1 series drones. Link to the project on the JLC open-source platform (OSHWHub): https://oshwhub.com/songge8/project_qqqyfdkm

# Changelog

## 2026-05-15 (20260515)
* Improved voltage measurement accuracy
* More reasonable low-battery protection threshold

## 2026-04-30 (20260430)
* Improved battery alarm logic: when the battery is low, arming is prohibited, and the drone automatically disarms and lands automatically
* Optimized throttle thrust control logic; added the parameters `MOT_THR_MIN` and `MOT_THR_MAX` for the lower and upper throttle thrust limits
* Optimized the web remote-control interface

## 2026-04-28 (20260428)
* Adapted to ESP32-C3 and ESP32-S3 (the original writes "EPS32-C3")

## 2026-04-17 (20260417)
* Fixed a bug where the debug log was displayed incompletely, and improved the display of the prompt commands

## 2026-04-16 (20260416)
* Reduced the print frequency of the routine remote-control debug logs
* The server cannot monitor the network signal quality accurately; this is already implemented accurately in the front end, so the server-side network monitoring was removed

## 2026-04-15 (20260415)
* Added LED status indication: the development board's status LED blinks slowly when unlocked (armed) and quickly for warning states such as low battery or lost connection
* Motor pins are now parameters and can be set through the debug console
* Wi-Fi user name and password are now parameters and can be set through the debug console
* Fixed an error in the network latency calculation logic
* Moderately increased the web remote-control packet rate to 20 Hz; to avoid affecting performance, do not exceed 20 Hz
* Updated some general functions

## 2026-03-28 (20260328)
* Refactored the low-level logic of the web remote control
* Removed redundant code from the web remote-control server

## 2026-03-27 (20260327)
* Greatly optimized the web remote control
* Added a debug function to the web remote control
* Added inverted-flight protection: if the aircraft stays upside down for a long time it is treated as abnormal and the motors are stopped to avoid further damage
