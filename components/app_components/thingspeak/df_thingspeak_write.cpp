#include "df_thingspeak_write.h"

DF_ThingspeakWrite::DF_ThingspeakWrite(const std::string& writeKey)
	: m_client("", writeKey)
{
	m_ports.addInputPort("in");
}

void DF_ThingspeakWrite::process()
{
	while(true) {

		// Node object to read messages with
		Node message;

		// Reading messages from IN port
		m_ports["in"].receive(message);

		// Writing to Thingspeak channel
		ThingSpeakUpdate update;
		for(uint8_t i = 0; i < message["update"].child_count(); i++) {
			update.setField(i + 1, message["update"][i]);
		}

		// Sending the update
		m_client.writeChannel(update);
	}
}
