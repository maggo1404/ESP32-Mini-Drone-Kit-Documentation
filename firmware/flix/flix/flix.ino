// Copyright (c) 2023 Oleg Kalachev <okalachev@gmail.com>
// Repository: https://github.com/okalachev/flix

// Main firmware file

#include "config.h"
#include "vector.h"
#include "quaternion.h"
#include "util.h"

extern float t, dt;
extern float controlRoll, controlPitch, controlYaw, controlThrottle, controlMode;
extern Vector gyro, acc;
extern Vector rates;
extern Quaternion attitude;
extern bool landed;
extern float motors[4];

void setup() {
	setupConfig();
	setupConsole();
	print("Initializing Flix\n");
	setupParameters();
	setupPower();
	setupLED();
	setupMotors();
	setupWiFi();
	setupIMU();
	setupRC();
	setupLog();
#ifdef XW_Z1
	setupXW();
#endif
	print("Initializing complete\n");
	printHelp();
}

void loop() {
	readIMU();
	step();
	readRC();
	estimate();
#ifdef XW_Z1
	xwBeforeControl();
#endif
	control();
#ifdef XW_Z1
	xwAfterControl();
#endif
	sendMotors();
	loopConsole();
	loopMavlink();
	loopPower();
	loopLED();
	loopLog();
	syncParameters();
#ifdef XW_Z1
	loopXW();
#endif
}
