# Lizenz- und Urheberrechtshinweise

Stand: Auswertung des Archivs `ESP32迷你无人机套件资料.rar`. Das Original-Archiv und die entpackten Dateien (`_original/extracted/ESP32迷你无人机套件资料/`) werden nur lokal aufbewahrt und sind nicht Teil des Repositorys (`_original/` ist in `.gitignore`).
Gesucht wurde in allen Text- und Quelldateien sowie in den PDF-Dokumenten und in den Zeichenketten der Windows-Programme (nur lesend, nichts wurde ausgeführt). Alle Pfade sind relativ zum Repository-Root angegeben. Die Kopien in der neuen Struktur (`firmware/`, `docs/`, ...) sind inhaltsgleich mit den Dateien im lokalen Ordner `_original/extracted/`. Die Original-Pfade dort dienen nur als Referenz.

Dies ist eine technische Bestandsaufnahme, keine Rechtsberatung.

## 1. Gefundene Lizenzangaben

### 1.1 Firmware-Quellcode XW-Drone

- Datei: `firmware/source/XW-Drone/LICENSE` (Original: `_original/extracted/ESP32迷你无人机套件资料/source code/XW-Drone-main/XW-Drone/LICENSE`)
- Lizenz: **GNU General Public License v3 (29. Juni 2007) in Kombination mit "Commons Clause License Condition v1.0"**
- Rechteinhaber: Licensor "琛光 / ChenGuang" (eingetragene Marke "琛光"). Das GPL-Textteil nennt nur den allgemeinen FSF-Copyright-Hinweis ("Copyright (C) 2007 Free Software Foundation, Inc.").
- Zitat: "The Software is provided to you by the Licensor (琛光 / ChenGuang) under the License, as defined below, subject to the following condition. Without limiting other conditions in the License, the grant of rights under the License will not include, and the License does not grant to you, the right to Sell the Software."
- Zitat: "'Sell' means practicing any or all of the rights granted to you under the License in exchange for fee or other consideration ... or producing hardware products for sale that incorporate or are derived from the Software, without prior written authorization from the Licensor."
- Zitat: "Commercial use authorization: contact the trademark holder ("琛光" registered trademark) for a commercial license."
- Hinweise:
  - Die Commons Clause schliesst den Verkauf aus; damit ist die Software nicht im Sinne der Open-Source-Definition frei, und die Kombination mit GPLv3 (die zusaetzliche Einschraenkungen grundsaetzlich nicht erlaubt, Abschnitt 7/10) ist rechtlich widerspruechlich bzw. auslegungsbeduerftig. Vor einer Weitergabe oder gewerblichen Nutzung sollte der Rechteinhaber kontaktiert werden.
  - Einzelne Quelldateien (`*.ino`, `*.h`) enthalten keine Lizenz- oder Copyright-Kopfzeilen (siehe Abschnitt 3).
  - Die zugehoerige `firmware/source/XW-Drone/README.MD` (chinesisch) verweist auf das Projekt auf OSHWHub: "嘉立创开源平台工程链接：https://oshwhub.com/songge8/project_qqqyfdkm". Die dortigen Lizenzbedingungen (u. a. fuer die Hardware) wurden nicht geprueft (kein Netzzugriff auf diese Seite).
  - `firmware/source/XW-Drone/web_rc_html.h` enthaelt einen Footer-Link "琛光无人机开源项目" (Zeilen ca. 140 und 197, Kommentar "版权页脚" = "Copyright-Fusszeile") ohne weitere Lizenzaussage.

### 1.2 Espressif Flash Download Tool, Dokumentation

- Dateien: `docs/tool-manuals/Flash_Download_Tool__en.pdf`, `docs/tool-manuals/Flash_Download_Tool__cn.pdf`
- Lizenz: proprietaer, alle Rechte vorbehalten
- Rechteinhaber: Espressif Systems (乐鑫信息科技 / 乐鑫)
- Zitat (en): "Copyright © 2023 Espressif Inc. All rights reserved. All rights are reserved." sowie "No licenses express or implied, by estoppel or otherwise, to any intellectual property rights are granted by this document."
- Zitat (cn): "版权归 © 2023 乐鑫所有。保留所有权利。" / "版权所有 © 2023"

