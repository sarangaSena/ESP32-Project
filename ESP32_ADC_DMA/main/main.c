/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define ADC_WIDTH    ADC_WIDTH_BIT_12  // ADC resolution (12-bit)
#define ADC_ATTEN    ADC_ATTEN_DB_0   // 0dB attenuation for input range of 0-1.1V
#define ADC_CHANNEL  ADC1_CHANNEL_0  // GPIO34 (ADC1_CHANNEL_0)

// Buffer to store ADC readings
#define BUFFER_SIZE  1024
int adc_buffer[BUFFER_SIZE];

void app_main(void) {
    // ADC configuration
    esp_err_t err;
    
    // Configure ADC width (resolution)
    err = adc1_config_width(ADC_WIDTH);
    if (err != ESP_OK) {
        ESP_LOGE("ADC", "Failed to configure ADC width");
        return;
    }

    // Configure ADC channel attenuation
    err = adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
    if (err != ESP_OK) {
        ESP_LOGE("ADC", "Failed to configure ADC attenuation");
        return;
    }

    // Set up DMA (simulated in this example)
    // Here, we're simulating DMA with a simple buffer. For real-world DMA, the hardware would manage memory automatically.
    
    while (1) {
        // In a real DMA setup, this would be triggered automatically
        for (int i = 0; i < BUFFER_SIZE; i++) {
            adc_buffer[i] = adc1_get_raw(ADC_CHANNEL);
        }

        // Process the buffer here, e.g., send the data to the network or store in non-volatile memory
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("ADC[%d]: %d\n", i, adc_buffer[i]);
        }

        // Add a delay between reads
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1-second delay
    }
}