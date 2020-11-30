#include "df_thingspeak_read.h"

DF_ThingspeakRead::DF_ThingspeakRead(uint64_t channelID, uint8_t fieldID, const std::string& readKey)
	: m_channelID(channelID), m_fieldID(fieldID), m_client(readKey)
{
	m_ports.addInputPort("in");
	m_ports.addOutputPort("out");
}

void DF_ThingspeakRead::process()
{
	while(true) {

		// Node object to read messages with
		Node message;

		// Reading messages from IN port
		m_ports["in"].receive(message);

		// Reading from Thingspeak channel
		std::string query = m_client.readField(m_channelID, m_fieldID);

		// Creating new message
		message.clear();

		// Writing query to the output
		message["query"] = query;
		m_ports["out"].send(message);
	}
}
