# RATKBD-Linux

> **A native, high-performance Linux configuration suite for the MAD60HE Hall effect keyboard and the VXE R1 Pro mouse.**


[![Linux](https://img.shields.io/badge/Platform-Linux-blue?logo=linux)](https://www.linux.org)
[![C/C++](https://img.shields.io/badge/Language-C%2FC++-orange?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License: Proprietary](https://img.shields.io/badge/License-Proprietary-red.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-success)]()
[![Python](https://img.shields.io/badge/Language-pyhthon-purple?logo=python)](https://fr.wikipedia.org/wiki/Python)
![stars](https://img.shields.io/badge/stars-0-yellow?style=flat-square&logo=github)

---

## 🚀 Overview

**RATKBD-Linux** is an ultra-lightweight, dependency-free native C/C++ utility designed to unlock the full potential of your high-performance peripherals under Linux, eliminating the need for proprietary web configurators, Electron wrappers, or cloud services.

Through complete reverse engineering of the proprietary HID protocols used by this hardware, RATKBD-Linux gives you instant access to your settings directly from your terminal or via a minimalist native backend.

### Supported Hardware

* **Keyboard:** MAD60HE (Magnetic Hall Effect Switches)
* **Mouse:** VXE R1 Pro (High-Performance Wired/Wireless Gaming Mouse)

---

## ✨ Features

### ⌨️ MAD60HE Keyboard

* **Per-Key RGB Lighting:** Full control over lighting zones, custom animations, and brightness profiles.
* **Dynamic Actuation Points:** Fine-tune key activation distances on the fly.
* **Rapid Trigger (RT):** Ultra-responsive key release triggers for maximum competitive advantage in gaming.
* **Profile Management:** Save and switch between multiple hardware configurations instantly.

### 🖱️ VXE R1 Pro Mouse

* **DPI Stages:** Custom DPI levels and precise stage switching.
* **Polling Rate Adjustment:** Fine control over frequencies to optimize latency.
* **Macro Assignment:** Bind and execute key or button sequences natively.
* **Battery & Status Tracking:** Real-time telemetry reports directly from the firmware.

---

## 🛠️ Tech Stack & Architecture

* **Language:** Pure C & C++ (compiled for maximum efficiency and minimal memory footprint).
* **Communication:** Direct raw HID communication (`libusb` / kernel HID interface) bypassing heavy abstraction layers.
* **Zero Bloat:** No Electron, no Node.js, zero browser dependencies—just fast, native execution on Linux.

---

## 📦 Installation

### Prerequisites

Make sure you have the compilation tools and development headers installed on your distribution:

* **Arch Linux / CachyOS:**
```bash
sudo pacman -S base-devel cmake libusb

```


* **Debian / Ubuntu / Mint:**
```bash
sudo apt install build-essential cmake libusb-1.0-0-dev

```



### Building from Source

```bash
# Clone the repository
git clone https://github.com/snayzy/RATKBD-Linux.git
cd RATKBD-Linux

# Create build directory and compile
mkdir build && cd build
cmake ..
make -j$(nproc)

```

### 🔌 Udev Rules Configuration

Since this tool communicates directly with raw HID endpoints, you need the appropriate permissions to access devices without running as root.

1. Copy the provided udev rules file:
```bash
sudo cp 99-openmad.rules /etc/udev/rules.d/

```


2. Reload and apply the udev rules:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger

```



---

## ⚡ Usage Examples

### Configure Keyboard Actuation and Rapid Trigger

```bash
# Set global actuation point to 0.2 mm with Rapid Trigger enabled
ratkbd-cli keyboard --actuation 0.2 --rapid-trigger on

```

### Adjust Mouse DPI and Polling Rate

```bash
# Configure the VXE R1 Pro to 1600 DPI and a 1000Hz polling rate
ratkbd-cli mouse --dpi 1600 --polling 1000

```

---

## 🤝 Contributing

Contributions, reverse-engineering notes, protocol dumps, and pull requests are very welcome! If you own compatible hardware or want to help extend support to other magnetic switches or gaming mice, feel free to open an issue or a PR.

---

## 📜 License

Copyright (c) 2026 SNAYZY (N0CTRACE)

All rights reserved.

This source code is made available for viewing purposes only. No permission is granted to copy, modify, merge, publish, distribute, sublicense, and/or sell copies of this software, in whole or in part, without prior written permission from the copyright holder.
