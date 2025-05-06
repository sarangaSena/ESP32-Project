/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/adc.h"      // Needed for ADC calibration
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp32/rom/rtc.h"   // Needed for hall_sensor_read()

void app_main(void)
{
    int hall_val;

    while (1) {
        hall_val = hall_sensor_read();  // Read hall sensor
        printf("Hall Sensor Reading: %d\n", hall_val);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}