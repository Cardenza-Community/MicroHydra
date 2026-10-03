# Cardenza support

`devices/CARDENZA` preserves the original Cardputer matrix-keyboard and display
layout, uses internal RAM only, and omits nonexistent battery/charging UI.
The native startup hook verifies ES8156 on SDA2/SCL1, configures Philips I2S
16-bit/32fs and holds GPIO21 high. A codec failure is explicitly logged.
PDM microphone pins DATA46/CLK43 remain documented; this pinned MicroPython
I2S driver does not implement PDM input, so recording is not promised.

## Build

The repository pins MicroPython 647c8b96c and ESP-IDF 5.4.2. In Linux/WSL:

```sh
git submodule update --init --recursive
. esp-idf/export.sh
python3 tools/create_frozen_folders.py
python3 tools/parse_files.py --frozen
make -C MicroPython/ports/esp32 BOARD=CARDENZA -j4
python3 tools/test_cardenza.py
```

Install only `MicroPython/ports/esp32/build-CARDENZA/micropython.bin` through
Software Launcher. Preserve the existing bootloader, partition table and NVS.
A dedicated 4 KiB-aligned `vfs` data partition (subtype `0x81`) is required.
The controlled layout uses 512 KiB; its first block must be erased for initial
LittleFS2 formatting. The app refuses to create a missing partition dynamically.
Whole shared-NVS erase is rejected by the target linker wrapper, including
MicroPython's own NVS recovery path. Normal NVS namespace operations remain.
UART REPL is disabled to avoid GPIO43; TinyUSB REPL remains enabled.

The original GPL-3.0 license is retained. The vendored HAL license is
`devices/CARDENZA/HAL-LICENSE`. A build or host check is not proof of physical
keyboard, display or audio operation. No hardware flashing is part of CI.
