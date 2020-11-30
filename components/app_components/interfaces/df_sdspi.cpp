#include "df_sdspi.h"

DF_SDSPI::DF_SDSPI(uint8_t miso_pin, uint8_t mosi_pin, uint8_t sck_pin, uint8_t cs_pin)
	: SD_SPI(miso_pin, mosi_pin, sck_pin, cs_pin)
{
	m_ports.addOutputPort("interface");
}

void DF_SDSPI::process()
{
	// Mounting SD card
	bool mounted = mount("/sd");

	// Creating message to contain a pointer to this interface
	if(mounted) m_ports["interface"].send(Node("root", (DriverInterface*) this));

	// Suspending this task indefinitely
	vTaskSuspend(xTaskGetCurrentTaskHandle());
}
