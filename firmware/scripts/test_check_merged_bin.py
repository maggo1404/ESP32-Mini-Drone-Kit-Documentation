#!/usr/bin/env python3
"""Self-test for check_merged_bin.py using synthetic images."""
import contextlib
import io
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import check_merged_bin as cmb  # noqa: E402


def part(label, ptype, subtype, off, size):
    return b"\xaa\x50" + struct.pack("<BBII", ptype, subtype, off, size) + \
        label.encode().ljust(16, b"\0") + b"\0\0\0\0"


def image(chip_id, n=64):
    hdr = bytes([0xE9, 1, 0, 0x20]) + b"\0" * 4 + b"\0" * 4 + struct.pack("<H", chip_id) + b"\0\0"
    return hdr + b"\x5a" * n


def build(chip="esp32", chip_id=None, app_part=True, boot_magic=True, flash=None):
    boot_off, cid = cmb.CHIPS[chip]
    chip_id = cid if chip_id is None else chip_id
    boot = image(cid, 100)
    if not boot_magic:
        boot = b"\x00" + boot[1:]
    table = part("nvs", 1, 2, 0x9000, 0x5000)
    if app_part:
        table += part("app0", 0, 0x10, 0x10000, 0x140000)
    table += b"\xeb\xeb" + b"\xff" * 30
    app = image(chip_id, 500)
    buf = bytearray(b"\xff" * (cmb.APP_OFFSET + len(app)))
    buf[boot_off:boot_off + len(boot)] = boot
    buf[0x8000:0x8000 + len(table)] = table
    buf[cmb.APP_OFFSET:cmb.APP_OFFSET + len(app)] = app
    return bytes(buf), boot, table, app


class T(unittest.TestCase):
    def run_check(self, files, chip="esp32"):
        with tempfile.TemporaryDirectory() as d:
            for name, content in files.items():
                with open(os.path.join(d, name), "wb") as f:
                    f.write(content)
            with contextlib.redirect_stdout(io.StringIO()) as out:
                rc = cmb.main([os.path.join(d, "flix.ino.merged.bin"), "--chip", chip])
            return rc, out.getvalue()

    def test_pass_all_chips(self):
        for chip in cmb.CHIPS:
            merged, boot, table, app = build(chip)
            rc, out = self.run_check({"flix.ino.merged.bin": merged, "flix.ino.bin": app,
                                      "flix.ino.bootloader.bin": boot,
                                      "flix.ino.partitions.bin": table}, chip)
            self.assertEqual(rc, 0, out)
            self.assertIn("RESULT: PASS", out)
            self.assertIn("app0", out)

    def test_target_option(self):
        merged = build("esp32c3")[0]
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, "m.bin")
            open(p, "wb").write(merged)
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(cmb.main([p, "--target", "xw-z1-c3"]), 0)
                self.assertEqual(cmb.main([p, "--target", "xw-z1"]), 1)

    def test_wrong_chip_id(self):
        merged = build("esp32", chip_id=0x0009)[0]
        rc, out = self.run_check({"flix.ino.merged.bin": merged})
        self.assertEqual(rc, 1)
        self.assertIn("chip id", out)

    def test_missing_app_partition(self):
        rc, out = self.run_check({"flix.ino.merged.bin": build(app_part=False)[0]})
        self.assertEqual(rc, 1)
        self.assertIn("no app partition", out)

    def test_bad_bootloader_magic(self):
        rc, _ = self.run_check({"flix.ino.merged.bin": build(boot_magic=False)[0]})
        self.assertEqual(rc, 1)

    def test_bad_partition_magic(self):
        merged = bytearray(build()[0])
        merged[0x8000] = 0
        rc, _ = self.run_check({"flix.ino.merged.bin": bytes(merged)})
        self.assertEqual(rc, 1)

    def test_app_mismatch(self):
        merged, boot, table, app = build()
        rc, out = self.run_check({"flix.ino.merged.bin": merged,
                                  "flix.ino.bin": app[:-1] + b"\x00"})
        self.assertEqual(rc, 1)
        self.assertIn("differs", out)

    def test_too_big_and_empty(self):
        rc, _ = self.run_check({"flix.ino.merged.bin": b""})
        self.assertEqual(rc, 1)
        big = build()[0] + b"\xff" * cmb.FLASH_SIZE  # > 4 MB
        rc, _ = self.run_check({"flix.ino.merged.bin": big})
        self.assertEqual(rc, 1)


if __name__ == "__main__":
    unittest.main()
