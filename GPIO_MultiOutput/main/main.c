/*
 * .c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define LED_GPIOS (1ULL << GPIO_NUM_2|1ULL << GPIO_NUM_3|1ULL << GPIO_NUM_4)

void app_main(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = LED_GPIOS,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    while (1) {
        gpio_set_level(GPIO_NUM_2, 1); // Turn on
        gpio_set_level(GPIO_NUM_3, 1); 
        gpio_set_level(GPIO_NUM_4, 1); 
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_2, 0); // Turn off
        gpio_set_level(GPIO_NUM_3, 0); 
        gpio_set_level(GPIO_NUM_4, 0); 
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}