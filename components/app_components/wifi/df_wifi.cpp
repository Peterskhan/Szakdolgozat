#include "df_wifi.h"

DF_WifiConnect::DF_WifiConnect(const std::string& ssid, const std::string& password)
	: m_ssid(ssid), m_password(password)
{
	m_ports.addInputPort("in");
	m_ports.addOutputPort("out");
}

void DF_WifiConnect::process()
{
	// Node object to read messages
	Node message;

	// Reading trigger message from the input
	m_ports["in"].receive(message);

	// Connecting to WiFi network
	WiFi::start();
	bool successful = WiFi::connect(m_ssid, m_password, 5);

	// Sending output message indicating success or failure
	m_ports["out"].send(Node("connected", successful));

	// Suspending execution indifinitely
	vTaskSuspend(xTaskGetCurrentTaskHandle());
}
