/*
 * hid_raw.c — communication directe avec /dev/hidrawX, sans dependre
 * du backend libusb de hidapi (qui ne remplit pas usage_page/usage
 * sous Linux). Portage C du script Python raw_hid_direct.py, valide
 * manuellement sur le MAD60HE (VIA protocol v9).
 */
#include "hid_raw.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define SYSFS_HIDRAW_DIR "/sys/class/hidraw"

/* Parcourt un report descriptor HID (format "short items") et renvoie
 * 1 si la paire (usage_page, usage) apparait juste avant une
 * Collection (tag 0xA0), ce qui signale une interface applicative
 * top-level -- exactement le pattern de l'interface QMK Raw HID. */
static int descriptor_matches(const uint8_t *desc, size_t len,
                               uint16_t usage_page, uint16_t usage)
{
    size_t i = 0;
    long current_usage_page = -1;
    long pending_usage = -1;

    while (i < len) {
        uint8_t prefix = desc[i];
        uint8_t tag = prefix & 0xFC;
        uint8_t size_code = prefix & 0x03;
        size_t size = (size_code == 3) ? 4 : size_code;

        if (i + 1 + size > len) {
            break;
        }

        long value = 0;
        for (size_t b = 0; b < size; b++) {
            value |= ((long)desc[i + 1 + b]) << (8 * b);
        }

        if (tag == 0x04) {          /* Usage Page (global) */
            current_usage_page = value;
        } else if (tag == 0x08) {   /* Usage (local) */
            pending_usage = value;
        } else if (tag == 0xA0) {   /* Collection */
            if (current_usage_page == usage_page && pending_usage == usage) {
                return 1;
            }
        }

        i += 1 + size;
    }
    return 0;
}

/* Lit HID_ID=<bus>:<vid>:<pid> depuis un fichier uevent sysfs. */
static int parse_uevent_ids(const char *uevent_path, uint16_t *vid, uint16_t *pid)
{
    FILE *f = fopen(uevent_path, "r");
    if (!f) return -1;

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        unsigned bus, v, p;
        if (sscanf(line, "HID_ID=%x:%x:%x", &bus, &v, &p) == 3) {
            *vid = (uint16_t)v;
            *pid = (uint16_t)p;
            found = 1;
            break;
        }
    }
    fclose(f);
    return found ? 0 : -1;
}

int hid_raw_find_device(uint16_t vid, uint16_t pid,
                         uint16_t usage_page, uint16_t usage,
                         char *out_path, size_t out_path_len)
{
    DIR *dir = opendir(SYSFS_HIDRAW_DIR);
    if (!dir) return -1;

    struct dirent *entry;
    int result = -1;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        char uevent_path[512];
        snprintf(uevent_path, sizeof(uevent_path), "%s/%s/device/uevent",
                  SYSFS_HIDRAW_DIR, entry->d_name);

        uint16_t found_vid, found_pid;
        if (parse_uevent_ids(uevent_path, &found_vid, &found_pid) != 0) continue;
        if (found_vid != vid || found_pid != pid) continue;

        char desc_path[512];
        snprintf(desc_path, sizeof(desc_path), "%s/%s/device/report_descriptor",
                  SYSFS_HIDRAW_DIR, entry->d_name);

        FILE *f = fopen(desc_path, "rb");
        if (!f) continue;

        uint8_t desc[4096];
        size_t n = fread(desc, 1, sizeof(desc), f);
        fclose(f);

        if (descriptor_matches(desc, n, usage_page, usage)) {
            snprintf(out_path, out_path_len, "/dev/%s", entry->d_name);
            result = 0;
            break;
        }
    }

    closedir(dir);
    return result;
}

int hid_raw_send_command(const char *device_path,
                          const uint8_t *payload, size_t payload_len,
                          uint8_t *response, int timeout_ms)
{
    if (payload_len > HID_RAW_REPORT_LENGTH) {
        errno = EINVAL;
        return -1;
    }

    uint8_t padded[HID_RAW_REPORT_LENGTH] = {0};
    memcpy(padded, payload, payload_len);

    int fd = open(device_path, O_RDWR);
    if (fd < 0) return -1;

    ssize_t written = write(fd, padded, sizeof(padded));
    if (written < 0) {
        int saved_errno = errno;
        close(fd);
        errno = saved_errno;
        return -1;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ready = select(fd + 1, &readfds, NULL, NULL, &tv);
    if (ready <= 0) {
        close(fd);
        return -2; /* timeout ou erreur select */
    }

    ssize_t n = read(fd, response, HID_RAW_REPORT_LENGTH);
    close(fd);
    return (n < 0) ? -1 : 0;
}
