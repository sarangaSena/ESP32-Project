/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_system.h"
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

// LM35 sensor pin
#define LM35_PIN ADC1_CHANNEL_6  // GPIO34

// LCD commands
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_HOME  0x02
#define LCD_CMD_ENTRY_MODE_SET 0x04
#define LCD_CMD_DISPLAY_CTRL 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDR 0x40
#define LCD_CMD_SET_DDRAM_ADDR 0x80

// Function prototypes
void lcd_send_command(i2c_port_t i2c_num, uint8_t cmd);
void lcd_send_data(i2c_port_t i2c_num, uint8_t data);
void lcd_init(i2c_port_t i2c_num);
void lcd_write_string(i2c_port_t i2c_num, const char *str);
void lcd_send_nibble(i2c_port_t i2c_num, uint8_t nibble);
void lcd_send_byte(i2c_port_t i2c_num, uint8_t byte);
float read_temperature(void);

void app_main(void)
{
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

    // Configure ADC for LM35
    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit ADC
    adc1_config_channel_atten(LM35_PIN, ADC_ATTEN_DB_0);  // 0 dB attenuation

    // Initialize LCD
    lcd_init(I2C_MASTER_NUM);

    while (1) {
        // Read temperature from LM35
        float temperature = read_temperature();
        
        // Display the temperature on LCD
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperature);
        lcd_send_command(I2C_MASTER_NUM, LCD_CMD_CLEAR); // Clear display
        lcd_write_string(I2C_MASTER_NUM, buffer);

        // Wait for 2 seconds before updating
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

// Function to send a command to the LCD
void lcd_send_command(i2c_port_t i2c_num, uint8_t cmd)
{
    uint8_t buf[1] = {cmd};
    i2c_master_write_to_device(i2c_num, LCD_ADDR, buf, sizeof(buf), 1000 / portTICK_PERIOD_MS);
}

// Function to send data to the LCD
void lcd_send_data(i2c_port_t i2c_num, uint8_t data)
{
    uint8_t buf[1] = {data};
    i2c_master_write_to_device(i2c_num, LCD_ADDR, buf, sizeof(buf), 1000 / portTICK_PERIOD_MS);
}

// Function to send a nibble (4 bits) to the LCD
void lcd_send_nibble(i2c_port_t i2c_num, uint8_t nibble)
{
    uint8_t buf[1] = {nibble};
    i2c_master_write_to_device(i2c_num, LCD_ADDR, buf, sizeof(buf), 1000 / portTICK_PERIOD_MS);
}

// Function to send a byte (8 bits) to the LCD
void lcd_send_byte(i2c_port_t i2c_num, uint8_t byte)
{
    lcd_send_nibble(i2c_num, byte >> 4);   // Send high nibble
    lcd_send_nibble(i2c_num, byte & 0x0F); // Send low nibble
}

// Function to initialize the LCD
void lcd_init(i2c_port_t i2c_num)
{
    // Set LCD to 4-bit mode
    lcd_send_command(i2c_num, LCD_CMD_FUNCTION_SET | 0x28); // 4-bit mode, 2-line, 5x8 matrix
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(i2c_num, LCD_CMD_DISPLAY_CTRL | 0x0C); // Display ON, Cursor OFF, Blink OFF
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(i2c_num, LCD_CMD_CLEAR); // Clear display
    vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_command(i2c_num, LCD_CMD_ENTRY_MODE_SET | 0x06); // Increment cursor
    vTaskDelay(5 / portTICK_PERIOD_MS);
}

// Function to write a string to the LCD
void lcd_write_string(i2c_port_t i2c_num, const char *str)
{
    while (*str) {
        lcd_send_data(i2c_num, *str++);
    }
}

// Function to read temperature from the LM35 sensor
float read_temperature(void)
{
    // Read raw ADC value (12-bit)
    int raw_value = adc1_get_raw(LM35_PIN);

    // Convert the raw value to voltage (assuming 3.3V reference)
    float voltage = (raw_value * 3.3) / 4095.0;

    // LM35 gives 10mV per °C, so calculate the temperature
    float temperature = voltage * 100.0;

    return temperature;
}