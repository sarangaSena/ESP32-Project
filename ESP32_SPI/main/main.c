/*
 * GccApplication1.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// SPI pins (modify as per your ESP32 board configuration)
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

// SPI bus configuration
spi_bus_config_t bus_config = {
    .miso_io_num = PIN_NUM_MISO,
    .mosi_io_num = PIN_NUM_MOSI,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1
};

// SPI device configuration
spi_device_interface_config_t dev_config = {
    .clock_speed_hz = 1000000, // Clock speed in Hz
    .mode = 0,                // SPI mode (0, 1, 2, 3)
    .spics_io_num = PIN_NUM_CS,
    .queue_size = 7,          // Number of transactions that can be queued
    .pre_cb = NULL
};

spi_device_handle_t spi;

void app_main(void)
{
    // Initialize SPI bus
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_config, 1);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to initialize SPI bus");
        return;
    }

    // Attach the SPI device to the bus
    ret = spi_bus_add_device(SPI2_HOST, &dev_config, &spi);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to add SPI device");
        return;
    }

    // Now you can send data to the SPI device
    uint8_t tx_data[2] = {0x01, 0x02};
    uint8_t rx_data[2];
    spi_transaction_t trans = {
        .length = 8 * 2,          // Transaction length in bits
        .tx_buffer = tx_data,     // Data to send
        .rx_buffer = rx_data      // Buffer to store received data
    };

    // Perform the transaction
    ret = spi_device_transmit(spi, &trans);
    if (ret == ESP_OK) {
        ESP_LOGI("SPI", "Received data: 0x%02X 0x%02X", rx_data[0], rx_data[1]);
    } else {
        ESP_LOGE("SPI", "SPI transmission failed");
    }
    
    // Cleanup: Don't forget to remove the device and deinitialize the bus when done
    spi_bus_remove_device(spi);
    spi_bus_free(SPI2_HOST);
}