set(MICROPY_FROZEN_MANIFEST ${MICROPY_BOARD_DIR}/manifest.py)
set(IDF_TARGET esp32s3)
set(MICROPY_SOURCE_BOARD ${MICROPY_BOARD_DIR}/board_init.c)
set(SDKCONFIG_DEFAULTS
    boards/sdkconfig.base
    ${SDKCONFIG_IDF_VERSION_SPECIFIC}
    boards/sdkconfig.usb
    boards/sdkconfig.ble
    boards/MICROHYDRA_GENERIC_S3/sdkconfig.board
    ${MICROPY_BOARD_DIR}/sdkconfig.cardenza
)

# Never erase the shared Launcher NVS during MicroPython recovery.
add_link_options(-Wl,--wrap=nvs_flash_erase)
