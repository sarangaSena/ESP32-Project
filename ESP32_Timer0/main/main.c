/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"

// Timer interrupt handler
void IRAM_ATTR timer_isr_handler(void* arg)
{
    // Clear the interrupt
    TIMERG0.int_clr_timers.t0 = 1;

    // Toggle an LED or print a message
    printf("Timer interrupt triggered!\n");
}

void app_main()
{
    // Timer configuration
    timer_config_t config = {
        .divider = 80,                 // Set the timer clock divider (80 means 1 MHz)
        .counter_dir = TIMER_COUNT_UP, // Count up (you can change to down)
        .counter_en = TIMER_START,     // Start the timer immediately
        .auto_reload = true,           // Enable auto-reload
        .alarm_en = TIMER_ALARM_EN,    // Enable alarm
        .intr_type = TIMER_INTR_LEVEL, // Set interrupt type
        .alarm_value = 1000000        // Set the alarm value (1 second)
    };
    
    // Initialize the timer
    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // Set the ISR handler for the timer
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_isr_handler, NULL, ESP_INTR_FLAG_LEVEL1, NULL);

    // Enable the timer interrupt
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    // Start the timer
    timer_start(TIMER_GROUP_0, TIMER_0);

    // Infinite loop to keep the application running
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to prevent the app from exiting
    }
}