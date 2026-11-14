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

Kompiliert mit `-DXW_Z1`; nichts davon wurde bisher an echter Hardware getestet. Bereits im Original-Flix vorhanden und daher nicht dupliziert: Akkuspannungsmessung (`power.ino`), Absinken bei RC-Verlust und das Neigungs-Failsafe bei 120° im STAB-Modus (`SF_DISARM_TILT`). Vom XW-Port ergänzt (`firmware/flix/flix/xw_*`):

- **Akku**: `PWR_VOLT_WARN` 3,5 V (Arm-Sperre; eine scharfe Drohne im Leerlauf wird nach 0,9 s entschärft) und `PWR_VOLT_CRIT` 3,0 V (automatisches Absinken im Flug, bis zum Disarm gespeichert). Geänderte Upstream-Standardwerte beim XW_Z1: `PWR_VOLT_SCALE` 1,303 (Teiler 43/33), `PWR_VOLT_LPF_A` 0,2, `PWR_VOLT_LOW` 3,3 V sowie die Pin-Parameter (`PWR_VOLT_PIN`, `RC_RX_PIN`, `IMU_MODEL`, `IMU_PIN_*`, `MOT_PIN_*`, `IMU_ROT_YAW`). Messwerte unter 0,5 V bedeuten USB-Versorgung und werden ignoriert.
- **Umkehrflug-Schutz**: in jedem Modus; Neigung über 134° für 1,5 s stoppt die Motoren.
- **Schubgrenzen**: `MOT_THR_MIN` 0,10 (Schub am unteren Ende des Gasbereichs / Leerlauf; das effektive Minimum ist auf mindestens 0,1 begrenzt) und `MOT_THR_MAX` 1,0.
- **Status-LED**: bei Disarm aus (schnelles Blinken bei niedrigem Akku), im Armed-Zustand langsames Blinken mit 1 Hz, bei jeder Warnung schnelles Blinken mit 8 Hz (Akku niedrig, Umkehrlage, RC verloren, Web-Verbindung verloren); beim Start an.
- **WLAN**: Beim XW_Z1 ist der Access Point `XW_Drone_WiFi` / `12345678` (nur gesetzt, wenn `WIFI_AP_SSID` / `WIFI_AP_PASS` noch nicht gespeichert sind; `ap <ssid> <passwort>` ändert sie, `preset` setzt zurück). Der Standard-Build `flix-esp32` behält `flix` / `flixwifi`.
- **Web-RC**: siehe [Fernsteuerung](remote-control.md).

!!! warning "Flash-Belegung"
    Der ESP32-C3-Build belegt 98 % der App-Partition, es bleibt kaum Spielraum für Erweiterungen.

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

Der Workflow `.github/workflows/upstream-sync.yml` läuft wöchentlich (montags 04:17 UTC, oder manuell). Er führt `git subtree pull --prefix=firmware/flix https://github.com/okalachev/flix master --squash` aus, baut alle Targets und öffnet bei Erfolg einen Pull Request, bei Merge-Konflikt oder Build-Fehler ein Issue (Label `upstream-sync`). Upstream-Dateien enthalten nur kleine `#ifdef XW_Z1`-Hooks (`config.h`, `flix.ino`, `parameters.ino`, `console.ino`); der XW-Code liegt in `firmware/flix/flix/xw_*`.

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
| `MOT_THR_MIN`, `MOT_THR_MAX` | Schubgrenzen: Schub am unteren Ende des Gasbereichs / Leerlauf (Standard 0,10, effektives Minimum mindestens 0,1) und am oberen Ende (Standard 1,0) |
| `PWR_VOLT_WARN`, `PWR_VOLT_CRIT`, `PWR_VOLT_LOW` | Akku-Schwellen: Warnung mit Arm-Sperre (3,5 V), automatisches Absinken (3,0 V), Warnung im Flug (3,3 V) |
| `WIFI_AP_SSID`, `WIFI_AP_PASS` | Zugangsdaten des Access Points (auch per Konsole: `ap <ssid> <passwort>`) |

Vollständige Liste und Details: [Flix-Dokumentation](https://github.com/okalachev/flix/blob/master/docs/usage.md) bzw. `firmware/flix/docs/` im Repository.
