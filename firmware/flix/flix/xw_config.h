// Based on Flix by Oleg Kalachev (https://github.com/okalachev/flix)
// Board support and features derived from XW-Drone by ChenGuang
// (firmware/source/XW-Drone/ in this repository), GPLv3 + Commons Clause.
// See the LICENSE file in firmware/source/XW-Drone/.

// XW_Z1 board target: board configuration and hook declarations.
//
// Everything here is active only when the sketch is built with -DXW_Z1
// (firmware/build.sh xw-z1 | xw-z1-c3 | xw-z1-s3). Without it this file is empty,
// so the stock Flix build stays unchanged.
//
// In Arduino IDE, enable the target by adding "#define XW_Z1" before including this file.

#pragma once

#ifdef XW_Z1

#include "vector.h"

// Variables of the upstream code that are configured here
extern int imuSckPin, imuMisoPin, imuMosiPin;

// XW feature variables (defined in xw_safety.ino)
extern float xwThrMin, xwThrMax; // throttle limits (MOT_THR_MIN, MOT_THR_MAX)
extern float xwBatWarn, xwBatCrit; // battery thresholds, V (PWR_VOLT_WARN, PWR_VOLT_CRIT)
extern int xwLedPin; // status LED pin, -1 if the board has no LED

// Parameters added by the XW_Z1 target (expanded inside the parameters[] array)
#define XW_PARAMETERS \
	{"MOT_THR_MIN", &xwThrMin}, \
	{"MOT_THR_MAX", &xwThrMax}, \
	{"PWR_VOLT_WARN", &xwBatWarn}, \
	{"PWR_VOLT_CRIT", &xwBatCrit},

// Default Wi-Fi access point credentials of the XW-Drone firmware
#define XW_WIFI_AP_SSID "XW_Drone_WiFi"
#define XW_WIFI_AP_PASS "12345678"

void xwSeedWiFiCredentials();

// Board pin configuration (called at the end of setupConfig())
// Pins are taken from XW-Drone's board_config.h.
void xwSetupConfig() {
	// IMU: MPU-6500 / MPU-9250 on SPI, mounted rotated by -90 degrees around Z
	imuModel = 1;
	imuRotation = Vector(0, 0, -PI / 2);

	// Battery voltage: 10k + 33k divider (Vbat = Vadc * 43 / 33), 1S LiPo
	voltageScale = 43.0f / 33.0f;
	voltageFilter.alpha = 0.2; // loopPower() runs at 10 Hz, XW averaged the ADC over 0.5 s
	voltageLow = 3.3; // in flight low battery threshold (XW L2)

#if defined(CONFIG_IDF_TARGET_ESP32C3)
	// ESP32-C3 (no status LED to save pins)
	motorPins[MOT_RL] = 4;
	motorPins[MOT_RR] = 5;
	motorPins[MOT_FR] = 8; // strapping pin, internal pull-up is fine for PWM output
	motorPins[MOT_FL] = 3;
	voltagePin = 1;
	rcRxPin = 9; // SBUS, UART1 on a remapped pin
	imuSckPin = 6;
	imuMisoPin = 2;
	imuMosiPin = 7;
	imuCsPin = 10;
	xwLedPin = -1;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
	// ESP32-S3
	motorPins[MOT_RL] = 4;
	motorPins[MOT_RR] = 5;
	motorPins[MOT_FR] = 6;
	motorPins[MOT_FL] = 7;
	voltagePin = 1;
	rcRxPin = 8;
	imuSckPin = 12;
	imuMisoPin = 13;
	imuMosiPin = 11;
	imuCsPin = 10;
	xwLedPin = 2;
#else
	// Classic ESP32 (XW_Z1 V2.0 kit)
	motorPins[MOT_RL] = 12;
	motorPins[MOT_RR] = 13;
	motorPins[MOT_FR] = 15;
	motorPins[MOT_FL] = 14;
	voltagePin = 36;
	rcRxPin = 16;
	imuSckPin = 18;
	imuMisoPin = 19;
	imuMosiPin = 23;
	imuCsPin = 5;
	xwLedPin = 2;
#endif

	ledType = -1; // disable the upstream LED subsystem, the XW status LED is driven by xw_safety.ino
	xwSeedWiFiCredentials();
}

#endif // XW_Z1
