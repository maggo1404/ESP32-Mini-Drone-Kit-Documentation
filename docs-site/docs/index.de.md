# ESP32 Mini Drone Kit

Dokumentation, Firmware und Web-Flasher für das **ESP32 Mini Drone Kit** (Platine `XW_Z1 V2.0`, ursprüngliche Firmware `XW-Drone` von 琛光 / ChenGuang). Die Original-Unterlagen des Kits sind überwiegend chinesisch; diese Seite fasst sie auf Deutsch und Englisch zusammen.

!!! danger "Sicherheit"
    Beim Flashen, Konfigurieren und bei Motortests **immer die Propeller abnehmen**. Drohnen sind gefährlich; Betrieb auf eigene Gefahr. Siehe [Lizenz & Haftung](license.md).

## Was ist das Kit?

Ein kleiner Quadrocopter mit Hohlbechermotoren, MPU-6500/9250-Lagesensor und einer ESP32-Platine. Die Drohne spannt ein eigenes WLAN auf und wird per Smartphone-Browser (Web-RC) oder per MAVLink (QGroundControl) gesteuert. Dieses Repository enthält zusätzlich eine neu gebaute Firmware auf Basis von [Flix](https://github.com/okalachev/flix) mit Unterstützung der Kit-Platine.

## Schnellstart

<div class="grid cards" markdown>

-   **[Jetzt flashen](flasher.md)**

    Firmware direkt im Browser auf die Drohne spielen (Chrome/Edge, USB-Kabel).

-   **[Zusammenbau](assembly.md)**

    Motoren, Propeller, Platine und erste Inbetriebnahme.

-   **[Web-Fernsteuerung](remote-control.md)**

    Mit dem Smartphone über das WLAN der Drohne fliegen.

-   **[Fehlerbehebung](troubleshooting.md)**

    Kein COM-Port, Drohne hebt nicht ab, Akku leer?

</div>

## Inhalt

| Seite | Inhalt |
|---|---|
| [Hardware](hardware.md) | Platine XW_Z1 V2.0, Pinbelegung (ESP32/C3/S3), Schaltplan, 3D-Schutzhülle |
| [Zusammenbau & Bedienung](assembly.md) | Übersetzte Kit-Anleitung |
| [Firmware](firmware.md) | Flix vs. XW-Drone, Build-Targets, lokaler Build, Parameter |
| [Web-Flasher](flasher.md) | Firmware im Browser flashen, Alternativen |
| [Fernsteuerung](remote-control.md) | Web-RC, QGroundControl, MAVLink |
| [Fehlerbehebung](troubleshooting.md) | Häufige Probleme |
| [Lizenz](license.md) | Lizenzlage, inoffizielle Veröffentlichung, Haftungsausschluss |

Quellen und alle Original-Dateien: [GitHub-Repository](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation).
