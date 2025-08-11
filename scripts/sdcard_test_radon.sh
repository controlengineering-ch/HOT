#!/usr/bin/env bash
# sdcard_test_radon.sh
# Mount -> write radon_latest.txt -> read back -> PASS/FAIL (optional unmount)
set -euo pipefail

RADON_MOUNT_ROOT="${RADON_MOUNT_ROOT:-/mnt/radonsafe_sd}"
TARGET="${1:-}"
UNMOUNT="${UNMOUNT:-0}"
RADON_ROOT_DIR="RadonSafe"
TEST_FILE="radon_latest.txt"

usage() {
  echo "Usage: sudo $0 </dev/sdX1|/dev/mmcblk0p1>  [ENV: UNMOUNT=1]"
  exit 1
}

if [[ $EUID -ne 0 ]]; then
  echo "Must run as root. Try: sudo $0 /dev/sdX1"
  exit 1
fi

if [[ -z "${TARGET}" ]]; then
  usage
fi

SMART="$(dirname "$0")/smart_mount_sdcard_radonsafe.sh"
if [[ ! -x "$SMART" ]]; then
  echo "smart_mount_sdcard_radonsafe.sh not found or not executable at: $SMART"
  exit 1
fi

echo "== Mounting ${TARGET} =="
RADON_MOUNT_ROOT="$RADON_MOUNT_ROOT" "$SMART" --device "$TARGET"

BASENAME="$(basename "$TARGET")"
MNT_PATH="${RADON_MOUNT_ROOT}/sd_${BASENAME}"

if [[ ! -d "$MNT_PATH" ]]; then
  ALT="$(lsblk -no MOUNTPOINT "$TARGET" || true)"
  if [[ -n "${ALT:-}" ]]; then
    MNT_PATH="$ALT"
  fi
fi

if [[ ! -d "$MNT_PATH" ]]; then
  echo "FAIL: mount path not found"; exit 2
fi

echo "== Using mount path: $MNT_PATH =="
mkdir -p "$MNT_PATH/$RADON_ROOT_DIR"/{data,logs,archive}

PAYLOAD="Radon: 175 Bq/m³ @ $(date '+%Y-%m-%d %H:%M:%S')"
echo "$PAYLOAD" > "$MNT_PATH/$RADON_ROOT_DIR/data/$TEST_FILE"
sync

READBACK="$(cat "$MNT_PATH/$RADON_ROOT_DIR/data/$TEST_FILE" || true)"
if [[ "$READBACK" == "$PAYLOAD" ]]; then
  echo "PASS: readback matches"
else
  echo "FAIL: readback mismatch"
  echo " expected: $PAYLOAD"
  echo " got:      $READBACK"
  exit 3
fi

if [[ "${UNMOUNT}" == "1" ]]; then
  echo "== Unmounting $MNT_PATH =="
  umount "$MNT_PATH" || umount -l "$MNT_PATH" || true
fi

echo "DONE."