### 1.3 Espressif Flash Download Tool 3.9.5 (Programm)

- Dateien: Die ausfuehrbare Datei `flash_download_tool_3.9.5.exe` wird **nicht im Repository weitergegeben** (nur lokal in `_original/`); im Repository liegen lediglich die kleinen Konfigurationsdateien `software/tools/flash-download-tool/configure/` und `combine/` sowie ein Hinweis mit Download-Link (`software/tools/flash-download-tool/README.md`, offizielle Seite: https://www.espressif.com/en/support/download/other-tools); Release-Notes: `docs/tool-manuals/release_note.txt` ("3.9.5：* support esp32-h2 ...")
- Lizenz: keine Lizenzdatei im Paket; das PDF-Handbuch (1.2) stellt klar, dass keine Lizenzen eingeraeumt werden. Das Programm ist aller Wahrscheinlichkeit nach Espressifs proprietaeres Freeware-Werkzeug (Weitergabebedingungen nicht belegt).
- Rechteinhaber: Espressif Systems
- Die `.exe` ist ein gepacktes Python-Programm (PyInstaller); in den Zeichenketten finden sich Lizenzdateien eingebetteter Bibliotheken: `cryptography-35.0.0` (`LICENSE`, `LICENSE.APACHE`, `LICENSE.BSD`, `LICENSE.PSF`, also Apache-2.0 oder BSD-3-Clause), `setuptools-56.0.0` (`LICENSE`, MIT) und zlib/inflate: "inflate 1.2.13 Copyright 1995-2022 Mark Adler" (zlib-Lizenz). Weitere gebuendelte Bibliotheken (z. B. Python selbst, PSF-Lizenz) sind wahrscheinlich, wurden aber nicht einzeln analysiert.

### 1.4 Quellcode-Kommentare mit Hinweis auf Espressif

- `firmware/source/XW-Drone/XW-Drone.ino` Zeile 3: "必须安装ESP32开发板核心库（即esp32 by Espressif Systems）" (Hinweis auf Abhaengigkeit, keine Lizenz).
- `firmware/source/XW-Drone/motors.ino` Zeile 64: Verweis auf https://github.com/espressif/arduino-esp32/issues/5306 (nur Kommentar).

## 2. Bekannte Drittbibliotheken und deren uebliche Lizenzen (nicht im Archiv enthalten, nur eingebunden)

Der Firmware-Quellcode bindet Bibliotheken per `#include` ein, die nicht mitgeliefert werden. Die Angaben beruhen auf Allgemeinwissen und wurden nicht an den Originalquellen verifiziert.

| Include | Bibliothek | Bekannte Lizenz |
|---|---|---|
| `<WiFi.h>`, `<WebServer.h>`, `<ESPmDNS.h>`, `<Preferences.h>`, `<SPI.h>`, `<soc/...>` | Arduino-ESP32 Core (Espressif) | LGPL-2.1 (Core), ESP-IDF-Teile Apache-2.0 |
| `<MAVLink.h>` | MAVLink C-Bibliothek | MIT |
| `<SBUS.h>` | SBUS-Bibliothek (Herkunft nicht im Archiv angegeben) | unbekannt, abhaengig von der verwendeten Bibliothek |
| `<FlixPeriph.h>` | FlixPeriph (IMU-Treiber des Flix-Projekts) | MIT (laut Projekt "Flix" von Oleg Kalachev; nicht verifiziert) |

Hinweis zur Herkunft: Der Code verwendet den Preferences-Namensraum `"flix"`, die Bibliothek `FlixPeriph` und eigene Hilfsklassen (`vector.h`, `quaternion.h`, `lpf.h`, `pid.h`) mit zweisprachigen Kommentaeren. Das spricht stark dafuer, dass XW-Drone auf dem Open-Source-Projekt "Flix" (MIT-Lizenz) basiert. Ein entsprechender MIT-Lizenz- und Urhebervermerk des Original-Autors ist im Archiv **nicht** enthalten. Dies sollte geklaert werden, weil die MIT-Lizenz die Beibehaltung des Copyright-Hinweises verlangt. Diese Zuordnung ist eine Vermutung.

Das Firmware-Abbild `firmware/binaries/XW-Drone.ino.merged.bin` enthaelt zusaetzlich die genannten Bibliotheken sowie ESP-IDF-Code (Apache-2.0) in kompilierter Form.

## 3. Keine Lizenzangabe gefunden

| Komponente / Pfad | Anmerkung |
|---|---|
| `firmware/source/XW-Drone/*.ino`, `*.h` (Flugsteuerung, ausser LICENSE-Datei) | Keine Lizenz-/Copyright-Kopfzeilen in den Quelldateien; massgeblich ist nur die LICENSE-Datei (1.1). Die Datei `docs/guides/firmware-build-requirements.txt` nennt `CF-Drone.ino`, die Datei heisst aber `XW-Drone.ino` (Namensinkonsistenz, vermutlich Umbenennung). |
| `firmware/binaries/XW-Drone.ino.merged.bin` | Kompiliertes Abbild; keine eigene Lizenzangabe (identisch mit der Kopie im lokalen `build/`-Ordner des Archivs, MD5 13bc7ee295e952f61b20626a31e79eff). Faellt vermutlich unter 1.1. |
| `hardware/schematics/SCH_XW_Z1_V2.0.pdf` | Schaltplan (Hardware XW_Z1 V2.0). Keine Lizenz oder Copyright-Angabe im PDF-Text. Das Projekt wird laut README auf OSHWHub veroeffentlicht; dortige Lizenz nicht geprueft. |
| `hardware/3d-models/Drone protective cover.stl` | 3D-Modell, binaeres STL ohne Metadaten; keine Lizenzangabe. |
| `docs/guides/Drone Kit Assembly and Usage Instructions.pdf` | Chinesische Anleitung des Kit-Anbieters, keine Lizenz- oder Copyright-Angabe. |
| `CH341SER.EXE` (nur lokal in `_original/`, **nicht im Repository weitergegeben**; Download-Hinweis in `software/drivers/ch340/README.md`, offizielle Seite: https://www.wch-ic.com/downloads/CH341SER_EXE.html) | USB-Seriell-Treiber (CH340/CH341, vermutlich von WCH / Nanjing Qinheng Microelectronics). Im Programm wurde kein lesbarer Copyright-/Lizenztext gefunden; die Begleitdatei `software/drivers/ch340/使用说明.txt` ist nur eine Installationsanleitung. Proprietaerer Treiber, Weitergabebedingungen ungeklaert. |
| `_original/extracted/.../Reference Tutorial/Tutorial Video_.mp4` | Tutorial-Video (685 MB, nicht im Repository). Keine Lizenzangabe bekannt (Datei nicht auf Metadaten geprueft). |
| Gesamtpaket (Archiv) | Keine uebergreifende README/LICENSE-Datei fuer das Gesamtpaket des Kit-Anbieters. |

## 4. Zusammenfassung

- Ausdruecklich lizenziert: nur der Firmware-Quellcode (GPLv3 + Commons Clause, Licensor 琛光 / ChenGuang) und die Espressif-Dokumente (proprietaer, alle Rechte vorbehalten).
- Widerspruech/Klaerungsbedarf: GPLv3 + Commons Clause (Verkaufsverbot); moeglicher Ursprung im MIT-lizenzierten Flix-Projekt ohne mitgelieferten Lizenzvermerk.
- Ohne Lizenzangabe: Hardware (Schaltplan, STL), Anleitung, Video, CH340-Treiber.
- Die Programme von Dritten (Espressif Flash Download Tool, WCH-Treiber) werden nicht weitergegeben; stattdessen verweisen READMEs auf die offiziellen Download-Seiten. Eine Regel in `.gitignore` (`software/**/*.exe`) verhindert ein versehentliches Einchecken.
- Uebersetzungen: `docs/guides/drone-kit-assembly-and-usage-instructions.en.md` und `firmware/source/XW-Drone/README.en.md` sind inoffizielle englische Uebersetzungen der jeweiligen chinesischen Originale und unterliegen den Rechten der Originalautoren (siehe 1.1 bzw. 3).
