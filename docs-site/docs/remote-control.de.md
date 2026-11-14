# Fernsteuerung

!!! danger "Sicherheit"
    Zum Konfigurieren und Testen Propeller abnehmen. Fliege nur im Freien oder in großen Räumen, abseits von Personen. Nie Motoren über USB betreiben.

## Web-RC (Smartphone-Browser)

Die Drohne spannt ein eigenes WLAN auf (Access Point). Das Smartphone verbindet sich damit und öffnet die Steuerseite im Browser. Es wird keine App benötigt.

1. Drohne mit Akku einschalten.
2. Smartphone mit dem WLAN der Drohne verbinden: SSID `TODO(verify)`, Passwort `TODO(verify)`. *(Original-Firmware: `XW_Drone_WiFi` / `12345678`.)*
3. Im Browser `http://TODO(verify)` öffnen. *(Original-Firmware: `192.168.4.1:8080`.)*
4. **Debug** → `ca` eingeben und die Beschleunigungssensor-Kalibrierung durchführen (einmalig, danach bei Bedarf).
5. **Unlock** zum Scharfschalten, linker Stick: Schub und Gieren, rechter Stick: Nick und Roll. Zum Landen Schub langsam senken, dann **Lock**.

Sicherheitsfunktionen (portiert aus XW-Drone): Bei niedrigem Akku wird das Scharfschalten gesperrt und eine laufende Drohne landet automatisch; bei dauerhaftem Umkehrflug stoppen die Motoren. Die Status-LED blinkt langsam (armed) bzw. schnell (Warnung). Schubgrenzen über `MOT_THR_MIN` / `MOT_THR_MAX`.

!!! note "TODO(verify)"
    SSID, Passwort, URL/Port und Parameternamen der Web-RC in der portierten Flix-Firmware eintragen und gegen `firmware/flix/flix/` prüfen. Ebenso, ob Web-RC und MAVLink gleichzeitig laufen.

## QGroundControl / MAVLink

Flix spricht MAVLink über WLAN (UDP). Damit funktionieren [QGroundControl](https://docs.qgroundcontrol.com/master/en/qgc-user-guide/getting_started/download_and_install.html) (Desktop und Android) und die [Mavlink-Joystick-App](https://github.com/goldarte/mavlink-joystick/releases/latest).

1. Drohne einschalten und mit deren WLAN verbinden (Flix-Standard: SSID `flix`, Passwort `flixwifi`; *TODO(verify): beim XW_Z1-Target ggf. abweichend*).
2. QGroundControl starten. Es verbindet sich automatisch und zeigt Telemetrie an. Bei Problemen Firewall/VPN am Rechner deaktivieren.
3. Für Joystick-Steuerung am Smartphone: in den Einstellungen *Virtual Joystick* aktivieren, *Auto-Center Throttle* **deaktivieren**. Für sanftere Steuerung `CTL_ATT_MAX` verringern.
4. Konsole: *Analyze Tools → MAVLink Console*. Nützliche Befehle: `help`, `imu`, `ca` (Kalibrierung), `p` (Parameter), `rc`, `mfl`/`mfr`/`mrl`/`mrr` (Motortests, **ohne Propeller**).
5. Parameter: *Vehicle Setup → Parameters*.

Weitere Optionen: SBUS-Funkfernsteuerung (`RC_RX_PIN` setzen, Kalibrierung mit `cr`) oder USB-Joystick über QGroundControl. Flugmodi: STAB (Standard, lagegeregelt), ACRO (Drehraten), RAW (nur Tests), AUTO. Arm: linker Stick unten rechts, Disarm: unten links.

Details: [Flix Usage](https://github.com/okalachev/flix/blob/master/docs/usage.md) (im Repository: `firmware/flix/docs/usage.md`).
