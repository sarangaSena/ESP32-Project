/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define LCD control pins
#define RS_PIN    17  // Register Select (RS)
#define RW_PIN    4   // Read/Write (RW)
#define E_PIN     16  // Enable (E)

// Define LCD data pins
#define D4_PIN    2   // Data line D4
#define D5_PIN    0   // Data line D5
#define D6_PIN    15  // Data line D6
#define D7_PIN    13  // Data line D7

// Define LCD commands
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_HOME  0x02
#define LCD_CMD_ENTRY_MODE_SET 0x04
#define LCD_CMD_DISPLAY_CTRL 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDR 0x40
#define LCD_CMD_SET_DDRAM_ADDR 0x80

// Function prototypes
void lcd_send_command(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_write_string(const char *str);
void lcd_send_nibble(uint8_t nibble);
void lcd_send_byte(uint8_t byte);

void app_main(void)
{
    // Configure GPIO pins for LCD control
    gpio_set_direction(RS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RW_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(E_PIN, GPIO_MODE_OUTPUT);

    // Configure GPIO pins for LCD data lines
    gpio_set_direction(D4_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(D5_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(D6_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(D7_PIN, GPIO_MODE_OUTPUT);

    // Initialize LCD
    lcd_init();

    // Display a message on the LCD
    lcd_write_string("Hello, ESP32!");

    // Loop indefinitely
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Function to send a command to the LCD
void lcd_send_command(uint8_t cmd)
{
    gpio_set_level(RS_PIN, 0);  // RS = 0 for command
    gpio_set_level(RW_PIN, 0);  // RW = 0 for write
    lcd_send_byte(cmd);
}

// Function to send data to the LCD
void lcd_send_data(uint8_t data)
{
    gpio_set_level(RS_PIN, 1);  // RS = 1 for data
    gpio_set_level(RW_PIN, 0);  // RW = 0 for write
    lcd_send_byte(data);
}

// Function to send a nibble (4 bits) to the LCD
void lcd_send_nibble(uint8_t nibble)
{
    gpio_set_level(D4_PIN, (nibble >> 0) & 0x01);
    gpio_set_level(D5_PIN, (nibble >> 1) & 0x01);
    gpio_set_level(D6_PIN, (nibble >> 2) & 0x01);
    gpio_set_level(D7_PIN, (nibble >> 3) & 0x01);
    gpio_set_level(E_PIN, 1);  // Enable the LCD
    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(E_PIN, 0);  // Disable the LCD
}

// Function to send a byte (8 bits) to the LCD
void lcd_send_byte(uint8_t byte)
{
    lcd_send_nibble(byte >> 4);   // Send high nibble
    lcd_send_nibble(byte & 0x0F); // Send low nibble
}

// Function to initialize the LCD
void lcd_init(void)
{
    // Set LCD to 4-bit mode
    lcd_send_command(LCD_CMD_FUNCTION_SET | 0x28); // 4-bit mode, 2-line, 5x8 matrix
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(LCD_CMD_DISPLAY_CTRL | 0x0C); // Display ON, Cursor OFF, Blink OFF
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(LCD_CMD_CLEAR); // Clear display
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(LCD_CMD_ENTRY_MODE_SET | 0x06); // Increment cursor
    vTaskDelay(5 / portTICK_PERIOD_MS);
}

// Function to write a string to the LCD
void lcd_write_string(const char *str)
{
    while (*str) {
        lcd_send_data(*str++);
    }
}