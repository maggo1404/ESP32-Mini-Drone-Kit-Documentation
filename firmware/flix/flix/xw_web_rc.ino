// Based on Flix by Oleg Kalachev (https://github.com/okalachev/flix)
// Web remote control derived from XW-Drone by ChenGuang
// (firmware/source/XW-Drone/web_rc.ino in this repository), GPLv3 + Commons Clause,
// see firmware/source/XW-Drone/LICENSE.

// Web remote control: control the drone from a phone browser over the Flix Wi-Fi access point.
// Open http://192.168.4.1:8080 after connecting to the drone's Wi-Fi network.
//
// The web inputs are written to the same control variables as the SBUS receiver
// (controlRoll, controlPitch, controlYaw, controlThrottle) and have priority over them while the
// web page is connected. Arming / disarming and flight mode are handled by the page's buttons.

#ifdef XW_Z1

#include <WebServer.h>
#include <Preferences.h>
#include "util.h"
#include "xw_web_rc_html.h"

extern float t, controlTime;
extern float controlRoll, controlPitch, controlYaw, controlThrottle, controlMode;
extern float voltage, thrustTarget, motors[4];
extern bool armed;
extern int mode;
extern const int RAW, ACRO, STAB, AUTO;

// ---- Configuration ----

const unsigned long WEB_RC_TIMEOUT_MS = 8000; // no packets from the page for this time means the link is lost; must be longer than the 2 s heartbeat
const float WEB_RC_ARM_THROTTLE_LIMIT = 0.05; // arming is only possible if the throttle is below this value

// Sensitivity (the page sends values in range [-100, 100])
const float WEB_RC_THROTTLE_SCALE = 1.0; // maximum throttle, 0.8 limits it to 80%
const float WEB_RC_STICK_SCALE = 0.85; // roll / pitch sensitivity, 0.5 - 1.0
const float WEB_RC_YAW_SCALE = 0.68; // yaw sensitivity, 0.3 - 0.8
const float WEB_RC_STICK_DEADZONE = 0.06; // stick dead zone (fraction of full deflection)
const float WEB_RC_THROTTLE_DEADZONE = 0.06; // throttle dead zone (fraction of full travel)
const float WEB_RC_RAW_MAX = 100; // full deflection value sent by the page

// Button bits sent by the page
const int WEB_BTN_ARM = 0, WEB_BTN_DISARM = 1, WEB_BTN_ESTOP = 2, WEB_BTN_STAB = 6, WEB_BTN_ACRO = 7;

// ---- State ----

WebServer webRCServer(8080);

bool webRCUpdated = false; // received at least one packet
unsigned long webRCLastUpdate = 0; // millis() of the last packet
uint16_t webRCButtons = 0;
float webRCRoll = 0, webRCPitch = 0, webRCYaw = 0, webRCThrottle = 0; // processed inputs, normalized
char webRCWarnMsg[96] = ""; // warning to deliver to the page with the next response

bool webConsoleEnabled = false; // debug console is active, print() output is buffered

// Debug console ring buffer and command queue
#define WEB_CONSOLE_LINES 50
#define WEB_CONSOLE_LINE_LEN 240
char consoleBuf[WEB_CONSOLE_LINES][WEB_CONSOLE_LINE_LEN];
int consoleTail = 0;
int consoleFilled = 0;
int consoleTotal = 0; // total number of lines ever written, used for incremental fetching

#define WEB_CMD_QUEUE_SIZE 4
#define WEB_CMD_LEN 64
char consoleCmdQueue[WEB_CMD_QUEUE_SIZE][WEB_CMD_LEN];
int consoleCmdHead = 0;
int consoleCmdCount = 0;

// ---- Link state ----

bool xwWebRCAlive() {
	return webRCUpdated && millis() - webRCLastUpdate < WEB_RC_TIMEOUT_MS;
}

// The page was connected and the link is lost
bool xwWebRCLost() {
	return webRCUpdated && !xwWebRCAlive();
}

