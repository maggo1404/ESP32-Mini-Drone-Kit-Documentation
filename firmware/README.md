# Firmware

Firmware for the ESP32 mini drone kit, based on the open-source
[Flix](https://github.com/okalachev/flix) quadcopter firmware with added XW-Z1 board support.

## Layout

| Path | Content |
|---|---|
| `flix/` | Git subtree of upstream `okalachev/flix` (`master`), including the XW-Z1 port |
| `source/XW-Drone/` | Original Chinese fork (XW-Drone) as shipped with the kit, kept for reference only |
| `binaries/` | Prebuilt original image from the kit |
| `build.sh` | Local/CI build script (arduino-cli) |
| `scripts/` | `check_merged_bin.py` (flash image checker) and its unit test |
| `build/` | Build output (generated, not committed) |

## Build locally

Requires [`arduino-cli`](https://arduino.github.io/arduino-cli/) on `PATH` (the ESP32 core 3.3.10 and libraries are installed on first run).

```bash
firmware/build.sh xw-z1          # one target
firmware/build.sh all            # all targets
python3 firmware/scripts/check_merged_bin.py firmware/build/xw-z1/flix.ino.merged.bin --target xw-z1
python3 firmware/scripts/test_check_merged_bin.py
```

Output per target in `firmware/build/<target>/`: `flix.ino.merged.bin`, `flix.ino.bin`,
`flix.ino.bootloader.bin`, `flix.ino.partitions.bin`.
Flash `flix.ino.merged.bin` at address `0x0`.

## Targets

| Target | Chip | Notes |
|---|---|---|
| `xw-z1` | ESP32 | XW_Z1 V2.0 kit board (`-DXW_Z1`) |
| `xw-z1-c3` | ESP32-C3 | XW/ChenGuang board variant (`-DXW_Z1`) |
| `xw-z1-s3` | ESP32-S3 | XW/ChenGuang board variant (`-DXW_Z1`) |
| `flix-esp32` | ESP32 | Unmodified upstream configuration, regression check |

Inside a merged image: bootloader at `0x1000` (ESP32) or `0x0` (C3/S3), partition table at `0x8000`, app at `0x10000`.

## Upstream sync

`flix/` is a `git subtree` (squashed). To update manually:

```bash
git subtree pull --prefix=firmware/flix https://github.com/okalachev/flix master --squash
```

The workflow `upstream-sync.yml` does this weekly on a branch `upstream-sync/flix-<sha>`,
builds all targets and opens a PR; on merge conflicts or build failures it opens/updates an
issue labelled `upstream-sync`. (PRs created by `GITHUB_TOKEN` do not trigger other workflows,
so the build runs inside the sync job.)

## CI

- `.github/workflows/firmware.yml`: jobs `unit-tests`, `build (<target>)` (matrix), `ci-ok`, and
  `release` on `v*` tags (attaches `flix-<target>-<tag>.merged.bin`). Artifacts: `firmware-<target>`.
  It runs on pushes to `main`, on every pull request (merge commit), manually, and via `workflow_call`.
- `.github/workflows/upstream-sync.yml`: see above.

Recommendation: enable branch protection on `main` and require the single status check **`ci-ok`**.

## License

The XW-Drone sources are under GPLv3 combined with a Commons Clause (no selling) and are
legally ambiguous; Flix has its own license. See [../Lizenz.md](../Lizenz.md) before redistributing
or commercially using any firmware. This is not legal advice.
