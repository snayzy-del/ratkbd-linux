#ifndef HID_RAW_H
#define HID_RAW_H

#include <stddef.h>
#include <stdint.h>

#define HID_RAW_REPORT_LENGTH 32
#define HID_RAW_PATH_MAX 64

int hid_raw_find_device(uint16_t vid, uint16_t pid,
                         uint16_t usage_page, uint16_t usage,
                         char *out_path, size_t out_path_len);


int hid_raw_send_command(const char *device_path,
                          const uint8_t *payload, size_t payload_len,
                          uint8_t *response, int timeout_ms);

#endif 
