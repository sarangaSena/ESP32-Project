/*
 * GccApplication1.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include "esp_log.h"
#include "driver/adc.h"
#include "driver/comparator.h"
#include "esp_intr_alloc.h"

#define TAG "Comparator Example"

// Comparator interrupt service routine
static void IRAM_ATTR comparator_isr_handler(void* arg) {
    ESP_LOGI(TAG, "Comparator interrupt triggered!");

    // You can handle comparator events here, such as reading ADC values
}

void app_main(void) {
    // Initialize the comparator
    comparator_config_t cmp_config = {
        .vref = 0, // Set the reference voltage for comparison, for example 0 to use default
        .cmp_in = COMPARATOR_INPUT_GPIO, // Use GPIO as the comparator input
        .threshold = 2048, // Set the threshold (this could be a value between 0 and 4095 for a 12-bit ADC)
    };

    // Initialize the comparator hardware
    ESP_ERROR_CHECK(comparator_init(&cmp_config));

    // Install the interrupt service for the comparator
    ESP_ERROR_CHECK(esp_intr_alloc(ETS_COMPARATOR_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, comparator_isr_handler, NULL, NULL));

    // Enable the comparator interrupt
    ESP_ERROR_CHECK(comparator_enable_interrupts());

    ESP_LOGI(TAG, "Comparator initialized and interrupt enabled!");

    // Main loop
    while (1) {
        // Application logic, such as checking GPIO states or other conditions
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for a second
    }
}