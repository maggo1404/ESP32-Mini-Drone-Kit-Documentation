#!/usr/bin/env bash
# Build Flix firmware (with XW-Z1 board support) using arduino-cli.
#
# Usage: firmware/build.sh <target> [<target> ...]
#        firmware/build.sh all
#
# Targets:
#   xw-z1      XW_Z1 V2.0 kit board (classic ESP32)        -> -DXW_Z1
#   xw-z1-c3   XW/ChenGuang board variant with ESP32-C3     -> -DXW_Z1
#   xw-z1-s3   XW/ChenGuang board variant with ESP32-S3     -> -DXW_Z1
#   flix-esp32 Unmodified upstream configuration (ESP32)    -> regression check
#
# Output: firmware/build/<target>/flix.ino.merged.bin (+ .bin, .bootloader.bin, .partitions.bin)
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SKETCH="$ROOT/firmware/flix/flix"
OUT="$ROOT/firmware/build"
CORE_VERSION="${CORE_VERSION:-3.3.10}"
INDEX_URL="https://espressif.github.io/arduino-esp32/package_esp32_index.json"
ALL_TARGETS="xw-z1 xw-z1-c3 xw-z1-s3 flix-esp32"

export ARDUINO_NETWORK_CONNECTION_TIMEOUT=1h
export ARDUINO_SKETCH_ALWAYS_EXPORT_BINARIES=1

fqbn_for() {
	case "$1" in
		xw-z1|flix-esp32) echo "esp32:esp32:esp32:DebugLevel=error" ;;
		xw-z1-c3)         echo "esp32:esp32:esp32c3:DebugLevel=error,CDCOnBoot=cdc" ;;
		xw-z1-s3)         echo "esp32:esp32:esp32s3:DebugLevel=error,CDCOnBoot=cdc" ;;
		*) echo "Unknown target: $1" >&2; exit 2 ;;
	esac
}

flags_for() {
	case "$1" in
		xw-z1*) echo "-DXW_Z1" ;;
		*)      echo "" ;;
	esac
}

setup() {
	command -v arduino-cli >/dev/null || { echo "arduino-cli not found" >&2; exit 1; }
	if [ ! -f "$OUT/.setup-done" ]; then
		arduino-cli core update-index --additional-urls "$INDEX_URL"
		arduino-cli core install "esp32:esp32@$CORE_VERSION" --additional-urls "$INDEX_URL"
		arduino-cli lib update-index
		arduino-cli lib install "FlixPeriph"
		arduino-cli lib install "MAVLink@2.0.33"
		mkdir -p "$OUT" && touch "$OUT/.setup-done"
	fi
}

build() {
	local target="$1" fqbn flags version
	fqbn="$(fqbn_for "$target")"
	flags="$(flags_for "$target")"
	version="$(git -C "$ROOT" describe --always --dirty 2>/dev/null || echo unknown)"
	echo "==> Building $target ($fqbn, flags: ${flags:-none}, version: $version)"
	arduino-cli compile "$SKETCH" \
		--fqbn "$fqbn" \
		--output-dir "$OUT/$target" \
		--build-property "compiler.cpp.extra_flags=-DVERSION=$version $flags"
	ls -l "$OUT/$target"
}

[ $# -ge 1 ] || { sed -n '2,15p' "$0"; exit 2; }
targets="$*"; [ "$targets" = "all" ] && targets="$ALL_TARGETS"
setup
for t in $targets; do build "$t"; done
