#ifndef KEYBOARD_VIA_H
#define KEYBOARD_VIA_H

#include <stdint.h>

/* VID/PID et interface Raw HID (Usage Page 0xFF60 / Usage 0x61,
 * standard VIA/QMK) du MAD60HE -- confirmes via lsusb + usbhid-dump. */
#define KBD_VID 0x373b
#define KBD_PID 0x1054
#define KBD_RAW_HID_USAGE_PAGE 0xFF60
#define KBD_RAW_HID_USAGE 0x61

typedef struct {
    char device_path[64]; /* /dev/hidrawX résolu une fois au démarrage */
} kbd_handle_t;

/* Résout le device et remplit kbd. Retourne 0 en cas de succès. */
int kbd_open(kbd_handle_t *kbd);

/* GET_PROTOCOL_VERSION (VIA standard, command 0x01).
 * Retourne 0 et remplit *version en cas de succès. */
int kbd_get_protocol_version(const kbd_handle_t *kbd, uint16_t *version);

/* RGB global (commande custom 07 41 02 00), confirmee en ecriture :
 * met toutes les touches a la couleur (r,g,b) avec la luminosite
 * donnee (0-255). */
int kbd_set_rgb_all(const kbd_handle_t *kbd, uint8_t brightness,
                     uint8_t r, uint8_t g, uint8_t b);

/* Point d'activation global, en centiemes de mm (ex: 150 = 1.50mm).
 * Commande 03 96 0d 00 00 00 42 0b 02, valeur repetee x4, big-endian. */
int kbd_set_actuation_point(const kbd_handle_t *kbd, uint16_t hundredths_mm);

/* Active/desactive le Rapid Trigger et regle ses sensibilites
 * (trigger = distance de declenchement, reset = distance de
 * relachement), en centiemes de mm.
 * Commande 03 96 0e 00 00 00 44 04 02, big-endian. */
int kbd_set_rapid_trigger(const kbd_handle_t *kbd, int enabled,
                           uint16_t trigger_hundredths_mm,
                           uint16_t reset_hundredths_mm);

/* Dead zone Press (trigger) / Lift (reset), en centiemes de mm.
 * Commande 03 96 0b 00, little-endian -- attention, ordre d'octets
 * different des deux fonctions precedentes. */
int kbd_set_dead_zone(const kbd_handle_t *kbd,
                       uint16_t press_hundredths_mm,
                       uint16_t lift_hundredths_mm);

#endif /* KEYBOARD_VIA_H */
