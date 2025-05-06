/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include "driver/dac.h"
#include "esp_log.h"
#include <math.h>

#define PI 3.14159265358979323846
#define SINE_RESOLUTION 256
#define DAC_CHANNEL DAC_CHANNEL_1

void generate_sine_wave(void)
{
    for (int i = 0; i < SINE_RESOLUTION; i++)
    {
        int value = (int)((sin(2 * PI * i / SINE_RESOLUTION) + 1) * 127.5);  // Range from 0 to 255
        dac_output_voltage(DAC_CHANNEL, value);  // Output to DAC
        vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay to control the speed of the waveform
    }
}

void app_main(void)
{
    dac_output_enable(DAC_CHANNEL);  // Enable DAC output
    while (1)
    {
        generate_sine_wave();  // Continuously generate sine wave
    }
}