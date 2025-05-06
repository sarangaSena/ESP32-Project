/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_system.h"

// Define I2C master GPIO pins
#define I2C_MASTER_SCL_IO    22               /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO    21               /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM       I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ   100000           /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0           /*!< I2C master doesn't need buffer */
#define EEPROM_ADDR          0x50             /*!< EEPROM I2C Address */
#define I2C_MASTER_TIMEOUT_MS 1000            /*!< I2C master timeout in ms */

// I2C master initialization
esp_err_t i2c_master_init() {
    esp_err_t esp_rc = ESP_OK;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .clk_speed = I2C_MASTER_FREQ_HZ
    };

    esp_rc = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (esp_rc != ESP_OK) {
        ESP_LOGE("I2C", "Failed to configure I2C parameters");
        return esp_rc;
    }

    esp_rc = i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (esp_rc != ESP_OK) {
        ESP_LOGE("I2C", "Failed to install I2C driver");
        return esp_rc;
    }

    return esp_rc;
}

// Write data to EEPROM
esp_err_t i2c_master_write(uint8_t mem_address, uint8_t data) {
    esp_err_t esp_rc;
    uint8_t data_buffer[2] = {mem_address, data};

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EEPROM_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data_buffer, sizeof(data_buffer), true);
    i2c_master_stop(cmd);

    esp_rc = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);

    if (esp_rc != ESP_OK) {
        ESP_LOGE("I2C", "Failed to write to EEPROM at address 0x%X", mem_address);
    }

    return esp_rc;
}

// Read data from EEPROM
esp_err_t i2c_master_read(uint8_t mem_address, uint8_t *data_out) {
    esp_err_t esp_rc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EEPROM_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, &mem_address, sizeof(mem_address), true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EEPROM_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, data_out, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    esp_rc = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);

    if (esp_rc != ESP_OK) {
        ESP_LOGE("I2C", "Failed to read from EEPROM at address 0x%X", mem_address);
    }

    return esp_rc;
}

void app_main() {
    esp_err_t ret = i2c_master_init();
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "I2C initialization failed");
        return;
    }

    uint8_t write_data = 0x42; // Data to write
    uint8_t read_data = 0x00;  // Variable to store read data
    uint8_t mem_address = 0x10; // Memory address to write/read from

    // Writing data to EEPROM
    ret = i2c_master_write(mem_address, write_data);
    if (ret == ESP_OK) {
        ESP_LOGI("I2C", "Data 0x%X written to EEPROM at address 0x%X", write_data, mem_address);
    } else {
        ESP_LOGE("I2C", "Failed to write to EEPROM");
    }

    // Reading data from EEPROM
    ret = i2c_master_read(mem_address, &read_data);
    if (ret == ESP_OK) {
        ESP_LOGI("I2C", "Data read from EEPROM at address 0x%X: 0x%X", mem_address, read_data);
    } else {
        ESP_LOGE("I2C", "Failed to read from EEPROM");
    }
}