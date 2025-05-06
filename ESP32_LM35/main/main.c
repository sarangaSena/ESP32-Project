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

#define ADC_CHANNEL     ADC1_CHANNEL_0  // GPIO34 (ADC1_CHANNEL_0)
#define ADC_WIDTH       ADC_WIDTH_BIT_12 // ADC resolution (12 bits, 0-4095)
#define ADC_ATTEN       ADC_ATTEN_DB_0  // Attenuation (0-1.1V)

void app_main(void) {
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

    while (1) {
        // Read the raw ADC value from the LM35
        int adc_value = adc1_get_raw(ADC_CHANNEL);
        
        // Convert the raw ADC value to voltage (LM35 output in mV)
        // ESP32 ADC resolution is 12-bit (0-4095)
        // The reference voltage is 3.3V, so the ADC value corresponds to the voltage (0-3.3V)
        // LM35 output is 10mV per degree Celsius, so 1°C = 10mV
        float voltage = (adc_value / 4095.0) * 3.3;  // Convert ADC value to voltage
        float temperature = voltage * 100;            // Convert voltage to temperature (10mV per °C)

        // Print the temperature value
        printf("Temperature: %.2f°C\n", temperature);

        // Add a delay before the next reading
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1-second delay
    }
}