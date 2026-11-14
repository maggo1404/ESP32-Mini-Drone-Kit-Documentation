# Fernsteuerung

!!! danger "Sicherheit"
    Zum Konfigurieren und Testen Propeller abnehmen. Fliege nur im Freien oder in großen Räumen, abseits von Personen. Nie Motoren über USB betreiben.

## Web-RC (Smartphone-Browser)

Die Drohne spannt ein eigenes WLAN auf (Access Point). Das Smartphone verbindet sich damit und öffnet die Steuerseite im Browser. Es wird keine App benötigt.

1. Drohne mit Akku einschalten.
2. Smartphone mit dem WLAN der Drohne verbinden: SSID `XW_Drone_WiFi`, Passwort `12345678` (Standard beim XW_Z1; änderbar mit dem Konsolenbefehl `ap <ssid> <passwort>`, `preset` setzt zurück).
3. Im Browser `http://192.168.4.1:8080` öffnen.
4. **Debug** → `ca` eingeben und die Beschleunigungssensor-Kalibrierung durchführen (einmalig, danach bei Bedarf).
5. **Arm** zum Scharfschalten (wird abgelehnt, solange der Schub über 0,05 liegt); linker Stick: Schub und Gieren, rechter Stick: Nick und Roll. Zum Landen Schub langsam senken, dann **Disarm**.

Sicherheitsfunktionen (portiert aus XW-Drone): Bei niedrigem Akku wird das Scharfschalten gesperrt und eine laufende Drohne landet automatisch; bei dauerhaftem Umkehrflug stoppen die Motoren. Die Status-LED blinkt langsam (armed) bzw. schnell (Warnung). Schubgrenzen über `MOT_THR_MIN` / `MOT_THR_MAX`, siehe [Firmware](firmware.md).

Die Seite sendet die Stickpositionen mit 20 Hz und alle 2 s einen Heartbeat. Schaltflächen:

- **Arm** / **Disarm**
- **E-stop**: Notaus
- **Mode**: schaltet zwischen STAB und ACRO um (kein ALTHOLD)
- **Debug console**: zeigt `print()`-Ausgaben und führt Konsolenbefehle aus

Solange die Seite verbunden ist, überschreibt sie SBUS und die MAVLink-Handsteuerung. Kommen 8 s lang keine Pakete an, sinkt die Drohne über das RC-Loss-Failsafe ab.

!!! warning "Einschränkungen"
    Lange Konsolenbefehle (`ca`, `cr` Kalibrierung) blockieren den Webserver, bis sie fertig sind. Der XW_Z1-Port wurde noch nicht an echter Hardware getestet.

## QGroundControl / MAVLink

Flix spricht MAVLink über WLAN (UDP). Damit funktionieren [QGroundControl](https://docs.qgroundcontrol.com/master/en/qgc-user-guide/getting_started/download_and_install.html) (Desktop und Android) und die [Mavlink-Joystick-App](https://github.com/goldarte/mavlink-joystick/releases/latest).

1. Drohne einschalten und mit deren WLAN verbinden (XW_Z1-Build: SSID `XW_Drone_WiFi`, Passwort `12345678`; Standard-Build `flix-esp32`: `flix` / `flixwifi`). QGroundControl/MAVLink funktionieren über denselben Access Point wie im Original-Flix.
2. QGroundControl starten. Es verbindet sich automatisch und zeigt Telemetrie an. Bei Problemen Firewall/VPN am Rechner deaktivieren.
3. Für Joystick-Steuerung am Smartphone: in den Einstellungen *Virtual Joystick* aktivieren, *Auto-Center Throttle* **deaktivieren**. Für sanftere Steuerung `CTL_ATT_MAX` verringern.
4. Konsole: *Analyze Tools → MAVLink Console*. Nützliche Befehle: `help`, `imu`, `ca` (Kalibrierung), `p` (Parameter), `rc`, `mfl`/`mfr`/`mrl`/`mrr` (Motortests, **ohne Propeller**).
5. Parameter: *Vehicle Setup → Parameters*.

Weitere Optionen: SBUS-Funkfernsteuerung (`RC_RX_PIN` setzen, Kalibrierung mit `cr`) oder USB-Joystick über QGroundControl. Flugmodi: STAB (Standard, lagegeregelt), ACRO (Drehraten), RAW (nur Tests), AUTO. Arm: linker Stick unten rechts, Disarm: unten links.

Details: [Flix Usage](https://github.com/okalachev/flix/blob/master/docs/usage.md) (im Repository: `firmware/flix/docs/usage.md`).
