#include "df_bme280.h"

DF_BME280::DF_BME280()
{
	m_ports.addInputPort("in");
	m_ports.addInputPort("interface");
	m_ports.addOutputPort("out");
}

void DF_BME280::process()
{
	// Node object to read messages
	Node message;

	// Reading the INTERFACE port for driver interface pointer
	m_ports["interface"].receive(message);

	// Setting up the driver interface for the sensor
	setDriverInterface((DriverInterface*) message);

	// Initializing the sensor
	initialize();

	// Reading messages from the IN port
	while(true) {

		// Receiving message from the IN port
		m_ports["in"].receive(message);

		// Starting the measurements
		set_mode(BME280::Mode::Forced);

		// Waiting for the measurements to finish
		vTaskDelay((get_measurement_delay_ms() + 100) / portTICK_RATE_MS);

		// Creating output message
		message.clear();
		message["temperature"] = (double) get_temperature();
		message["pressure"]    = (double) get_pressure();
		message["humidity"]    = (double) get_humidity();

		// Sending the measurement data to the output port
		m_ports["out"].send(message);
	}
}
