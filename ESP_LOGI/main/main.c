/*
 * GccApplication1.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "esp_log.h"

// Define a TAG for your module or function
static const char *TAG = "my_example_app";

void app_main(void) {
    ESP_LOGI(TAG, "This is an info log message.");
    int value = 42;
    ESP_LOGI(TAG, "The value of the variable is: %d", value);
}