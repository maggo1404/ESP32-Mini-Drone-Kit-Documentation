# Firmware

## Flix und XW-Drone

Die Original-Firmware **XW-Drone** von 琛光 / ChenGuang ist eine Ableitung von [Flix](https://github.com/okalachev/flix) (Oleg Kalachev), siehe [Lizenz](license.md). Dieses Repository enthält beides:

- `firmware/source/XW-Drone/`: Original-Quelltext und fertiges Abbild des Herstellers (unverändert).
- `firmware/flix/`: Git-Subtree des aktuellen Flix-Upstreams, ergänzt um ein `XW_Z1`-Board-Target und portierte Funktionen. Daraus entstehen die Binaries des [Web-Flashers](flasher.md).

### Was wurde portiert?

| Funktion | Beschreibung |
|---|---|
| Board `XW_Z1` | Pinbelegung von XW_Z1 V2.0 (ESP32) sowie C3- und S3-Variante, siehe [Hardware](hardware.md) |
| Akku-Schutz | Bei niedriger Akkuspannung Arm-Sperre und automatische Landung |
| Umkehrflug-Schutz | Motoren stoppen, wenn die Drohne dauerhaft auf dem Kopf steht |
| Schubgrenzen | Parameter `MOT_THR_MIN` / `MOT_THR_MAX` |
| Status-LED | Langsam blinkend = armed, schnell = Warnung (Akku leer, Verbindung verloren) |
| Web-Fernsteuerung | Steuerung im Smartphone-Browser über das WLAN der Drohne, siehe [Fernsteuerung](remote-control.md) |

!!! note "TODO(verify)"
    Genaue Parameternamen, Standardwerte und Schwellen der portierten Funktionen eintragen (Quelle: `firmware/flix/flix/*`, insbesondere `config.h`, `safety.ino`, `power.ino`, `led.ino`, `parameters.ino`).

## Targets

| Target | Chip | Beschreibung | Build-Flag |
|---|---|---|---|
| `xw-z1` | ESP32 | Kit-Platine XW_Z1 V2.0 | `-DXW_Z1` |
| `xw-z1-c3` | ESP32-C3 | Variante mit ESP32-C3 | `-DXW_Z1` |
| `xw-z1-s3` | ESP32-S3 | Variante mit ESP32-S3 | `-DXW_Z1` |
| `flix-esp32` | ESP32 | Unveränderte Flix-Konfiguration (Referenz) | – |

Die Binaries werden bei jedem Push von GitHub Actions gebaut und im [Web-Flasher](flasher.md) bereitgestellt.

## Lokal bauen

### Mit `firmware/build.sh` (arduino-cli)

```bash
git clone https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation.git
cd ESP32-Mini-Drone-Kit-Documentation
firmware/build.sh xw-z1        # oder: xw-z1-c3, xw-z1-s3, flix-esp32, all
```

Ergebnis in `firmware/build/<target>/`: `flix.ino.merged.bin` (Adresse 0x0), `flix.ino.bin`, `flix.ino.bootloader.bin`, `flix.ino.partitions.bin`. Das Skript installiert Core und Bibliotheken über arduino-cli.

### Arduino IDE

1. Arduino IDE 2.x installieren (1.8.x wird nicht unterstützt).
2. ESP32-Core **esp32 by Espressif Systems, Version 3.3.10** installieren. Andere Versionen führen zu Kompilierfehlern.
3. Bibliotheken installieren: `FlixPeriph` (neueste Version) und `MAVLink` **2.0.33**.
4. `firmware/flix/flix/flix.ino` öffnen. Für das Kit-Target das Build-Flag `-DXW_Z1` setzen (siehe `firmware/build.sh`).
5. Board „ESP32 Dev Module“ bzw. „WEMOS D1 MINI ESP32“ wählen; bei C3/S3 „USB CDC on Boot: Enabled“. Core Debug Level auf „Error“.

## Upstream-Sync

`firmware/flix/` ist ein Git-Subtree von [okalachev/flix](https://github.com/okalachev/flix). Ein Workflow prüft den Upstream regelmäßig und bereitet Aktualisierungen als Pull Request vor; die XW_Z1-Anpassungen werden dabei gegen den neuen Stand getestet (Build aller Targets in der CI). Details: `firmware/README.md`.

!!! note "TODO(verify)"
    Genauen Ablauf des Upstream-Syncs (Workflow `upstream-sync.yml`, PR-Prozess) mit `firmware/README.md` abgleichen.

## Parameter

Flix wird über Parameter konfiguriert (QGroundControl: *Vehicle Setup → Parameters*, oder Konsole: `p`). Auswahl der Parameter aus der Flix-Doku:

| Parameter | Bedeutung |
|---|---|
| `MOT_PIN_FL/FR/RL/RR` | Motor-Pins |
| `MOT_PWM_FREQ`, `MOT_PWM_STOP/MIN/MAX` | PWM-Einstellungen |
| `IMU_MODEL`, `IMU_BUS`, `IMU_PIN_*`, `IMU_ROT_*` | IMU-Typ, Bus, Pins und Einbaulage |
| `PWR_VOLT_PIN`, `PWR_VOLT_SCALE` | Akkuspannungsmessung |
| `RC_RX_PIN` | SBUS-Eingang (negativ = aus) |
| `CTL_ATT_MAX` | Maximaler Neigungswinkel (für Smartphone-Steuerung verringern) |
| `WIFI_MODE` | WLAN-Modus (AP/Client) |
| `MOT_THR_MIN`, `MOT_THR_MAX` | Schubgrenzen *(TODO(verify): Name, Bereich, Standard)* |
| Akku-Schwellen (Warnung/Auto-Landung) | *TODO(verify): Parameternamen und Standardwerte* |
| Web-RC (SSID, Passwort) | *TODO(verify): Parameternamen* |

Vollständige Liste und Details: [Flix-Dokumentation](https://github.com/okalachev/flix/blob/master/docs/usage.md) bzw. `firmware/flix/docs/` im Repository.
