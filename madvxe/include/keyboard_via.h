#ifndef KEYBOARD_VIA_H
#define KEYBOARD_VIA_H

#include <stdint.h>

#define KBD_VID 0x373b
#define KBD_PID 0x1054
#define KBD_RAW_HID_USAGE_PAGE 0xFF60
#define KBD_RAW_HID_USAGE 0x61

typedef struct {
    char device_path[64]; 
} kbd_handle_t;

int kbd_open(kbd_handle_t *kbd);

int kbd_get_protocol_version(const kbd_handle_t *kbd, uint16_t *version);

int kbd_set_rgb_all(const kbd_handle_t *kbd, uint8_t brightness,
                     uint8_t r, uint8_t g, uint8_t b);

int kbd_set_actuation_point(const kbd_handle_t *kbd, uint16_t hundredths_mm);


int kbd_set_rapid_trigger(const kbd_handle_t *kbd, int enabled,
                           uint16_t trigger_hundredths_mm,
                           uint16_t reset_hundredths_mm);

int kbd_set_dead_zone(const kbd_handle_t *kbd,
                       uint16_t press_hundredths_mm,
                       uint16_t lift_hundredths_mm);

#endif