// Called before control(): web inputs override the other control sources while the page is connected
void xwApplyWebRC() {
	if (!xwWebRCAlive()) return;
	controlRoll = webRCRoll;
	controlPitch = webRCPitch;
	controlYaw = webRCYaw;
	controlThrottle = webRCThrottle;
	controlMode = NAN; // mode is selected using the page
	controlTime = t; // keep the RC loss failsafe quiet while the link is alive
}

// ---- Inputs processing ----

// Dead zone in normalized space [-1, 1], the rest of the range is rescaled to full
float webDeadzone(float norm, float deadzone) {
	if (fabsf(norm) < deadzone) return 0;
	return copysignf((fabsf(norm) - deadzone) / (1 - deadzone), norm);
}

void setWebRCInput(float roll, float pitch, float yaw, float throttle) {
	if (invalid(roll) || invalid(pitch) || invalid(yaw) || invalid(throttle)) return;

	float r = webDeadzone(constrain(roll, -WEB_RC_RAW_MAX, WEB_RC_RAW_MAX) / WEB_RC_RAW_MAX, WEB_RC_STICK_DEADZONE);
	float p = webDeadzone(constrain(pitch, -WEB_RC_RAW_MAX, WEB_RC_RAW_MAX) / WEB_RC_RAW_MAX, WEB_RC_STICK_DEADZONE);
	float y = webDeadzone(constrain(yaw, -WEB_RC_RAW_MAX, WEB_RC_RAW_MAX) / WEB_RC_RAW_MAX, WEB_RC_STICK_DEADZONE);
	float th = (constrain(throttle, -WEB_RC_RAW_MAX, WEB_RC_RAW_MAX) + WEB_RC_RAW_MAX) / (2 * WEB_RC_RAW_MAX); // [-100, 100] -> [0, 1]
	if (th < WEB_RC_THROTTLE_DEADZONE) th = 0;

	webRCRoll = constrain(r * WEB_RC_STICK_SCALE, -1, 1);
	webRCPitch = constrain(p * WEB_RC_STICK_SCALE, -1, 1);
	webRCYaw = constrain(y * WEB_RC_YAW_SCALE, -1, 1);
	webRCThrottle = constrain(th * WEB_RC_THROTTLE_SCALE, 0, 1);
}

// Minimal JSON helpers (the protocol is fixed: {"t":1,"th":..,"r":..,"p":..,"y":..})
const char *findJsonValue(const char *json, const char *key) {
	const char *p = strstr(json, key);
	if (!p) return nullptr;
	p = strchr(p, ':');
	if (!p) return nullptr;
	p++;
	while (*p == ' ' || *p == '"') p++;
	return p;
}

bool handleWebRCPacket(const String& body) {
	const char *json = body.c_str();
	const char *typePos = strstr(json, "\"t\":");
	if (!typePos) return false;
	int type = atoi(typePos + 4);
	const char *v;

	switch (type) {
		case 1: { // sticks
			float th = 0, r = 0, p = 0, y = 0;
			if ((v = findJsonValue(json, "\"th\""))) th = atof(v);
			if ((v = findJsonValue(json, "\"r\""))) r = atof(v);
			if ((v = findJsonValue(json, "\"p\""))) p = atof(v);
			if ((v = findJsonValue(json, "\"y\""))) y = atof(v);
			setWebRCInput(r, p, y, th);
			break;
		}
		case 2: { // button
			int idx = 0, state = 0;
			if ((v = findJsonValue(json, "\"b\""))) idx = atoi(v);
			if ((v = findJsonValue(json, "\"s\""))) state = atoi(v);
			if (idx >= 0 && idx < 16) {
				if (state) webRCButtons |= (1 << idx);
				else webRCButtons &= ~(1 << idx);
			}
			break;
		}
		case 4: // heartbeat
			break;
		default:
			return false;
	}
	webRCLastUpdate = millis();
	webRCUpdated = true;
	return true;
}

// Show a warning on the page
void xwWebWarn(const char *msg) {
	// quotes would break the JSON response
	size_t i = 0;
	for (; msg[i] && i < sizeof(webRCWarnMsg) - 1; i++) webRCWarnMsg[i] = (msg[i] == '"' || msg[i] == '\\') ? '\'' : msg[i];
	webRCWarnMsg[i] = '\0';
}

