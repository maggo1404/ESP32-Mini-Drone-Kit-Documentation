#!/usr/bin/env python3
"""Generate ESP Web Tools manifests for the firmware published on GitHub Pages.

Expects the CI artifacts already unpacked as <site-dir>/firmware/<target>/flix.ino*.bin
and writes <site-dir>/firmware/manifest-<target>.json plus version.txt files.
Fails (exit 1) if an expected file is missing. Python standard library only.
"""
import argparse
import json
import sys
from pathlib import Path

# target -> (ESP Web Tools chipFamily, display name)
TARGETS = {
    "xw-z1": ("ESP32", "XW-Z1 (ESP32)"),
    "xw-z1-c3": ("ESP32-C3", "XW-Z1 C3 (ESP32-C3)"),
    "xw-z1-s3": ("ESP32-S3", "XW-Z1 S3 (ESP32-S3)"),
    "flix-esp32": ("ESP32", "Flix stock (ESP32)"),
}
FILES = ["flix.ino.merged.bin", "flix.ino.bin", "flix.ino.bootloader.bin", "flix.ino.partitions.bin"]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--site-dir", default="site", help="built site directory (default: site)")
    ap.add_argument("--version", required=True, help="firmware version, e.g. short commit sha")
    ap.add_argument("--targets", nargs="*", default=list(TARGETS), help="targets to process")
    args = ap.parse_args()

    fw = Path(args.site_dir) / "firmware"
    missing = [str(fw / t / f) for t in args.targets for f in FILES if not (fw / t / f).is_file()]
    if missing:
        print("ERROR: missing firmware files:", *missing, sep="\n  ", file=sys.stderr)
        return 1

    for t in args.targets:
        chip, name = TARGETS[t]
        manifest = {
            "name": f"ESP32 Mini Drone Kit - {name}",
            "version": args.version,
            "new_install_prompt_erase": True,
            "builds": [{
                "chipFamily": chip,
                # relative to the manifest file, i.e. firmware/<target>/...
                "parts": [{"path": f"{t}/flix.ino.merged.bin", "offset": 0}],
            }],
        }
        out = fw / f"manifest-{t}.json"
        out.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        print("wrote", out)

    for p in (fw / "version.txt", Path(args.site_dir) / "version.txt"):
        p.write_text(args.version + "\n", encoding="utf-8")
        print("wrote", p)
    return 0


if __name__ == "__main__":
    sys.exit(main())
