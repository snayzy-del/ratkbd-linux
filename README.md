# RATKBD-Linux

> **Suite de configuration native et haute performance sous Linux pour le clavier à effet Hall MAD60HE et la souris VXE R1 Pro.**

[![Linux](https://img.shields.io/badge/Platform-Linux-blue?logo=linux)](https://www.linux.org)
[![C/C++](https://img.shields.io/badge/Language-C%2FC++-orange?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License: Proprietary](https://img.shields.io/badge/License-Proprietary-red.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-success)]()
[![Python](https://img.shields.io/badge/Language-pyhthon-purple?logo=python)](https://fr.wikipedia.org/wiki/Python)
![stars](https://img.shields.io/badge/stars-0-yellow?style=flat-square&logo=github)

---

## 🚀 Présentation

**RATKBD-Linux** est un utilitaire natif en C/C++ ultra-léger et sans dépendance, conçu pour libérer tout le potentiel de tes périphériques haute performance sous Linux, sans passer par les configurateurs web propriétaires, les wrappers Electron ou les services cloud.

Grâce à une rétro-ingénierie complète des protocoles HID propriétaires utilisés par ces matériels, OpenMAD-Linux te donne un accès instantané à tes paramètres directement depuis ton terminal ou via un backend natif minimaliste.

### Matériel pris en charge
* **Clavier :** MAD60HE (Switches Magnétiques à Effet Hall)
* **Souris :** VXE R1 Pro (Souris Gamer Haute Performance sans fil/filaire)

---

## ✨ Fonctionnalités

### ⌨️ Clavier MAD60HE
* **Éclairage RGB par touche :** Contrôle total des zones d'éclairage, animations personnalisées et profils de luminosité.
* **Points d'actionnement dynamiques :** Ajuste finement la distance d'activation des touches à la volée.
* **Rapid Trigger (RT) :** Déclenchement ultra-réactif au relâchement des touches pour un avantage compétitif maximal en jeu.
* **Gestion des profils :** Sauvegarde et bascule entre plusieurs configurations matérielles instantanément.

### 🖱️ Souris VXE R1 Pro
* **Paliers DPI :** Niveaux DPI personnalisés et commutation précise des paliers.
* **Ajustement du taux de rapport (Polling Rate) :** Contrôle fin des fréquences pour optimiser la latence.
* **Assignation de macros :** Lie et exécute des séquences de touches ou de boutons de manière native.
* **Suivi de la batterie et de l'état :** Rapports de télémétrie en temps réel directement depuis le firmware.

---

## 🛠️ Stack Technique & Architecture

* **Langage :** Pur C & C++ (compilé pour une efficacité maximale et une empreinte mémoire minime).
* **Communication :** Communication HID brute directe (`libusb` / interface noyau HID) en contournant les couches d'abstraction lourdes.
* **Zéro superflu :** Pas d'Electron, pas de Node.js, aucune dépendance navigateur — juste de l'exécution native et rapide sous Linux.

---

## 📦 Installation

### Prérequis
Assure-toi d'avoir les outils de compilation et les en-têtes de développement installés sur ta distribution :

* **Arch Linux / CachyOS :**
  ```bash
  sudo pacman -S base-devel cmake libusb

    Debian / Ubuntu / Mint :
    Bash

    sudo apt install build-essential cmake libusb-1.0-0-dev

Compilation depuis les sources
Bash

# Cloner le dépôt
git clone [https://github.com/snayzy/RATKBD-Linux.git](https://github.com/snayzy/RATKBD-Linux.git)
cd RATKBD-Linux

# Créer le dossier de build et compiler
mkdir build && cd build
cmake ..
make -j$(nproc)

🔌 Configuration des règles Udev

Puisque cet outil communique directement avec les points de terminaison HID bruts, tu as besoin des permissions appropriées pour accéder aux périphériques sans être obligé de tourner en root.

    Copie le fichier de règles udev fourni :
    Bash

    sudo cp 99-openmad.rules /etc/udev/rules.d/

    Recharge et applique les règles udev :
    Bash

    sudo udevadm control --reload-rules
    sudo udevadm trigger

⚡ Exemples d'utilisation
Configurer l'actionnement et le Rapid Trigger du clavier
Bash

# Définir le point d'actionnement global à 0.2 mm avec le Rapid Trigger activé
openmad-cli keyboard --actuation 0.2 --rapid-trigger on

Ajuster les DPI et le taux de rapport de la souris
Bash

# Configurer la VXE R1 Pro à 1600 DPI et un taux de rapport de 1000Hz
openmad-cli mouse --dpi 1600 --polling 1000

🤝 Contribution

Les contributions, notes de rétro-ingénierie, dumps de protocoles et pull requests sont les bienvenus ! Si tu possèdes un matériel compatible ou que tu souhaites aider à étendre le support à d'autres switches magnétiques ou souris gaming, n'hésite pas à ouvrir une issue ou une PR.
📜 Licence

Copyright (c) 2026 SNAYZY (N0CTRACE)

All rights reserved.

This source code is made available for viewing purposes only. No permission is granted to copy, modify, merge, publish, distribute, sublicense, and/or sell copies of this software, in whole or in part, without prior written permission from the copyright holder.
