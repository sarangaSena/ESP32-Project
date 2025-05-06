/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/touch_pad.h"

static const char *TAG = "TOUCH";

void app_main(void)
{
    // Initialize touch pad peripheral
    ESP_ERROR_CHECK(touch_pad_init());

    // Configure GPIO4 (TOUCH0)
    ESP_ERROR_CHECK(touch_pad_config(TOUCH_PAD_NUM0, 0)); // 0 = no threshold yet

    // Calibrate threshold based on raw value
    uint16_t touch_value;
    ESP_LOGI(TAG, "Calibrating...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    touch_pad_read(TOUCH_PAD_NUM0, &touch_value);
    uint16_t threshold = touch_value * 0.8;  // 80% of idle value
    ESP_ERROR_CHECK(touch_pad_set_thresh(TOUCH_PAD_NUM0, threshold));
    ESP_LOGI(TAG, "Threshold set to %d", threshold);

    while (1) {
        touch_pad_read(TOUCH_PAD_NUM0, &touch_value);
        ESP_LOGI(TAG, "Raw Touch Value: %d", touch_value);
        if (touch_value < threshold) {
            ESP_LOGI(TAG, "Touch detected!");
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}