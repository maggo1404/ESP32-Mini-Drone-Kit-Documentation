# Troubleshooting

!!! danger "Remove the propellers"
    Do all bench tests without propellers.

## No COM port

- Install the **CH340 driver** ([instructions](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/software/drivers/ch340/README.md)). If the installer reports an error it may already be installed: uninstall first, then install again. It is enough if the Device Manager shows a CH340 port.
- **Change the USB cable**: some USB-C cables are charge-only.
- Try another USB port or another computer.

## Flashing fails

- Only **Chrome/Edge on a desktop** support the [web flasher](flasher.md). Otherwise use the Flash Download Tool or esptool.
- While connecting, hold **BOOT** and tap **EN/RST** briefly.
- Choose the right COM port; other programs (serial monitor, Flash Download Tool) must release the port.
- Check the chip variant: ESP32, C3 and S3 need **different** firmware files.
- Use address `0x0` for the `merged.bin`.

## Drone won't take off / behaves strangely

- **Check the battery**: about 4.2 V fully charged; it must deliver enough discharge current (about 15 A, i.e. at least 15C for 1000 mAh). On low voltage the firmware blocks arming or lands automatically.
- Check **propeller positions and direction** (A/B marking) and **motor positions / wire colors**.
- **Calibrate**: console, `ca`, follow the prompts.
- **Check the IMU**: the `imu` command must show `status: OK` and a rate around 1000 Hz; values change when you move the drone. Orientation via `IMU_ROT_*`.
- **Motor test** (no propellers): `mfl`, `mfr`, `mrl`, `mrr` spin one motor each.
- Check the **attitude display** in QGroundControl while tilting the drone.
- Console shows garbage: set the baud rate to 115200.
- Motors stop in flight: inverted-flight or battery protection triggered (status LED blinking fast).
- Weak thrust or limiting: check the throttle limits `MOT_THR_MIN` / `MOT_THR_MAX` (defaults 0.10 / 1.0).

## Wi-Fi / remote control

- The Wi-Fi only appears once the drone is powered (battery or USB) and the firmware runs.
- Web RC does not open: turn off mobile data, enter the URL with `http://` (not https), use the correct port ([Remote control](remote-control.md)).
- QGroundControl does not connect: disable firewall and VPN.

## Restore the original firmware

Flash `firmware/binaries/XW-Drone.ino.merged.bin` ([GitHub](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/firmware/binaries/XW-Drone.ino.merged.bin)) at address `0x0` (Flash Download Tool or esptool).

More hints in the [Flix troubleshooting](https://github.com/okalachev/flix/blob/master/docs/troubleshooting.md).
