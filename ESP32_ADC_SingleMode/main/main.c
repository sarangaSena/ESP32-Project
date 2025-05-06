/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_log.h"

#define ADC1_CHANNEL ADC1_CHANNEL_0  // Use GPIO34 (ADC1_CHANNEL_0)
#define ADC_WIDTH ADC_WIDTH_BIT_12    // Set ADC width to 12 bits (0-4095)
#define ADC_ATTEN ADC_ATTEN_DB_0     // Set the input range for ADC

void app_main(void) {
    // Configure ADC width and attenuation
    esp_err_t err;

    err = adc1_config_width(ADC_WIDTH);  // Set ADC width
    if (err != ESP_OK) {
        ESP_LOGE("ADC", "Failed to configure ADC width");
        return;
    }

    err = adc1_config_channel_atten(ADC1_CHANNEL, ADC_ATTEN);  // Set ADC attenuation
    if (err != ESP_OK) {
        ESP_LOGE("ADC", "Failed to configure ADC attenuation");
        return;
    }

    while (1) {
        // Read ADC value
        int adc_value = adc1_get_raw(ADC1_CHANNEL);
        
        // Print the ADC value
        printf("ADC Value: %d\n", adc_value);

        // Delay for a short time before reading again
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1 second delay
    }
}