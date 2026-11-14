# Zusammenbau & Bedienung

Grundlage ist die [Kit-Anleitung (chinesisches PDF)](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/docs/guides/Drone%20Kit%20Assembly%20and%20Usage%20Instructions.pdf) in der [inoffiziellen englischen Übersetzung](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/docs/guides/drone-kit-assembly-and-usage-instructions.en.md). Das Original-PDF ist maßgeblich; Fotos und Diagramme (Motorpositionen, Kabelfarben, Propeller-Markierungen) findest du dort. Das ausführliche Lötvideo ist aus Größengründen nicht im Repository.

!!! danger "Propeller abnehmen"
    Bei Flashen, Kalibrierung und Motortests die Propeller immer abnehmen. Motoren nie über USB betreiben, nur mit Akku.

## 1. Motoren und Propeller

1. **Motoren**: Auf die richtige Position achten. Beim fertig gekauften Kit die Motoren an den in der Anleitung markierten Positionen montieren und die **Kabelfarben** beachten. Bei losen Teilen die beim Löten verwendete Kabelreihenfolge einhalten (siehe Video).
2. **Propeller**: Auf die Markierungen (A/B) achten. Falsch montierte Propeller verhindern einen stabilen Flug.

## 2. Platine einbauen und Firmware flashen

1. Platine einbauen: der **Stecker zeigt in dieselbe Richtung wie der Akkustecker**, nicht verkehrt herum.
2. **CH340-Treiber** installieren (siehe [Treiber-Seite](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). Meldet der Installer einen Fehler, war der Treiber evtl. schon installiert; es genügt, wenn der Gerätemanager einen CH340-COM-Port zeigt. Sonst anderen Rechner probieren.
3. Firmware flashen: am einfachsten mit dem [Web-Flasher](flasher.md). Alternativ im Espressif Flash Download Tool über „…“ die Firmware wählen, Adresse `0x0000` eintragen und den **richtigen COM-Port** wählen.
4. Wird trotz Treiber kein COM-Port erkannt, unterstützt das USB-C-Kabel evtl. nur Laden. Anderes Kabel verwenden.

## 3. Einstellen und Fliegen

1. Drohne einschalten. Am Smartphone das WLAN `XW_Drone_WiFi` suchen und verbinden (Passwort `12345678`), dann im Browser `192.168.4.1:8080` öffnen (Doppelpunkt im englischen Tastaturmodus).
2. Vor dem ersten Flug kalibrieren: in der Oberfläche auf „Debug“ tippen, `ca` eingeben und den Anweisungen folgen (Beschleunigungssensor-Kalibrierung).
3. Testflug: erst „Arm“ tippen, die Propeller drehen an. Linker Stick nach oben erhöht den Schub, links/rechts dreht (Gieren). Der rechte Stick steuert Nick/Roll.
4. Landen: Schub **langsam** reduzieren, sonst fällt die Drohne. Mit „Disarm“ die Motoren stoppen. Für Testflüge nicht zu hoch fliegen.

!!! note
    Diese Werte sind die Standardwerte der portierten Flix-Firmware (`XW_Z1`-Target), siehe [Fernsteuerung](remote-control.md). Der Port wurde noch nicht an echter Hardware getestet.
