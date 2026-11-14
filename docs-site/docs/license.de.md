# Lizenz & Haftung

Dies ist eine technische Bestandsaufnahme, **keine Rechtsberatung**. Die ausführliche Analyse steht in [Lizenz.md](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/Lizenz.md) im Repository.

!!! danger "Flix hat keine Lizenz"
    Das Flix-Repository von **Oleg Kalachev** enthält keine Lizenzdatei, auch nicht in der Historie. Damit gilt: **alle Rechte vorbehalten**. Nur Oleg Kalachev kann Lizenzen für seinen Code vergeben. Die Firmware auf dieser Seite ist von Flix abgeleitet bzw. enthält Flix-Code.

## Überblick

| Komponente | Lage |
|---|---|
| **Flix** (`firmware/flix/`) | Keine Lizenz, alle Rechte bei Oleg Kalachev. Quelle: [okalachev/flix](https://github.com/okalachev/flix) |
| **XW-Drone** (`firmware/source/XW-Drone/`) | GPLv3 + „Commons Clause“ (Verkaufsverbot), Licensor 琛光 / ChenGuang. Der Code ist eindeutig von Flix abgeleitet, die Copyright-Zeilen wurden entfernt; die Lizenz ist für übernommene Flix-Teile sehr wahrscheinlich nicht wirksam und rechtlich widersprüchlich. |
| Schaltplan, STL, Anleitung, Video | Keine Lizenzangabe. Die englische Anleitung ist eine inoffizielle Übersetzung. |
| Espressif Flash Download Tool, CH340-Treiber | Proprietär; **nicht** im Repository, nur Download-Links |
| Bibliotheken (Arduino-ESP32, MAVLink, FlixPeriph) | Eigene Lizenzen; im Binary kompiliert enthalten |

## Inoffizielle Veröffentlichung der Firmware

Die hier bereitgestellten Binaries werden **inoffiziell und ausschließlich als Hilfe für Besitzer des Kits** veröffentlicht, damit diese ihre Drohne flashen können. Es gibt keine Zustimmung des Flix-Autors oder des Kit-Herstellers. Gewerbliche Nutzung, Verkauf und Weitergabe sind nicht gedacht; die Commons Clause von ChenGuang schließt den Verkauf ausdrücklich aus. Rechteinhaber, die eine Entfernung wünschen, können sich über die [GitHub-Issues](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/issues) melden.

## Haftungsausschluss und Sicherheit

- Alles wird **ohne Gewährleistung** bereitgestellt, Nutzung **auf eigene Gefahr**. Es wird keine Haftung für Schäden an Geräten, Sachen oder Personen übernommen.
- Drohnen können verletzen. **Propeller beim Flashen, Konfigurieren und bei Motortests immer abnehmen.** Nicht über Personen oder Tiere fliegen, Akku-Sicherheit beachten, lokale Vorschriften zum Betrieb von Drohnen einhalten.
- Fehlerhafte Firmware kann die Drohne unkontrollierbar machen. Erste Flüge nur niedrig, im Freien und ohne Personen in der Nähe.
