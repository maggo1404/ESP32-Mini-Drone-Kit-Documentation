// Based on Flix by Oleg Kalachev (https://github.com/okalachev/flix)
// Features derived from XW-Drone by ChenGuang (firmware/source/XW-Drone/ in this repository),
// GPLv3 + Commons Clause, see firmware/source/XW-Drone/LICENSE.

// XW_Z1 board extras: throttle limits, low-battery protection, inverted flight
// protection and status LED patterns.
//
// Already provided by upstream Flix and therefore not duplicated here:
// - battery voltage measurement (power.ino, PWR_VOLT_* parameters)
// - RC loss failsafe with smooth descend (safety.ino)
// - disarm on excessive tilt in STAB mode (safety.ino, SF_DISARM_TILT)

#ifdef XW_Z1

#include "util.h"

// ---- Configuration ----

float xwThrMin = 0.10f; // thrust at the lowest stick position above the dead zone (MOT_THR_MIN)
float xwThrMax = 1.0f; // thrust at the highest stick position (MOT_THR_MAX)
float xwBatWarn = 3.5f; // L1: arming is prohibited / idle drone is disarmed below this voltage (PWR_VOLT_WARN)
float xwBatCrit = 3.0f; // L3: automatic landing below this voltage in flight (PWR_VOLT_CRIT)
int xwLedPin = -1;

// L2 (in flight warning) is the upstream voltageLow parameter (PWR_VOLT_LOW)
const float XW_BAT_ABSENT = 0.5f; // below this the battery is considered absent (powered from USB), ignore all alerts
const float XW_BAT_FLYING_THRUST = 0.15f; // thrust at or above this value is considered flying
const float XW_BAT_DEBOUNCE = 0.9f; // low voltage must persist this long (s) to trigger an action
const float XW_THROTTLE_DEADZONE = 0.05f; // same as upstream arming/idle threshold
const float XW_MIN_CONTROLLED_THRUST = 0.1f; // upstream does not run the controllers below this thrust

const float XW_INVERTED_COS = -0.7f; // up vector Z below this is inverted (tilt > 134 degrees)
const float XW_INVERTED_TIMEOUT = 1.5f; // s

const unsigned XW_BLINK_SLOW = 500000; // armed: half period 500 ms (1 Hz)
const unsigned XW_BLINK_FAST = 62500; // warning: half period 62.5 ms (8 Hz)

extern float voltage, voltageLow;
extern float thrustTarget, descendTime, rcLossTimeout, controlTime, motors[4];
extern bool armed;
extern Quaternion attitude;

static float xwRawThrottle = 0; // throttle command before applying MOT_THR_MIN / MOT_THR_MAX
static bool xwInverted = false;
static bool xwBatLatched = false; // L3 automatic landing is in progress

// ---- Helpers ----

float xwThrMinEffective() {
	return constrain(xwThrMin, XW_MIN_CONTROLLED_THRUST, 1.0f);
}

bool xwBatteryPresent() {
	return valid(voltage) && voltage > XW_BAT_ABSENT;
}

void xwStopMotors() {
	armed = false;
	thrustTarget = 0;
	for (float& m : motors) m = 0;
}

void xwWarn(const char *format, ...) {
	char buf[96];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	print("%s\n", buf);
	xwWebWarn(buf);
}

// ---- Hooks ----

// Called before control(): web RC input and throttle limits
void xwBeforeControl() {
	xwApplyWebRC();

	// Map the throttle stick to [MOT_THR_MIN, MOT_THR_MAX]. The lower dead zone is left untouched
	// to keep the arm / disarm gestures and the idle behavior.
	xwRawThrottle = controlThrottle;
	if (controlThrottle >= XW_THROTTLE_DEADZONE) {
		float low = xwThrMinEffective();
		float high = constrain(xwThrMax, low, 1.0f);
		controlThrottle = mapf(controlThrottle, XW_THROTTLE_DEADZONE, 1, low, high);
	}
}

// Called after control(), before the motors are updated
void xwAfterControl() {
	controlThrottle = xwRawThrottle; // restore the pilot's value

	// Idle thrust is MOT_THR_MIN
	float idle = xwThrMinEffective();
	if (armed && thrustTarget < XW_MIN_CONTROLLED_THRUST && idle > XW_MIN_CONTROLLED_THRUST) {
		bool idling = true;
		for (float m : motors) idling &= (m == XW_MIN_CONTROLLED_THRUST);
		if (idling) for (float& m : motors) m = idle;
	}

	xwArmingLockout();
	xwInvertedFailsafe();
	xwBatteryFailsafe();
}

