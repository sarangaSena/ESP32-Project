/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"


#define BUTTON_GPIO GPIO_NUM_0

void app_main(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    while (1) {
        int level = gpio_get_level(BUTTON_GPIO);
        ESP_LOGI("BUTTON", "Level: %d", level);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}