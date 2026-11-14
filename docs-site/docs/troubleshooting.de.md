# Fehlerbehebung

!!! danger "Propeller abnehmen"
    Bei allen Tests am Boden ohne Propeller arbeiten.

## Kein COM-Port

- **CH340-Treiber** installieren ([Anleitung](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). Meldet der Installer einen Fehler, war er evtl. schon installiert: erst deinstallieren, dann neu installieren. Es genügt, wenn der Gerätemanager einen CH340-Port zeigt.
- **USB-Kabel wechseln**: manche USB-C-Kabel können nur laden.
- Anderen USB-Port oder anderen Rechner probieren.

## Flashen schlägt fehl

- Nur **Chrome/Edge am Desktop** unterstützen den [Web-Flasher](flasher.md). Sonst Flash Download Tool oder esptool nutzen.
- Beim Verbinden **BOOT** gedrückt halten und kurz **EN/RST** tippen.
- Richtigen COM-Port wählen; andere Programme (Serieller Monitor, Flash Download Tool) müssen den Port freigeben.
- Chip-Variante prüfen: ESP32, C3 und S3 haben **verschiedene** Firmware-Dateien.
- Adresse `0x0` bei der `merged.bin`.

## Drohne hebt nicht ab / verhält sich seltsam

- **Akku prüfen**: voll geladen ca. 4,2 V; er muss genug Entladestrom liefern (ca. 15 A, d. h. bei 1000 mAh mindestens 15C). Bei niedriger Spannung sperrt die Firmware das Scharfschalten oder landet automatisch.
- **Propellerposition und -drehrichtung** (A/B-Markierung) und **Motorpositionen/Kabelfarben** prüfen.
- **Kalibrierung** durchführen: Konsole, `ca`, Anweisungen folgen.
- **IMU prüfen**: Befehl `imu` muss `status: OK` und eine Rate um 1000 Hz zeigen; Werte ändern sich beim Bewegen. Einbaulage per `IMU_ROT_*`.
- **Motortest** (ohne Propeller): `mfl`, `mfr`, `mrl`, `mrr` drehen je einen Motor.
- **Lagedarstellung** in QGroundControl beim Kippen der Drohne prüfen.
- Konsole zeigt Müll: Baudrate 115200 einstellen.
- Motoren stoppen im Flug: Umkehrflug-Schutz oder Akku-Schutz (Status-LED schnell blinkend) ausgelöst.
- Wenig Schub oder Abregeln: Schubgrenzen `MOT_THR_MIN` / `MOT_THR_MAX` prüfen *(TODO(verify): Defaults)*.

## WLAN / Fernsteuerung

- Das WLAN erscheint erst, wenn die Drohne mit Akku (oder USB) Strom hat und die Firmware läuft.
- Web-RC lässt sich nicht öffnen: Mobile Daten abschalten, URL mit `http://` (nicht https) eingeben, korrekten Port verwenden ([Fernsteuerung](remote-control.md)).
- QGroundControl verbindet nicht: Firewall und VPN deaktivieren.

## Original-Firmware wiederherstellen

Flashe `firmware/binaries/XW-Drone.ino.merged.bin` ([GitHub](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/firmware/binaries/XW-Drone.ino.merged.bin)) an Adresse `0x0` (Flash Download Tool oder esptool).

Weitere Hinweise in der [Flix-Fehlerbehebung](https://github.com/okalachev/flix/blob/master/docs/troubleshooting.md).