// ---- Safety ----

// Prohibit arming on low battery
void xwArmingLockout() {
	static bool wasArmed = false;
	if (armed && !wasArmed && xwBatteryPresent() && voltage < xwBatWarn) {
		xwStopMotors();
		xwWarn("Battery low (%.2fV), arming prohibited", voltage);
	}
	wasArmed = armed;
}

// Stop the motors if the drone stays upside down. Upstream only disarms in STAB mode (SF_DISARM_TILT),
// this covers all modes with a timeout to avoid reacting to flips in ACRO mode.
void xwInvertedFailsafe() {
	static float since = 0;
	xwInverted = false;
	if (!armed) {
		since = 0;
		return;
	}
	Vector up = Quaternion::rotateVector(Vector(0, 0, 1), attitude);
	if (up.z < XW_INVERTED_COS) {
		xwInverted = true;
		if (since == 0) since = t;
		if (t - since > XW_INVERTED_TIMEOUT) {
			xwStopMotors();
			since = 0;
			xwWarn("Inverted flight protection: motors stopped");
		}
	} else {
		since = 0;
	}
}

// Low battery protection:
// - on the ground (idle) below PWR_VOLT_WARN: disarm
// - in flight below PWR_VOLT_CRIT: land automatically (latched until disarmed)
// - in flight below PWR_VOLT_LOW: only the LED warning
void xwBatteryFailsafe() {
	static float lowSince = 0;
	static float lastNotify = 0;

	if (!armed || !xwBatteryPresent()) {
		xwBatLatched = false;
		lowSince = 0;
		lastNotify = 0;
		return;
	}

	if (xwBatLatched) {
		if (t - lastNotify >= 2) {
			xwWarn("Battery critical (%.2fV), landing, thrust %.0f%%", voltage, thrustTarget * 100);
			lastNotify = t;
		}
		descend();
		if (!armed) xwStopMotors();
		return;
	}

	bool flying = thrustTarget >= XW_BAT_FLYING_THRUST;
	bool critical = flying && voltage < xwBatCrit;
	bool idleLow = !flying && voltage < xwBatWarn;

	if (!critical && !idleLow) {
		lowSince = 0;
		return;
	}

	if (lowSince == 0) lowSince = t;
	if (t - lowSince < XW_BAT_DEBOUNCE) return;
	lowSince = 0;

	if (critical) {
		xwBatLatched = true;
		lastNotify = t;
		xwWarn("Battery critical (%.2fV), landing automatically", voltage);
		descend();
	} else {
		xwStopMotors();
		xwWarn("Battery low (%.2fV), disarmed", voltage);
	}
}

// ---- Status LED ----
// Disarmed: off (fast blink if the battery is low). Armed: slow blink, fast blink on any warning.

bool xwBatteryAlert() {
	if (!xwBatteryPresent()) return false;
	bool flying = armed && thrustTarget >= XW_BAT_FLYING_THRUST;
	return voltage < (flying ? voltageLow : xwBatWarn);
}

bool xwAlertActive() {
	if (xwInverted) return true;
	if (xwBatteryAlert()) return true;
	if (armed && xwWebRCLost()) return true; // web RC link lost
	if (armed && valid(controlTime) && t - controlTime > rcLossTimeout) return true; // RC lost
	return false;
}

void xwLoopLED() {
	if (xwLedPin < 0) return;
	bool on;
	if (!armed) {
		on = xwBatteryAlert() ? (micros() / XW_BLINK_FAST % 2) : false;
	} else {
		on = micros() / (xwAlertActive() ? XW_BLINK_FAST : XW_BLINK_SLOW) % 2;
	}
	digitalWrite(xwLedPin, on);
}

// ---- Setup and loop ----

void setupXW() {
	if (xwLedPin >= 0) {
		pinMode(xwLedPin, OUTPUT);
		digitalWrite(xwLedPin, HIGH); // on during startup
	}
	setupWebRC();
}

void loopXW() {
	loopWebRC();
	xwLoopLED();
}

#endif // XW_Z1
