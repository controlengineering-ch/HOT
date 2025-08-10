#!/usr/bin/env bash
set -euo pipefail

# === Config ===
LOG_DIR="/var/log/radon_safe"
LOG_FILE="$LOG_DIR/sdcard_mount.log"

# === Globals ===
DEBUG=0
TARGET=""
VOL_LABEL=""
YES=0

# === Utils ===
log() { local ts; ts="$(date '+%Y-%m-%d %H:%M:%S')"; mkdir -p "$LOG_DIR" || true; echo "[$ts] $*" | tee -a "$LOG_FILE" >/dev/null; }
dbg() { [[ "$DEBUG" -eq 1 ]] && echo "[DEBUG] $*"; }
require_root() { [[ $EUID -ne 0 ]] && { echo "This action requires root. Try with: sudo $0 $*" >&2; exit 1; }; }
confirm() { local prompt="${1:-Proceed? [y/N]} "; if [[ "$YES" -eq 1 ]]; then return 0; fi; read -r -p "$prompt" ans || true; [[ "$ans" =~ ^(y|Y|yes|YES)$ ]]; }

# List candidates robustly
list_candidates() {
  lsblk -P -p -o NAME,FSTYPE,MOUNTPOINT,SIZE,MODEL,TYPE | while read -r line; do
    eval "$line"
    if [[ "$TYPE" =~ ^(part|disk)$ ]] && [[ -n "$FSTYPE" ]]; then
      printf "%s\t%s\t-\t%s\t%s\t%s\n" "$NAME" "${FSTYPE:-}" "${MOUNTPOINT:- -}" "${SIZE:- -}" "${MODEL:- -}"
    fi
  done
}
pretty_list() {
  printf "Candidates:\n"
  printf "%-20s %-8s %-7s %-25s %-8s %s\n" "DEVICE" "FSTYPE" "RM" "MOUNTPOINT" "SIZE" "MODEL"
  list_candidates | while IFS=$'\t' read -r dev fstype rm mnt size model; do
    printf "%-20s %-8s %-7s %-25s %-8s %s\n" "$dev" "$fstype" "$rm" "$mnt" "$size" "$model"
  done
}

is_fat32() { local dev="$1"; local t; t="$(blkid -o value -s TYPE "$dev" 2>/dev/null || true)"; [[ "$t" == "vfat" || "$t" == "fat" || "$t" == "msdos" ]]; }
ensure_fat32() { local dev="$1"; if is_fat32 "$dev"; then dbg "$dev is already FAT32/vfat"; else echo "Partition $dev is not FAT32."; if confirm "Format $dev as FAT32? ALL DATA WILL BE LOST. [y/N] "; then require_root; log "Formatting $dev as FAT32"; mkfs.vfat -F 32 ${VOL_LABEL:+-n "$VOL_LABEL"} "$dev"; sync; echo "Formatted."; else return 1; fi; fi; return 0; }

set_vfat_label_if_needed() { local dev="$1"; local wanted="$2"; [[ -z "$wanted" ]] && return 0; local cur; cur="$(blkid -o value -s LABEL "$dev" 2>/dev/null || true)"; if [[ "$cur" != "$wanted" ]]; then require_root; if command -v fatlabel >/dev/null 2>&1; then fatlabel "$dev" "$wanted" || true; elif command -v dosfslabel >/dev/null 2>&1; then dosfslabel "$dev" "$wanted" || true; fi; log "Set vfat label on $dev to '$wanted'"; else dbg "Label already '$wanted' on $dev"; fi; }

auto_unmount_if_mounted_elsewhere() { local dev="$1"; local mnts; mnts=$(findmnt -rn -S "$dev" -o TARGET || true); if [[ -n "$mnts" ]]; then require_root; while read -r mnt; do log "Auto-unmounting $dev from $mnt"; umount -l "$mnt"; done <<< "$mnts"; fi; }

