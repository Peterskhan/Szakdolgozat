#include "i2c_master.h"


I2C_Master::I2C_Master(uint8_t port, uint8_t scl_pin, uint8_t sda_pin, std::size_t speed_hz)
	: DriverInterface(DriverInterface::Type::I2C), m_port(port), m_commands(i2c_cmd_link_create()),
	  m_mutex(xSemaphoreCreateBinary())
{
	// Initially unlocking the interface
	unlock();

	// I2C bus configuration
	i2c_config_t config;
	config.mode = I2C_MODE_MASTER;
	config.sda_io_num = sda_pin;
	config.sda_pullup_en = GPIO_PULLUP_ENABLE;
	config.scl_io_num = scl_pin;
	config.scl_pullup_en = GPIO_PULLUP_ENABLE;
	config.master.clk_speed = speed_hz;

	// Applying configuration
	i2c_param_config(port, &config);
	i2c_driver_install(port, config.mode, 0, 0, 0);
}

void I2C_Master::queue_start()
{
	// Delegating to ESP-IDF implementation
	i2c_master_start(m_commands);
}

void I2C_Master::queue_write(uint8_t bytes[], std::size_t length, bool acknowledged)
{
	// Delegating to ESP-IDF implementation
	i2c_master_write(m_commands, bytes, length, acknowledged);
}

void I2C_Master::queue_write_byte(uint8_t byte, bool acknowledged)
{
	// Delegating to ESP-IDF implementation
	i2c_master_write_byte(m_commands, byte, acknowledged);
}

void I2C_Master::queue_read(uint8_t bytes[], std::size_t length, acknowledge_t acknowledge)
{
	// Delegating to ESP-IDF implementation
	i2c_master_read(m_commands, bytes, length, (i2c_ack_type_t) acknowledge);
}

void I2C_Master::queue_read_byte(uint8_t* byte, acknowledge_t acknowledge)
{
	// Delegating to ESP-IDF implementation
	i2c_master_read_byte(m_commands, byte, (i2c_ack_type_t) acknowledge);
}

void I2C_Master::queue_stop()
{
	// Delegating to ESP-IDF implementation
	i2c_master_stop(m_commands);
}

bool I2C_Master::execute()
{
	// Delegating to ESP-IDF implementation (execute)
	esp_err_t status = i2c_master_cmd_begin(m_port, m_commands, 1000 / portTICK_RATE_MS);

	// Delegating to ESP-IDF implementation (free resources & reallocate)
	i2c_cmd_link_delete(m_commands);
	m_commands = i2c_cmd_link_create();

	// Return true when successfully executed
	return status == ESP_OK;
}

void I2C_Master::lock()
{
	xSemaphoreTake(m_mutex, portMAX_DELAY);
}

void I2C_Master::unlock()
{
	xSemaphoreGive(m_mutex);
}
