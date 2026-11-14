#!/usr/bin/env python3
"""Sanity-check an ESP32 merged flash image (flashed at 0x0).

Usage:
  check_merged_bin.py <merged.bin> --chip esp32|esp32c3|esp32s3
  check_merged_bin.py <merged.bin> --target xw-z1|xw-z1-c3|xw-z1-s3|flix-esp32

Checks: size (1 byte..4 MB), bootloader/app image magic (0xE9), partition table magic (0xAA50),
an `app` partition at 0x10000, chip id in the app image header, and that the
bootloader/partitions/app bytes equal the sibling flix.ino.*.bin files (if present).
Python 3 standard library only. Exit code 0 = PASS, 1 = FAIL, 2 = usage error.
"""
import argparse
import os
import struct
import sys

FLASH_SIZE = 4 * 1024 * 1024
PART_TABLE_OFFSET = 0x8000
APP_OFFSET = 0x10000
IMAGE_MAGIC = 0xE9
# chip -> (bootloader offset, chip id in extended image header)
CHIPS = {
    "esp32": (0x1000, 0x0000),
    "esp32c3": (0x0000, 0x0005),
    "esp32s3": (0x0000, 0x0009),
}
TARGET_CHIP = {
    "xw-z1": "esp32",
    "xw-z1-c3": "esp32c3",
    "xw-z1-s3": "esp32s3",
    "flix-esp32": "esp32",
}
PART_TYPES = {0x00: "app", 0x01: "data"}


def parse_partitions(data):
    """Return list of dicts; raise ValueError on bad table."""
    entries = []
    end = min(len(data), PART_TABLE_OFFSET + 0x1000)
    for off in range(PART_TABLE_OFFSET, end, 32):
        raw = data[off:off + 32]
        if len(raw) < 32 or raw[:2] == b"\xff\xff":
            break
        if raw[:2] == b"\xeb\xeb":  # MD5 checksum entry
            break
        if raw[:2] != b"\xaa\x50":
            raise ValueError("bad partition entry magic at 0x%X: %s" % (off, raw[:2].hex()))
        ptype, subtype, poff, psize = struct.unpack("<BBII", raw[2:12])
        label = raw[12:28].split(b"\0", 1)[0].decode("ascii", "replace")
        entries.append({"label": label, "type": ptype, "subtype": subtype,
                        "offset": poff, "size": psize})
    return entries


class Checker:
    def __init__(self):
        self.failed = 0

    def ok(self, msg):
        print("  PASS  " + msg)

    def fail(self, msg):
        self.failed += 1
        print("  FAIL  " + msg)

    def check(self, cond, okmsg, failmsg):
        (self.ok if cond else self.fail)(okmsg if cond else failmsg)
        return cond


def compare_part(c, data, name, path, offset):
    if not os.path.isfile(path):
        print("  SKIP  %s not found (%s)" % (name, path))
        return
    with open(path, "rb") as f:
        ref = f.read()
    got = data[offset:offset + len(ref)]
    c.check(len(ref) > 0 and got == ref,
            "%s matches %s at 0x%X (%d bytes)" % (name, os.path.basename(path), offset, len(ref)),
            "%s differs from %s at 0x%X (%d bytes)" % (name, os.path.basename(path), offset, len(ref)))


def check(path, chip):
    c = Checker()
    boot_off, chip_id = CHIPS[chip]
    print("Checking %s (chip %s)" % (path, chip))
    try:
        with open(path, "rb") as f:
            data = f.read()
    except OSError as e:
        c.fail("cannot read file: %s" % e)
        return c.failed
    c.check(0 < len(data) <= FLASH_SIZE,
            "size %d bytes (<= 4 MB)" % len(data),
            "size %d bytes not in 1..4 MB" % len(data))
    if len(data) < APP_OFFSET + 16:
        c.fail("file too small to contain an app image at 0x%X" % APP_OFFSET)
        return c.failed

    c.check(data[boot_off] == IMAGE_MAGIC,
            "bootloader magic 0xE9 at 0x%X" % boot_off,
            "bootloader magic at 0x%X is 0x%02X, expected 0xE9" % (boot_off, data[boot_off]))
    c.check(data[PART_TABLE_OFFSET:PART_TABLE_OFFSET + 2] == b"\xaa\x50",
            "partition table magic AA 50 at 0x%X" % PART_TABLE_OFFSET,
            "partition table magic at 0x%X is %s, expected aa50"
            % (PART_TABLE_OFFSET, data[PART_TABLE_OFFSET:PART_TABLE_OFFSET + 2].hex()))

    try:
        entries = parse_partitions(data)
    except ValueError as e:
        c.fail(str(e))
        entries = []
    print("  Partition table:")
    for e in entries:
        print("    %-10s type=%-4s subtype=0x%02X offset=0x%06X size=0x%06X"
              % (e["label"], PART_TYPES.get(e["type"], "0x%02X" % e["type"]),
                 e["subtype"], e["offset"], e["size"]))
    c.check(any(e["type"] == 0x00 and e["offset"] == APP_OFFSET for e in entries),
            "app partition at 0x%X" % APP_OFFSET,
            "no app partition at 0x%X" % APP_OFFSET)
    for e in entries:
        if e["offset"] + e["size"] > FLASH_SIZE:
            c.fail("partition %s exceeds 4 MB flash" % e["label"])

    c.check(data[APP_OFFSET] == IMAGE_MAGIC,
            "app image magic 0xE9 at 0x%X" % APP_OFFSET,
            "app image magic at 0x%X is 0x%02X, expected 0xE9" % (APP_OFFSET, data[APP_OFFSET]))
    got_id = struct.unpack("<H", data[APP_OFFSET + 12:APP_OFFSET + 14])[0]
    c.check(got_id == chip_id,
            "app chip id 0x%04X matches %s" % (got_id, chip),
            "app chip id 0x%04X, expected 0x%04X for %s" % (got_id, chip_id, chip))

    d = os.path.dirname(os.path.abspath(path))
    compare_part(c, data, "app", os.path.join(d, "flix.ino.bin"), APP_OFFSET)
    compare_part(c, data, "bootloader", os.path.join(d, "flix.ino.bootloader.bin"), boot_off)
    compare_part(c, data, "partitions", os.path.join(d, "flix.ino.partitions.bin"), PART_TABLE_OFFSET)

    print("RESULT: %s" % ("PASS" if not c.failed else "FAIL (%d check(s) failed)" % c.failed))
    return c.failed


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("merged_bin")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--chip", choices=sorted(CHIPS))
    g.add_argument("--target", choices=sorted(TARGET_CHIP))
    a = ap.parse_args(argv)
    chip = a.chip or TARGET_CHIP[a.target]
    return 1 if check(a.merged_bin, chip) else 0


if __name__ == "__main__":
    sys.exit(main())