// Page buttons, called every loop. Buttons are handled on the rising edge.
void xwProcessWebButtons() {
	static uint16_t last = 0;
	uint16_t pressed = webRCButtons & ~last;
	last = webRCButtons;
	if (!pressed) return;

	if (pressed & (1 << WEB_BTN_ARM)) {
		if (controlThrottle < WEB_RC_ARM_THROTTLE_LIMIT) {
			armed = true; // the battery level is checked by the arming lockout
		} else {
			xwWebWarn("Throttle is too high, cannot arm");
		}
	}
	if (pressed & (1 << WEB_BTN_DISARM)) {
		armed = false;
	}
	if (pressed & (1 << WEB_BTN_ESTOP)) {
		armed = false;
		thrustTarget = 0;
		for (float& m : motors) m = 0;
	}
	if (pressed & (1 << WEB_BTN_STAB)) mode = STAB;
	if (pressed & (1 << WEB_BTN_ACRO)) mode = ACRO;
}

// ---- Debug console ----

// Called from print()
void xwWebLog(const char *msg) {
	if (!webConsoleEnabled) return;
	// split by lines, line breaks would break the JSON
	const char *start = msg;
	while (*start) {
		const char *end = strchr(start, '\n');
		int len = end ? (int)(end - start) : (int)strlen(start);
		if (len > 0) {
			int copy = min(len, WEB_CONSOLE_LINE_LEN - 1);
			memcpy(consoleBuf[consoleTail], start, copy);
			consoleBuf[consoleTail][copy] = '\0';
			consoleTail = (consoleTail + 1) % WEB_CONSOLE_LINES;
			if (consoleFilled < WEB_CONSOLE_LINES) consoleFilled++;
			consoleTotal++;
		}
		if (!end) break;
		start = end + 1;
	}
}

bool enqueueConsoleCmd(const char *cmd) {
	if (!cmd || !*cmd || consoleCmdCount >= WEB_CMD_QUEUE_SIZE) return false;
	int tail = (consoleCmdHead + consoleCmdCount) % WEB_CMD_QUEUE_SIZE;
	strncpy(consoleCmdQueue[tail], cmd, WEB_CMD_LEN - 1);
	consoleCmdQueue[tail][WEB_CMD_LEN - 1] = '\0';
	consoleCmdCount++;
	return true;
}

// Run console commands from the main loop, not from the HTTP handler
void xwProcessConsoleCommands() {
	if (consoleCmdCount <= 0) return;
	String cmd = consoleCmdQueue[consoleCmdHead];
	consoleCmdHead = (consoleCmdHead + 1) % WEB_CMD_QUEUE_SIZE;
	consoleCmdCount--;
	doCommand(cmd, false);
}

// ---- HTTP handlers ----

void handleWebRCRequest() {
	if (!webRCServer.hasArg("plain")) {
		webRCServer.send(400, "application/json", "{\"e\":\"no data\"}");
		return;
	}
	if (!handleWebRCPacket(webRCServer.arg("plain"))) {
		webRCServer.send(400, "application/json", "{\"e\":\"parse failed\"}");
		return;
	}
	// The page numbers flight modes as RAW=0, ACRO=1, STAB=2, ALTHOLD=3, AUTO=4
	int pageMode = mode == AUTO ? 4 : mode;
	char resp[160];
	if (webRCWarnMsg[0]) {
		snprintf(resp, sizeof(resp), "{\"s\":\"ok\",\"m\":%d,\"arm\":%d,\"warn\":\"%s\"}", pageMode, (int)armed, webRCWarnMsg);
		webRCWarnMsg[0] = '\0';
	} else {
		snprintf(resp, sizeof(resp), "{\"s\":\"ok\",\"m\":%d,\"arm\":%d}", pageMode, (int)armed);
	}
	webRCServer.send(200, "application/json", resp);
}

