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


#define SENSOR_GPIO GPIO_NUM_36

void app_main(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << SENSOR_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    while (1) {
        int val = gpio_get_level(SENSOR_GPIO);
        printf("Sensor state: %d\n", val);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}