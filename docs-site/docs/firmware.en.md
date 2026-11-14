# Firmware

## Flix and XW-Drone

The original **XW-Drone** firmware by 琛光 / ChenGuang is derived from [Flix](https://github.com/okalachev/flix) (Oleg Kalachev), see [License](license.md). This repository contains both:

- `firmware/source/XW-Drone/`: the vendor's original source and prebuilt image (unchanged).
- `firmware/flix/`: git subtree of current upstream Flix, extended with an `XW_Z1` board target and ported features. The [web flasher](flasher.md) binaries are built from it.

### What was ported?

| Feature | Description |
|---|---|
| Board `XW_Z1` | Pin map of XW_Z1 V2.0 (ESP32) plus C3 and S3 variants, see [Hardware](hardware.md) |
| Battery protection | Arming lockout and automatic landing on low battery voltage |
| Inverted-flight protection | Motors stop if the drone stays upside down |
| Throttle limits | Parameters `MOT_THR_MIN` / `MOT_THR_MAX` |
| Status LED | Slow blink = armed, fast = warning (low battery, link lost) |
| Web remote control | Control from the smartphone browser over the drone's Wi-Fi, see [Remote control](remote-control.md) |

Compiled with `-DXW_Z1`; none of this has been tested on real hardware yet. Already in upstream Flix and therefore not duplicated: battery voltage measurement (`power.ino`), RC-loss descend and the tilt failsafe at 120° in STAB mode (`SF_DISARM_TILT`). Added by the XW port (`firmware/flix/flix/xw_*`):

- **Battery**: `PWR_VOLT_WARN` 3.5 V (arming lockout, and an idle armed drone is disarmed after 0.9 s) and `PWR_VOLT_CRIT` 3.0 V (automatic descent in flight, latched until disarm). Changed upstream defaults on XW_Z1: `PWR_VOLT_SCALE` 1.303 (43/33 divider), `PWR_VOLT_LPF_A` 0.2, `PWR_VOLT_LOW` 3.3 V, plus the pin parameters (`PWR_VOLT_PIN`, `RC_RX_PIN`, `IMU_MODEL`, `IMU_PIN_*`, `MOT_PIN_*`, `IMU_ROT_YAW`). Readings below 0.5 V mean USB power and are ignored.
- **Inverted protection**: in any mode; tilt above 134° for 1.5 s stops the motors.
- **Throttle limits**: `MOT_THR_MIN` 0.10 (thrust at the bottom of the throttle range / idle; the effective minimum is clamped to 0.1) and `MOT_THR_MAX` 1.0.
- **Status LED**: off when disarmed (fast blink if the battery is low), 1 Hz slow blink when armed, 8 Hz fast blink on any warning (low battery, inverted, RC lost, web link lost); on during startup.
- **Wi-Fi**: on XW_Z1 the access point is `XW_Drone_WiFi` / `12345678` (only set if `WIFI_AP_SSID` / `WIFI_AP_PASS` are not stored yet; `ap <ssid> <password>` changes them, `preset` resets). The stock `flix-esp32` build keeps `flix` / `flixwifi`.
- **Web RC**: see [Remote control](remote-control.md).

!!! warning "Flash usage"
    The ESP32-C3 build uses 98% of the app partition, so there is little headroom for additions.

## Targets

| Target | Chip | Description | Build flag |
|---|---|---|---|
| `xw-z1` | ESP32 | Kit board XW_Z1 V2.0 | `-DXW_Z1` |
| `xw-z1-c3` | ESP32-C3 | Variant with ESP32-C3 | `-DXW_Z1` |
| `xw-z1-s3` | ESP32-S3 | Variant with ESP32-S3 | `-DXW_Z1` |
| `flix-esp32` | ESP32 | Unmodified Flix configuration (reference) | – |

The binaries are built by GitHub Actions on every push and published via the [web flasher](flasher.md).

## Build locally

### With `firmware/build.sh` (arduino-cli)

```bash
git clone https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation.git
cd ESP32-Mini-Drone-Kit-Documentation
firmware/build.sh xw-z1        # or: xw-z1-c3, xw-z1-s3, flix-esp32, all
```

Output in `firmware/build/<target>/`: `flix.ino.merged.bin` (address 0x0), `flix.ino.bin`, `flix.ino.bootloader.bin`, `flix.ino.partitions.bin`. The script installs the core and libraries via arduino-cli.

### Arduino IDE

1. Install Arduino IDE 2.x (1.8.x is not supported).
2. Install the ESP32 core **esp32 by Espressif Systems, version 3.3.10**. Other versions cause compile errors.
3. Install the libraries `FlixPeriph` (latest) and `MAVLink` **2.0.33**.
4. Open `firmware/flix/flix/flix.ino`. For the kit target define the build flag `-DXW_Z1` (see `firmware/build.sh`).
5. Select "ESP32 Dev Module" or "WEMOS D1 MINI ESP32"; for C3/S3 set "USB CDC on Boot: Enabled". Set Core Debug Level to "Error".

## Upstream sync

`firmware/flix/` is a git subtree of [okalachev/flix](https://github.com/okalachev/flix). A workflow checks upstream regularly and prepares updates as a pull request; the XW_Z1 changes are tested against the new state (all targets are built in CI). Details: `firmware/README.md`.

The workflow `.github/workflows/upstream-sync.yml` runs weekly (Monday 04:17 UTC, or manually). It executes `git subtree pull --prefix=firmware/flix https://github.com/okalachev/flix master --squash`, builds all targets and opens a pull request on success, or an issue (label `upstream-sync`) on a merge conflict or build failure. Upstream files carry only small `#ifdef XW_Z1` hooks (`config.h`, `flix.ino`, `parameters.ino`, `console.ino`); the XW code lives in `firmware/flix/flix/xw_*`.

## Parameters

Flix is configured via parameters (QGroundControl: *Vehicle Setup → Parameters*, or console: `p`). Selection from the Flix docs:

| Parameter | Meaning |
|---|---|
| `MOT_PIN_FL/FR/RL/RR` | Motor pins |
| `MOT_PWM_FREQ`, `MOT_PWM_STOP/MIN/MAX` | PWM settings |
| `IMU_MODEL`, `IMU_BUS`, `IMU_PIN_*`, `IMU_ROT_*` | IMU type, bus, pins and orientation |
| `PWR_VOLT_PIN`, `PWR_VOLT_SCALE` | Battery voltage measurement |
| `RC_RX_PIN` | SBUS input (negative = off) |
| `CTL_ATT_MAX` | Maximum tilt angle (reduce for smartphone control) |
| `WIFI_MODE` | Wi-Fi mode (AP/client) |
| `MOT_THR_MIN`, `MOT_THR_MAX` | Throttle limits: thrust at the bottom of the throttle range / idle (default 0.10, effective minimum at least 0.1) and at the top (default 1.0) |
| `PWR_VOLT_WARN`, `PWR_VOLT_CRIT`, `PWR_VOLT_LOW` | Battery thresholds: warning with arming lockout (3.5 V), automatic descent (3.0 V), in-flight warning (3.3 V) |
| `WIFI_AP_SSID`, `WIFI_AP_PASS` | Access point credentials (also via console: `ap <ssid> <password>`) |

Full list and details: [Flix documentation](https://github.com/okalachev/flix/blob/master/docs/usage.md) or `firmware/flix/docs/` in the repository.
