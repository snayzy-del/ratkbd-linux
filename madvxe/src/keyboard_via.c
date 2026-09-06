#include "keyboard_via.h"
#include "hid_raw.h"

#include <string.h>

#define TIMEOUT_MS 1000

int kbd_open(kbd_handle_t *kbd)
{
    return hid_raw_find_device(KBD_VID, KBD_PID,
                                KBD_RAW_HID_USAGE_PAGE, KBD_RAW_HID_USAGE,
                                kbd->device_path, sizeof(kbd->device_path));
}

int kbd_get_protocol_version(const kbd_handle_t *kbd, uint16_t *version)
{
    uint8_t payload[1] = {0x01};
    uint8_t response[HID_RAW_REPORT_LENGTH];

    int rc = hid_raw_send_command(kbd->device_path, payload, sizeof(payload),
                                   response, TIMEOUT_MS);
    if (rc != 0) return rc;

    *version = ((uint16_t)response[1] << 8) | response[2];
    return 0;
}

int kbd_set_rgb_all(const kbd_handle_t *kbd, uint8_t brightness,
                     uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t payload[9] = {0x07, 0x41, 0x02, 0x00, brightness, r, g, b, 0xd2};
    uint8_t response[HID_RAW_REPORT_LENGTH];

    return hid_raw_send_command(kbd->device_path, payload, sizeof(payload),
                                 response, TIMEOUT_MS);
}

int kbd_set_actuation_point(const kbd_handle_t *kbd, uint16_t hundredths_mm)
{
    uint8_t hi = (uint8_t)(hundredths_mm >> 8);
    uint8_t lo = (uint8_t)(hundredths_mm & 0xFF);

    uint8_t payload[17] = {
        0x03, 0x96, 0x0d, 0x00, 0x00, 0x00, 0x42, 0x0b, 0x02,
        hi, lo, hi, lo, hi, lo, hi, lo
    };
    uint8_t response[HID_RAW_REPORT_LENGTH];

    return hid_raw_send_command(kbd->device_path, payload, sizeof(payload),
                                 response, TIMEOUT_MS);
}

int kbd_set_rapid_trigger(const kbd_handle_t *kbd, int enabled,
                           uint16_t trigger_hundredths_mm,
                           uint16_t reset_hundredths_mm)
{
    uint8_t flag = enabled ? 0x01 : 0x00;
    uint8_t t_hi = (uint8_t)(trigger_hundredths_mm >> 8);
    uint8_t t_lo = (uint8_t)(trigger_hundredths_mm & 0xFF);
    uint8_t r_hi = (uint8_t)(reset_hundredths_mm >> 8);
    uint8_t r_lo = (uint8_t)(reset_hundredths_mm & 0xFF);

    uint8_t payload[19] = {
        0x03, 0x96, 0x0e, 0x00, 0x00, 0x00, 0x44, 0x04, 0x02,
        flag, t_hi, t_lo, r_hi, r_lo,
        flag, t_hi, t_lo, r_hi, r_lo
    };
    uint8_t response[HID_RAW_REPORT_LENGTH];

    return hid_raw_send_command(kbd->device_path, payload, sizeof(payload),
                                 response, TIMEOUT_MS);
}

int kbd_set_dead_zone(const kbd_handle_t *kbd,
                       uint16_t press_hundredths_mm,
                       uint16_t lift_hundredths_mm)
{

    uint8_t p_lo = (uint8_t)(press_hundredths_mm & 0xFF);
    uint8_t p_hi = (uint8_t)(press_hundredths_mm >> 8);
    uint8_t l_lo = (uint8_t)(lift_hundredths_mm & 0xFF);
    uint8_t l_hi = (uint8_t)(lift_hundredths_mm >> 8);

    uint8_t payload[8] = {
        0x03, 0x96, 0x0b, 0x00,
        p_lo, p_hi, l_lo, l_hi
    };
    uint8_t response[HID_RAW_REPORT_LENGTH];

    return hid_raw_send_command(kbd->device_path, payload, sizeof(payload),
                                 response, TIMEOUT_MS);
}
