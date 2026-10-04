#define MICROPY_HW_BOARD_NAME "Cardenza"
#define MICROPY_HW_MCU_NAME "ESP32S3"
#define MICROPY_HW_ENABLE_UART_REPL (0)
#define MICROPY_HW_I2C0_SCL (1)
#define MICROPY_HW_I2C0_SDA (2)
void cardenza_board_startup(void);
#define MICROPY_BOARD_STARTUP cardenza_board_startup
