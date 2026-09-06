#include "keyboard_via.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *prog)
{
    fprintf(stderr,
        "Usage:\n"
        "  %s kbd version\n"
        "  %s kbd set-rgb <brightness 0-255> <r> <g> <b>\n"
        "  %s kbd set-actuation <mm ex: 1.50>\n"
        "  %s kbd set-rt <on|off> <trigger_mm> <reset_mm>\n"
        "  %s kbd set-deadzone <press_mm> <lift_mm>\n",
        prog, prog, prog, prog, prog);
}

static uint16_t mm_to_hundredths(double mm)
{
    return (uint16_t)(mm * 100.0 + 0.5);
}

int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "kbd") != 0) {
        print_usage(argv[0]);
        return 1;
    }

    kbd_handle_t kbd;
    if (kbd_open(&kbd) != 0) {
        fprintf(stderr, "Clavier introuvable (verifie VID/PID, permissions udev, cable).\n");
        return 1;
    }

    const char *action = argv[2];

    if (strcmp(action, "version") == 0) {
        uint16_t version;
        if (kbd_get_protocol_version(&kbd, &version) != 0) {
            fprintf(stderr, "Echec de la lecture de version.\n");
            return 1;
        }
        printf("Protocole VIA v%u\n", version);

    } else if (strcmp(action, "set-rgb") == 0 && argc == 7) {
        uint8_t brightness = (uint8_t)atoi(argv[3]);
        uint8_t r = (uint8_t)atoi(argv[4]);
        uint8_t g = (uint8_t)atoi(argv[5]);
        uint8_t b = (uint8_t)atoi(argv[6]);
        if (kbd_set_rgb_all(&kbd, brightness, r, g, b) != 0) {
            fprintf(stderr, "Echec set-rgb.\n");
            return 1;
        }
        printf("RGB applique : brightness=%u r=%u g=%u b=%u\n", brightness, r, g, b);

    } else if (strcmp(action, "set-actuation") == 0 && argc == 4) {
        double mm = atof(argv[3]);
        if (kbd_set_actuation_point(&kbd, mm_to_hundredths(mm)) != 0) {
            fprintf(stderr, "Echec set-actuation.\n");
            return 1;
        }
        printf("Point d'activation regle a %.2fmm\n", mm);

    } else if (strcmp(action, "set-rt") == 0 && argc == 6) {
        int enabled = (strcmp(argv[3], "on") == 0);
        double trigger_mm = atof(argv[4]);
        double reset_mm = atof(argv[5]);
        if (kbd_set_rapid_trigger(&kbd, enabled,
                                   mm_to_hundredths(trigger_mm),
                                   mm_to_hundredths(reset_mm)) != 0) {
            fprintf(stderr, "Echec set-rt.\n");
            return 1;
        }
        printf("Rapid Trigger %s : trigger=%.2fmm reset=%.2fmm\n",
               enabled ? "active" : "desactive", trigger_mm, reset_mm);

    } else if (strcmp(action, "set-deadzone") == 0 && argc == 5) {
        double press_mm = atof(argv[3]);
        double lift_mm = atof(argv[4]);
        if (kbd_set_dead_zone(&kbd,
                               mm_to_hundredths(press_mm),
                               mm_to_hundredths(lift_mm)) != 0) {
            fprintf(stderr, "Echec set-deadzone.\n");
            return 1;
        }
        printf("Dead zone reglee : press=%.2fmm lift=%.2fmm\n", press_mm, lift_mm);

    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
