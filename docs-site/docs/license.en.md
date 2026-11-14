# License & liability

This is a technical inventory, **not legal advice**. The detailed analysis (in German) is in [Lizenz.md](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/blob/main/Lizenz.md) in the repository.

!!! danger "Flix has no license"
    The Flix repository by **Oleg Kalachev** contains no license file, not even in its history. Therefore: **all rights reserved**. Only Oleg Kalachev can grant licenses for his code. The firmware on this site is derived from Flix or contains Flix code.

## Overview

| Component | Situation |
|---|---|
| **Flix** (`firmware/flix/`) | No license, all rights reserved by Oleg Kalachev. Source: [okalachev/flix](https://github.com/okalachev/flix) |
| **XW-Drone** (`firmware/source/XW-Drone/`) | GPLv3 + "Commons Clause" (no selling), licensor 琛光 / ChenGuang. The code is clearly derived from Flix and the copyright lines were removed; for the parts taken from Flix the license is very likely not effective, and it is legally contradictory. |
| Schematic, STL, instructions, video | No license information. The English instructions are an unofficial translation. |
| Espressif Flash Download Tool, CH340 driver | Proprietary; **not** in the repository, download links only |
| Libraries (Arduino-ESP32, MAVLink, FlixPeriph) | Own licenses; included compiled in the binary |

## Unofficial distribution of the firmware

The binaries provided here are published **unofficially and solely to help kit owners** flash their drone. There is no approval from the Flix author or the kit manufacturer. Commercial use, selling and redistribution are not intended; ChenGuang's Commons Clause explicitly excludes selling. Rights holders who want it removed can contact us via [GitHub issues](https://github.com/maggo1404/ESP32-Mini-Drone-Kit-Documentation/issues).

## Disclaimer and safety

- Everything is provided **without warranty**, use is **at your own risk**. No liability is accepted for damage to devices, property or persons.
- Drones can injure. **Always remove the propellers when flashing, configuring and running motor tests.** Do not fly over people or animals, observe battery safety, follow local drone regulations.
- Faulty firmware can make the drone uncontrollable. Make first flights low, outdoors and with nobody nearby.
