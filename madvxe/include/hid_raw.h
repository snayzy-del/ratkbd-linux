#ifndef HID_RAW_H
#define HID_RAW_H

#include <stddef.h>
#include <stdint.h>

#define HID_RAW_REPORT_LENGTH 32
#define HID_RAW_PATH_MAX 64

/* Trouve le /dev/hidrawX correspondant au VID:PID et à l'interface
 * identifiée par (usage_page, usage) dans son report descriptor.
 * Retourne 0 et remplit out_path en cas de succès, -1 sinon. */
int hid_raw_find_device(uint16_t vid, uint16_t pid,
                         uint16_t usage_page, uint16_t usage,
                         char *out_path, size_t out_path_len);

/* Ouvre le device, envoie payload (complété à HID_RAW_REPORT_LENGTH
 * octets avec des zéros), lit la réponse dans response (buffer d'au
 * moins HID_RAW_REPORT_LENGTH octets). timeout_ms = délai max d'attente
 * de la réponse. Retourne 0 en cas de succès, -1 en cas d'erreur
 * (errno positionné), -2 en cas de timeout. */
int hid_raw_send_command(const char *device_path,
                          const uint8_t *payload, size_t payload_len,
                          uint8_t *response, int timeout_ms);

#endif /* HID_RAW_H */
