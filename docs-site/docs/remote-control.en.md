# Remote control

!!! danger "Safety"
    Remove the propellers for configuration and tests. Fly outdoors or in large rooms, away from people. Never run the motors from USB power.

## Web RC (smartphone browser)

The drone opens its own Wi-Fi network (access point). Your phone connects to it and opens the control page in the browser. No app is needed.

1. Power the drone from the battery.
2. Connect the phone to the drone's Wi-Fi: SSID `XW_Drone_WiFi`, password `12345678` (XW_Z1 default; changeable with the console command `ap <ssid> <password>`, `preset` resets it).
3. Open `http://192.168.4.1:8080` in the browser.
4. **Debug** → enter `ca` and perform the accelerometer calibration (once, then as needed).
5. **Arm** to arm (refused while the throttle is above 0.05); left stick: throttle and yaw, right stick: pitch and roll. To land, lower the throttle slowly, then **Disarm**.

Safety features (ported from XW-Drone): on low battery arming is blocked and a flying drone lands automatically; sustained inverted flight stops the motors. The status LED blinks slowly (armed) or quickly (warning). Throttle limits via `MOT_THR_MIN` / `MOT_THR_MAX`; see [Firmware](firmware.md).

The page sends the stick positions at 20 Hz and a heartbeat every 2 s. Buttons:

- **Arm** / **Disarm**
- **E-stop**: emergency stop
- **Mode**: toggles STAB / ACRO (no ALTHOLD)
- **Debug console**: shows `print()` output and runs console commands

While the page is connected, it overrides SBUS and MAVLink manual control. If no packets arrive for 8 s, the RC-loss failsafe makes the drone descend.

!!! warning "Caveats"
    Long console commands (`ca`, `cr` calibration) block the web server until they finish. The XW_Z1 port has not been tested on real hardware yet.

## QGroundControl / MAVLink

Flix speaks MAVLink over Wi-Fi (UDP). This works with [QGroundControl](https://docs.qgroundcontrol.com/master/en/qgc-user-guide/getting_started/download_and_install.html) (desktop and Android) and the [Mavlink Joystick app](https://github.com/goldarte/mavlink-joystick/releases/latest).

1. Power the drone and connect to its Wi-Fi (XW_Z1 build: SSID `XW_Drone_WiFi`, password `12345678`; stock `flix-esp32` build: `flix` / `flixwifi`). QGroundControl/MAVLink work over the same access point as in upstream Flix.
2. Start QGroundControl. It connects automatically and shows telemetry. If it does not, disable firewall/VPN on your computer.
3. For joystick control on a phone: enable *Virtual Joystick* in settings and **disable** *Auto-Center Throttle*. Reduce `CTL_ATT_MAX` for gentler controls.
4. Console: *Analyze Tools → MAVLink Console*. Useful commands: `help`, `imu`, `ca` (calibration), `p` (parameters), `rc`, `mfl`/`mfr`/`mrl`/`mrr` (motor tests, **without propellers**).
5. Parameters: *Vehicle Setup → Parameters*.

Other options: SBUS radio (set `RC_RX_PIN`, calibrate with `cr`) or a USB joystick via QGroundControl. Flight modes: STAB (default, attitude-stabilized), ACRO (rates), RAW (tests only), AUTO. Arm: left stick bottom right, disarm: bottom left.

Details: [Flix usage](https://github.com/okalachev/flix/blob/master/docs/usage.md) (in the repository: `firmware/flix/docs/usage.md`).
