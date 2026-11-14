# ESP32 Mini Drone Kit Documentation

Documentation, hardware files, firmware and tools for the ESP32 mini drone kit (original package: `ESP32迷你无人机套件资料`, "ESP32 mini drone kit materials"; kit hardware designation `XW_Z1 V2.0`, firmware `XW-Drone`). Most of the original documents are written in Chinese; a few are bilingual.

## Documentation site and web flasher

- Documentation (German/English): https://maggo1404.github.io/ESP32-Mini-Drone-Kit-Documentation/
- Web flasher (flash the firmware from Chrome/Edge): https://maggo1404.github.io/ESP32-Mini-Drone-Kit-Documentation/flasher/ ([English](https://maggo1404.github.io/ESP32-Mini-Drone-Kit-Documentation/en/flasher/))

## Repository structure

| Folder | Contents |
|---|---|
| `docs/guides/` | Assembly and usage instructions (Chinese PDF) and an unofficial English translation (`drone-kit-assembly-and-usage-instructions.en.md`), firmware build requirements, short notes |
| `docs/tool-manuals/` | Espressif Flash Download Tool user guide (English and Chinese) and release notes |
| `hardware/schematics/` | Board schematic `SCH_XW_Z1_V2.0.pdf` |
| `hardware/3d-models/` | 3D-printable protective cover (`.stl`) |
| `firmware/source/XW-Drone/` | Arduino (ESP32) flight controller source code (`README.MD` in Chinese, unofficial English translation in `README.en.md`) |
| `firmware/flix/` | Git subtree of upstream [Flix](https://github.com/okalachev/flix) with the added `XW_Z1` board target (ESP32 / ESP32-C3 / ESP32-S3) and ported kit features. Source of the web-flasher binaries. |
| `firmware/build.sh` | Local build script (arduino-cli): `firmware/build.sh xw-z1\|xw-z1-c3\|xw-z1-s3\|flix-esp32\|all` |
| `firmware/binaries/` | Prebuilt merged firmware image (`XW-Drone.ino.merged.bin`, flash at address `0x0000`) |
| `software/tools/flash-download-tool/` | Download link, version (3.9.5) and flashing instructions for the Espressif Flash Download Tool, plus its small configuration files. The executable is not included. |
| `software/drivers/ch340/` | Download link and installation notes for the CH340/CH341 USB-serial driver. The installer is not included. |
| `docs-site/` | Source of the bilingual (DE/EN) MkDocs documentation site incl. web flasher (`mkdocs.yml`, `docs/`, `scripts/make_manifests.py`) |
| `.github/workflows/` | CI: firmware build (`firmware.yml`), upstream sync, and docs build/deploy to GitHub Pages (`pages.yml`) |
| `media/` | Placeholder; the 685 MB tutorial video is not included (exceeds GitHub's 100 MB limit) |

The original download (`ESP32迷你无人机套件资料.rar`) and its extracted contents are kept only locally in `_original/`, which is git-ignored and not part of the repository.

## Licenses

Components come from different authors and carry different licenses; some have no license information at all. See [Lizenz.md](Lizenz.md) (German) for a detailed overview. Note in particular that the firmware is published under GPLv3 combined with a "Commons Clause" that forbids selling.
