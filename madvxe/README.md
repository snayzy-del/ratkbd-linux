# madvxe

Logiciel Linux natif (C) pour le clavier MAD60HE (Hall Effect, protocole VIA)
et la souris VXE R1 Pro. Protocole reverse-engineered, sans dépendance au
configurateur web officiel.

## Build

```bash
mkdir -p build && cd build
cmake ..
make
```

Ou directement sans CMake :
```bash
gcc -Wall -Wextra -Iinclude -o build/madctl src/main.c src/hid_raw.c src/keyboard_via.c
```

## Permissions

Copier la règle udev pour éviter d'avoir à lancer en `sudo` :
```bash
sudo cp udev/99-madvxe.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## Usage

```bash
./build/madctl kbd version
./build/madctl kbd set-rgb 255 255 0 0        # rouge pur, luminosité max
./build/madctl kbd set-actuation 1.50         # point d'activation à 1.50mm
./build/madctl kbd set-rt on 0.50 0.64        # Rapid Trigger ON, trigger 0.50mm, reset 0.64mm
./build/madctl kbd set-deadzone 0.16 0.16     # dead zone press/lift à 0.16mm
```

## Protocole clavier (MAD60HE)

Interface Raw HID : Usage Page `0xFF60` / Usage `0x61` (standard VIA/QMK, v9).

| Fonction | Header | Encodage |
|---|---|---|
| GET_PROTOCOL_VERSION | `01` | standard VIA |
| RGB global (custom) | `07 41 02 00 <brightness> <r> <g> <b> d2` | brut, 1 octet/canal |
| Point d'activation | `03 96 0d 00 00 00 42 0b 02` + 4×valeur | big-endian, centièmes de mm |
| Rapid Trigger (toggle+sens.) | `03 96 0e 00 00 00 44 04 02` + 2×[flag,trigger,reset] | big-endian, centièmes de mm |
| Dead Zone press/lift | `03 96 0b 00` + [press,lift] | **little-endian**, centièmes de mm |

RGB par touche individuelle : non résolu (mis en pause, cf. TODO).

## TODO

- [ ] RGB par touche individuelle (nécessite d'isoler le paquet d'adressage de touche, nos captures se sont révélées incohérentes)
- [ ] Protocole souris VXE R1 Pro (reconnaissance pas encore commencée)
- [ ] Moteur de macros (libevdev/uinput)
- [ ] CLI plus complète / GUI
