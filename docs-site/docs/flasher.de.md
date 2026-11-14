# Web-Flasher

!!! warning "Inoffizielle Firmware, auf eigene Gefahr"
    Diese Binaries sind **inoffiziell** und werden ohne Gewährleistung für Besitzer des Kits bereitgestellt. Flix hat keine Lizenz (alle Rechte bei Oleg Kalachev), siehe [Lizenz](license.md). **Propeller abnehmen**, bevor du flashst oder testest. Das Flashen überschreibt die vorhandene Firmware; die Original-Firmware kannst du über `firmware/binaries/XW-Drone.ino.merged.bin` wiederherstellen.

## Firmware installieren

<div id="flasher"><noscript>Der Flasher benötigt JavaScript.</noscript></div>

Wähle dein Board und klicke auf „Firmware installieren“. Im Dialog den COM-Port der Drohne auswählen. Die Option „Erase device“ (Standard) löscht den Flash vor der Installation, dabei gehen gespeicherte Parameter und Kalibrierungen verloren.

| Auswahl | Wann? |
|---|---|
| **XW-Z1 (ESP32)** | Standard-Kitplatine XW_Z1 V2.0 |
| **XW-Z1 C3** | Platinenvariante mit ESP32-C3 |
| **XW-Z1 S3** | Platinenvariante mit ESP32-S3 |
| **Flix stock (ESP32)** | Unveränderte Flix-Firmware für ESP32-Boards mit Flix-Pinbelegung; **nicht** für die Kit-Platine gedacht (andere Pins) |

## Voraussetzungen

- **Chrome oder Edge am Desktop** (Windows, macOS, Linux, ChromeOS). Firefox, Safari und Smartphone-Browser unterstützen Web Serial nicht.
- **CH340-Treiber** unter Windows installiert ([Anleitung und Download-Link](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). Die Platine muss als COM-Port erscheinen.
- **USB-Datenkabel** (nicht nur Ladekabel).
- Seite über HTTPS geöffnet (GitHub Pages: ja).
- **Propeller abgenommen**, Akku nicht nötig (USB reicht zum Flashen).

Erscheint beim Verbinden ein Fehler, halte die **BOOT**-Taste gedrückt, tippe kurz auf **EN/RST** (oder stecke USB neu an) und lasse BOOT los, sobald der Flash-Vorgang startet. Meist erledigt die CH340-Schaltung das automatisch.

## Wenn der Browser nicht unterstützt wird

Dann zeigt der Button eine Warnung. Alternativen:

1. **Espressif Flash Download Tool** (Windows): Chip `ESP32`, Modus *Develop / SPI Download*, die Datei `flix.ino.merged.bin` (Download oben unter „Direkt-Downloads“) wählen, Adresse `0x0000`, richtigen COM-Port wählen, *START*. Download und Anleitung: [Flash Download Tool](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/tools/flash-download-tool/README.md).
2. **esptool** (Python):

   ```bash
   pip install esptool
   esptool.py --chip esp32 write_flash 0x0 flix.ino.merged.bin
   ```

   Für C3/S3 `--chip esp32c3` bzw. `--chip esp32s3` verwenden. Bei Bedarf `--port COM5` bzw. `--port /dev/ttyUSB0` ergänzen.

## Nach dem Flashen

Drohne mit Akku einschalten, Kalibrierung durchführen (`ca`) und per [Fernsteuerung](remote-control.md) verbinden. Bei Problemen: [Fehlerbehebung](troubleshooting.md).
