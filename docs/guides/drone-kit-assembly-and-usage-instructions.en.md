# Drone Kit Assembly and Usage Instructions

> Unofficial English translation of [`Drone Kit Assembly and Usage Instructions.pdf`](Drone%20Kit%20Assembly%20and%20Usage%20Instructions.pdf) (original title: 无人机套件组装及使用说明), which is in Chinese. The original PDF is authoritative. The PDF contains photographs and diagrams (see "Figures" below) that are not reproduced here.

## 1. Motor and propeller assembly

(1) When installing the motors, pay close attention to their positions. If you bought the finished product from our shop, simply install the motors at the positions marked in the figure below, and note the different colors of the motor wires. If you bought the kit as loose parts, follow the wire order you used when soldering; see the tutorial video for reference.

*Figures (PDF page 1): two photographs/diagrams showing the motor positions and wire colors.*

(2) When installing the propellers, pay attention to the markings on them. Do not install them in the wrong positions, otherwise flight will be affected.

*Figure (PDF page 1): propeller markings.*

## 2. Installing the development board and flashing the firmware

(1) When installing the development board, the connector must face the same direction as the battery connector. Be careful not to plug it in the wrong way round.

(2) Before flashing the firmware, install the serial port driver first. The driver file is included in the materials; for the installation method see the tutorial video. If the installation reports a failure, the driver may have been installed before; it is sufficient if the Device Manager recognizes the CH340 driver's serial port. If the installation fails and the serial port is not recognized, a problem with the computer cannot be ruled out; please try another computer.

(3) When flashing the firmware, click "..." to select the firmware to flash, enter the address `0x0000`, and be sure to select the correct COM port!

*Figure (PDF page 2): screenshot of the flashing tool.*

(4) If the COM port is still not recognized after installing the driver, the USB-C cable may not support data transfer; try a different cable.

## 3. Debugging and operation

(1) After flashing the firmware, power on the drone. Search for Wi-Fi networks with your phone, find `XW_Drone_WiFi` and connect to it; the password is `12345678`. After connecting, open `192.168.4.1:8080` in the phone's browser. Note that the ":" must be typed in English input mode.

(2) Before use, the drone must be calibrated/debugged: tap the "Debug" button in the interface, enter `ca`, and follow the prompts. For details, follow the tutorial video.

(3) After debugging you can make a test flight. First tap "Unlock" (arm); the propellers start to spin. Pull the left stick up to increase the throttle; moving it left or right adjusts the yaw. The drone then takes off slowly. The right stick controls pitch/roll.

(4) When landing, lower the throttle slowly; otherwise the drone will fall and may be damaged. Tap the "Lock" (disarm) button to stop the motors from spinning. For test flights it is recommended not to fly too high; get familiar with the stick operation first.

*The complete soldering tutorial and operating instructions can be found in the tutorial video included in the materials.*

## Figures

The original PDF has 2 pages with 5 distinct photographs/diagrams (page 1: motor positions and wire colors, propeller markings; page 2: flashing tool screenshot, firmware selection and settings). They can be viewed in the original PDF.

Translator's notes: the step "ca" in 3 (2) is the debug-console command given in the original; the word "调试" was translated as "debug/calibrate" depending on context. "解锁/上锁" (unlock/lock) means arming/disarming the motors.
