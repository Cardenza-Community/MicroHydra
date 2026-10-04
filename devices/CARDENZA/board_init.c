#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "cardenza_hal.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "nvs_flash.h"

void cardenza_board_startup(void) {
    esp_log_level_set("Cardenza", ESP_LOG_INFO);
    cardenza_hal_led_off();
    ESP_LOGI("Cardenza", "MicroHydra startup; initializing ES8156");
    bool ready = cardenza_hal_init(32, 16);
    ESP_LOGI("Cardenza", "ES8156 setup: %s", ready ? "OK" : "FAILED");

    // The Launcher shares NVS. A full erase must never be an app recovery action.
    esp_err_t result = nvs_flash_init();
    if (result != ESP_OK) ESP_LOGE("Cardenza", "NVS init failed: %s; preserved", esp_err_to_name(result));
    esp_flash_get_physical_size(NULL, &esp_flash_default_chip->size);
    if (!esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "vfs")) {
        ESP_LOGE("Cardenza", "Missing dedicated vfs partition; refusing to create one over other apps");
    }
}

esp_err_t __wrap_nvs_flash_erase(void) {
    ESP_LOGE("Cardenza", "Refusing whole shared NVS erase");
    return ESP_ERR_NOT_SUPPORTED;
}
