/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_INPUT_IO_0    GPIO_NUM_0  // Falling edge
#define GPIO_INPUT_IO_1    GPIO_NUM_4  // Rising edge

static const char* TAG = "GPIO_INT";

static xQueueHandle gpio_evt_queue = NULL;

// ISR Handler (IRAM-safe)
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Task to handle GPIO events
static void gpio_task(void* arg) {
    uint32_t io_num;
    while (1) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Interrupt on GPIO %d", io_num);
        }
    }
}

void app_main(void) {
    // Configure both GPIOs
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pin_bit_mask = (1ULL << GPIO_INPUT_IO_0) | (1ULL << GPIO_INPUT_IO_1)
    };
    gpio_config(&io_conf);

    // Set individual interrupt types
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_NEGEDGE);  // Falling edge
    gpio_set_intr_type(GPIO_INPUT_IO_1, GPIO_INTR_POSEDGE);  // Rising edge

    // Create a queue to handle gpio events
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Start gpio task
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    // Install GPIO ISR service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // Hook ISR handler for each GPIO
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    ESP_LOGI(TAG, "GPIO interrupt example started");
}