# Remote control

!!! danger "Safety"
    Remove the propellers for configuration and tests. Fly outdoors or in large rooms, away from people. Never run the motors from USB power.

## Web RC (smartphone browser)

The drone opens its own Wi-Fi network (access point). Your phone connects to it and opens the control page in the browser. No app is needed.

1. Power the drone from the battery.
2. Connect the phone to the drone's Wi-Fi: SSID `TODO(verify)`, password `TODO(verify)`. *(Original firmware: `XW_Drone_WiFi` / `12345678`.)*
3. Open `http://TODO(verify)` in the browser. *(Original firmware: `192.168.4.1:8080`.)*
4. **Debug** → enter `ca` and perform the accelerometer calibration (once, then as needed).
5. **Unlock** to arm; left stick: throttle and yaw, right stick: pitch and roll. To land, lower the throttle slowly, then **Lock**.

Safety features (ported from XW-Drone): on low battery arming is blocked and a flying drone lands automatically; sustained inverted flight stops the motors. The status LED blinks slowly (armed) or quickly (warning). Throttle limits via `MOT_THR_MIN` / `MOT_THR_MAX`.

!!! note "TODO(verify)"
    Fill in SSID, password, URL/port and parameter names of the web RC in the ported Flix firmware and check them against `firmware/flix/flix/`. Also whether web RC and MAVLink run at the same time.

## QGroundControl / MAVLink

Flix speaks MAVLink over Wi-Fi (UDP). This works with [QGroundControl](https://docs.qgroundcontrol.com/master/en/qgc-user-guide/getting_started/download_and_install.html) (desktop and Android) and the [Mavlink Joystick app](https://github.com/goldarte/mavlink-joystick/releases/latest).

1. Power the drone and connect to its Wi-Fi (Flix default: SSID `flix`, password `flixwifi`; *TODO(verify): may differ for the XW_Z1 target*).
2. Start QGroundControl. It connects automatically and shows telemetry. If it does not, disable firewall/VPN on your computer.
3. For joystick control on a phone: enable *Virtual Joystick* in settings and **disable** *Auto-Center Throttle*. Reduce `CTL_ATT_MAX` for gentler controls.
4. Console: *Analyze Tools → MAVLink Console*. Useful commands: `help`, `imu`, `ca` (calibration), `p` (parameters), `rc`, `mfl`/`mfr`/`mrl`/`mrr` (motor tests, **without propellers**).
5. Parameters: *Vehicle Setup → Parameters*.

Other options: SBUS radio (set `RC_RX_PIN`, calibrate with `cr`) or a USB joystick via QGroundControl. Flight modes: STAB (default, attitude-stabilized), ACRO (rates), RAW (tests only), AUTO. Arm: left stick bottom right, disarm: bottom left.

Details: [Flix usage](https://github.com/okalachev/flix/blob/master/docs/usage.md) (in the repository: `firmware/flix/docs/usage.md`).
