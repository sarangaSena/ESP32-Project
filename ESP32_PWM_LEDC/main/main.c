/*
 * GccApplication1.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include "driver/ledc.h"
#include "esp_system.h"
#include "esp_log.h"

#define PWM_FREQUENCY    5000  // 5 kHz
#define PWM_RESOLUTION    LEDC_TIMER_13_BIT  // Resolution (13 bits for a range of 0 to 8191)
#define PWM_TIMER         LEDC_TIMER_0
#define PWM_CHANNEL       LEDC_CHANNEL_0
#define PWM_GPIO          18  // GPIO pin where the PWM signal will be output

void app_main(void) {
    // Configure LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = PWM_RESOLUTION,  // Resolution of PWM duty
        .freq_hz = PWM_FREQUENCY,          // Frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE, // High speed mode
        .timer_num = PWM_TIMER             // Timer number
    };
    ledc_timer_config(&ledc_timer); // Initialize the timer

    // Configure LEDC channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num = PWM_GPIO,              // GPIO pin to output PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Speed mode
        .channel = PWM_CHANNEL,            // PWM channel
        .intr_type = LEDC_INTR_DISABLE,    // Disable interrupts
        .timer_sel = PWM_TIMER,            // Select timer
        .duty = 0,                         // Initial duty cycle
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel); // Initialize the channel

    // Set PWM duty cycle
    int duty_cycle = 4095;  // Duty cycle (full brightness for 13-bit resolution)
    
    // Start PWM signal
    while (1) {
        for (int duty = 0; duty < duty_cycle; duty++) {
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}