void handleWebConsole() {
	int since = webRCServer.hasArg("since") ? webRCServer.arg("since").toInt() : -1;
	int limit = webRCServer.hasArg("limit") ? webRCServer.arg("limit").toInt() : 20;
	if (limit <= 0) limit = 20;
	limit = min(limit, WEB_CONSOLE_LINES);

	int availableFrom = max(0, consoleTotal - consoleFilled);
	int sendFrom = since >= 0 ? since : availableFrom;
	sendFrom = constrain(sendFrom, availableFrom, consoleTotal);
	int sendTo = min(consoleTotal, sendFrom + limit);

	String json = "{\"total\":";
	json += consoleTotal;
	json += ",\"next\":";
	json += sendTo;
	json += ",\"has_more\":";
	json += sendTo < consoleTotal ? "true" : "false";
	json += ",\"lines\":[";
	for (int i = sendFrom; i < sendTo; i++) {
		String line = consoleBuf[i % WEB_CONSOLE_LINES];
		line.replace("\\", "\\\\");
		line.replace("\"", "\\\"");
		line.replace("\r", "\\r");
		if (i > sendFrom) json += ",";
		json += "\"" + line + "\"";
	}
	json += "]}";
	webRCServer.send(200, "application/json", json);
}

void handleWebConsoleCommand() {
	String cmd = webRCServer.arg("plain");
	cmd.trim();
	if (cmd.length() == 0) {
		webRCServer.send(400, "application/json", "{\"ok\":0,\"e\":\"empty command\"}");
		return;
	}
	xwWebLog(("> " + cmd).c_str());
	if (!enqueueConsoleCmd(cmd.c_str())) {
		xwWebLog("! command queue is full");
		webRCServer.send(503, "application/json", "{\"ok\":0,\"e\":\"queue full\"}");
		return;
	}
	webRCServer.send(200, "application/json", "{\"ok\":1,\"queued\":1}");
}

void handleWebRCStatus() {
	char json[160];
	snprintf(json, sizeof(json), "{\"enabled\":%s,\"active\":%s,\"voltage\":%.2f,\"armed\":%d,\"mode\":\"%s\"}",
		xwWebRCAlive() ? "true" : "false", xwWebRCAlive() ? "true" : "false",
		valid(voltage) ? voltage : 0.0f, (int)armed, getModeName());
	webRCServer.send(200, "application/json", json);
}

// ---- Setup and loop ----

// Use the XW-Drone default access point credentials unless they were configured by the `ap` command.
// Called before setupParameters() and setupWiFi().
void xwSeedWiFiCredentials() {
	Preferences prefs;
	prefs.begin("flix"); // the main storage namespace, see parameters.ino
	if (!prefs.isKey("WIFI_AP_SSID")) prefs.putString("WIFI_AP_SSID", XW_WIFI_AP_SSID);
	if (!prefs.isKey("WIFI_AP_PASS")) prefs.putString("WIFI_AP_PASS", XW_WIFI_AP_PASS);
	prefs.end();
}

void setupWebRC() {
	webRCServer.on("/", HTTP_GET, []() {
		webRCServer.send(200, "text/html", xwWebRCIndexHtml);
	});
	webRCServer.on("/web_rc", HTTP_POST, handleWebRCRequest);
	webRCServer.on("/web_rc/heartbeat", HTTP_POST, handleWebRCRequest);
	webRCServer.on("/web_rc/status", HTTP_GET, handleWebRCStatus);
	webRCServer.on("/console", HTTP_GET, handleWebConsole);
	webRCServer.on("/console/cmd", HTTP_POST, handleWebConsoleCommand);
	webRCServer.on("/console/enable", HTTP_POST, []() {
		webConsoleEnabled = true;
		webRCServer.send(200, "application/json", "{\"ok\":1}");
	});
	webRCServer.on("/console/disable", HTTP_POST, []() {
		webConsoleEnabled = false;
		webRCServer.send(200, "application/json", "{\"ok\":1}");
	});
	webRCServer.begin();
	print("Web RC: http://192.168.4.1:8080\n");
}

void loopWebRC() {
	webRCServer.handleClient();
	xwProcessWebButtons();
	xwProcessConsoleCommands();
}

#endif // XW_Z1