mount_dev() { local dev="$1"; local base="$(basename "$dev")"; local mnt="${TARGET:-/mnt/radonsafe/$base}"; auto_unmount_if_mounted_elsewhere "$dev"; mkdir -p "$mnt"; if findmnt -rn -T "$mnt" >/dev/null 2>&1; then echo "$dev already mounted on $mnt"; return 0; fi; require_root; log "Mounting $dev to $mnt"; mount -o rw,utf8,uid=0,gid=0,umask=022 "$dev" "$mnt"; echo "$dev mounted at $mnt"; }

umount_dev() { local dev="$1"; local mnts; mnts=$(findmnt -rn -S "$dev" -o TARGET || true); if [[ -z "$mnts" ]]; then echo "$dev is not mounted."; return 0; fi; require_root; while read -r mnt; do log "Unmounting $dev from $mnt"; umount "$mnt"; echo "Unmounted $dev from $mnt"; done <<< "$mnts"; }

pick_device() { local first; first="$(list_candidates | grep -E '\tvfat\t' | head -n1 | awk '{print $1}')"; if [[ -z "$first" ]]; then echo "No candidate devices found." >&2; exit 2; fi; echo "$first"; }

usage() { cat <<'USAGE'
smart_mount_sdcard_radonsafe.sh [options] [DEVICE]

Options:
  --list                     List detected filesystems (partitions and whole-disk)
  --mount [DEVICE]           Ensure FAT32 and mount (asks confirmation)
  --unmount [DEVICE]         Unmount DEVICE
  --auto                     Ensure FAT32 and mount first candidate (no prompts)
  --target DIR               Mountpoint directory (default: /mnt/radonsafe/<devname>)
  --label NAME               Ensure volume label NAME for vfat (sets if missing/different)
  --yes, -y                  Skip confirmations (assume yes)
  --debug                    Verbose debug logging
  -h, --help                 Show this help

Examples:
  
  sudo ./smart_mount_sdcard_radonsafe.sh --mount /dev/sdb1 --target /mnt/radonsafe/sd --label RADONSAFE -y
  sudo ./smart_mount_sdcard_radonsafe.sh --auto -y
USAGE
}

MODE=""; DEVICE="${DEVICE:-}"
while [[ $# -gt 0 ]]; do
  case "$1" in
    --list) MODE="list"; shift ;;
    --mount) MODE="mount"; shift; [[ $# -gt 0 && "${1:-}" == /dev/* ]] && DEVICE="$1" && shift || true ;;
    --unmount) MODE="unmount"; shift; [[ $# -gt 0 && "${1:-}" == /dev/* ]] && DEVICE="$1" && shift || true ;;
    --auto) MODE="auto"; shift ;;
    --target) TARGET="${2:-}"; shift 2 ;;
    --label) VOL_LABEL="${2:-}"; shift 2 ;;
    --yes|-y) YES=1; shift ;;
    --debug) DEBUG=1; shift ;;
    -h|--help) usage; exit 0 ;;
    /dev/*) DEVICE="$1"; shift ;;
    *) echo "Unknown argument: $1"; usage; exit 1 ;;
  esac
done

case "${MODE:-}" in
  list) pretty_list ;;
  mount) dev="${DEVICE:-$(pick_device)}"; echo "Selected: $dev"; ensure_fat32 "$dev" || exit 3; set_vfat_label_if_needed "$dev" "$VOL_LABEL"; echo; if confirm "Mount $dev now? [y/N] "; then mount_dev "$dev"; fi ;;
  unmount) dev="${DEVICE:-$(pick_device)}"; umount_dev "$dev" ;;
  auto) dev="${DEVICE:-$(pick_device)}"; ensure_fat32 "$dev" || exit 3; set_vfat_label_if_needed "$dev" "$VOL_LABEL"; mount_dev "$dev" ;;
  *) usage; exit 1 ;;
esac

