/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C configuration
#define I2C_MASTER_SCL_IO 22               /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO 21               /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0           /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0        /*!< I2C master does not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0        /*!< I2C master does not need buffer */

// LCD I2C address (adjust this depending on your LCD module)
#define LCD_ADDR 0x27

// LCD commands
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_HOME 0x02
#define LCD_CMD_ENTRY_MODE_SET 0x04
#define LCD_CMD_DISPLAY_CTRL 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDR 0x40
#define LCD_CMD_SET_DDRAM_ADDR 0x80

// Write a byte to the LCD (either data or command)
esp_err_t lcd_write_byte(i2c_port_t i2c_num, uint8_t data, bool is_data) {
    uint8_t buf[4];
    buf[0] = data;
    if (is_data) {
        buf[0] |= 0x01; // Set RS bit for data
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_to_device(cmd, LCD_ADDR, buf, 1, 1000 / portTICK_RATE_MS);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return err;
}

// Initialize the LCD
esp_err_t lcd_init(i2c_port_t i2c_num) {
    // Set function to 4-bit mode
    lcd_write_byte(i2c_num, LCD_CMD_FUNCTION_SET | 0x28, false); // 4-bit mode, 2-line display, 5x8 dots
    vTaskDelay(5 / portTICK_PERIOD_MS);

    // Display on, cursor off
    lcd_write_byte(i2c_num, LCD_CMD_DISPLAY_CTRL | 0x0C, false);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    // Clear the display
    lcd_write_byte(i2c_num, LCD_CMD_CLEAR, false);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    // Set entry mode (cursor moves right, no display shift)
    lcd_write_byte(i2c_num, LCD_CMD_ENTRY_MODE_SET | 0x06, false);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    return ESP_OK;
}

// Write a string to the LCD
esp_err_t lcd_write_string(i2c_port_t i2c_num, const char *str) {
    while (*str) {
        lcd_write_byte(i2c_num, *str++, true);
    }
    return ESP_OK;
}

void app_main(void) {
    // Configure I2C master
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0));

    // Initialize LCD
    ESP_ERROR_CHECK(lcd_init(I2C_MASTER_NUM));

    // Print a message on the LCD
    lcd_write_string(I2C_MASTER_NUM, "Hello, ESP32!");

    // Loop indefinitely
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}