#include "df_debounce.h"

DF_Debounce::DF_Debounce(uint8_t debounce_ms)
	: m_lastDebounced(0), m_debounce_ms(debounce_ms)
{
	// Adding input & output ports
	m_ports.addInputPort("in");
	m_ports.addOutputPort("out");
}

void DF_Debounce::process()
{
	// Reading input messages indefinitely
	while(true) {

		// Node object to receive messages
		Node message;

		// Reading input message
		m_ports["in"].receive(message);

		// Checking if the debounce time has elapsed
		if((xTaskGetTickCount() - m_lastDebounced) > pdMS_TO_TICKS(m_debounce_ms)) {

			// Forwarding the message to the output port
			m_ports["out"].send(message);

			// Resetting the debounce timing
			m_lastDebounced = xTaskGetTickCount();
		}
	}
}
