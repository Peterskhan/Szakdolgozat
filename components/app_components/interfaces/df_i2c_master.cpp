#include "df_i2c_master.h"

DF_I2C_Master::DF_I2C_Master(uint8_t port, uint8_t scl_pin, uint8_t sda_pin, std::size_t speed_hz)
		: I2C_Master(port, scl_pin, sda_pin, speed_hz)
{
	m_ports.addOutputPort("interface");
}

void DF_I2C_Master::process()
{
	// Creating message to contain a pointer to this interface
	m_ports["interface"].send(Node("root", (DriverInterface*) this));

	// Suspending this task indefinitely
	vTaskSuspend(xTaskGetCurrentTaskHandle());
}
