#!/usr/bin/env python3
"""
Contourne les limitations du backend libusb de hidapi sur Linux en lisant
directement le report descriptor HID via sysfs, puis en communiquant avec
le périphérique via /dev/hidrawX (lecture/écriture brute).

Ne nécessite PAS le module 'hid' / 'hidapi'.
"""
import sys
import select
import glob
import os

TARGET_VID = 0x373b
TARGET_PID = 0x1054  # adapte si besoin

TARGET_USAGE_PAGE = 0xFF60
TARGET_USAGE = 0x61

REPORT_LENGTH = 32


def send_command(device_path, payload_bytes, report_length=REPORT_LENGTH, timeout=1.0):
    padded = bytes(payload_bytes) + bytes(report_length - len(payload_bytes))
    fd = os.open(device_path, os.O_RDWR)
    try:
        os.write(fd, padded)
        ready, _, _ = select.select([fd], [], [], timeout)
        if not ready:
            raise TimeoutError("Pas de réponse du clavier (timeout)")
        return os.read(fd, report_length)
    finally:
        os.close(fd)

def read_hex_attr(path):
    with open(path) as f:
        return int(f.read().strip(), 16)


def descriptor_matches(desc_bytes, usage_page, usage):
    """
    Parcourt un report descriptor HID (format 'short items') et vérifie
    si une paire Usage Page / Usage correspond à celle recherchée,
    juste avant une Collection (0xA1) -- ce qui signale une interface
    applicative top-level (comme celle de QMK Raw HID).
    """
    i = 0
    current_usage_page = None
    pending_usage = None

    while i < len(desc_bytes):
        prefix = desc_bytes[i]
        tag = prefix & 0xFC
        size = prefix & 0x03
        size = {0: 0, 1: 1, 2: 2, 3: 4}[size]
        data = desc_bytes[i + 1: i + 1 + size]
        value = int.from_bytes(data, "little") if data else 0

        if tag == 0x04:  # Usage Page (global)
            current_usage_page = value
        elif tag == 0x08:  # Usage (local)
            pending_usage = value
        elif tag == 0xA0:  # Collection
            if current_usage_page == usage_page and pending_usage == usage:
                return True

        i += 1 + size

    return False


def find_hidraw_device():
    for hidraw_path in sorted(glob.glob("/sys/class/hidraw/hidraw*")):
        name = os.path.basename(hidraw_path)
        device_dir = os.path.join(hidraw_path, "device")

        uevent_path = os.path.join(device_dir, "uevent")
        if not os.path.exists(uevent_path):
            continue

        # Récupère VID/PID depuis uevent (format: HID_ID=0003:0000373B:00001054)
        vid = pid = None
        with open(uevent_path) as f:
            for line in f:
                if line.startswith("HID_ID="):
                    parts = line.strip().split("=")[1].split(":")
                    vid = int(parts[1], 16)
                    pid = int(parts[2], 16)

        if vid != TARGET_VID or pid != TARGET_PID:
            continue

        report_desc_path = os.path.join(device_dir, "report_descriptor")
        if not os.path.exists(report_desc_path):
            continue

        with open(report_desc_path, "rb") as f:
            desc = f.read()

        if descriptor_matches(desc, TARGET_USAGE_PAGE, TARGET_USAGE):
            return f"/dev/{name}"

    return None


def main():
    device_path = find_hidraw_device()
    if device_path is None:
        print("Aucune interface Raw HID (0xFF60/0x61) trouvée pour ce VID/PID.")
        return
    print(f"Interface Raw HID trouvée : {device_path}")
    # Lire la commande depuis les arguments, sinon 0x01 par défaut
    if len(sys.argv) > 1:
        payload_bytes = [int(arg, 0) for arg in sys.argv[1:]]
    else:
        payload_bytes = [0x01]
    try:
        response = send_command(device_path, payload_bytes)
    except PermissionError:
        print(f"Permission refusée sur {device_path}.")
        return
    except TimeoutError as e:
        print(e)
        return
    print("Envoyé :", bytes(payload_bytes).hex())
    print("Reçu   :", response.hex())


if __name__ == "__main__":
    main()
