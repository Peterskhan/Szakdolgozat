#include "df_debug.h"

DF_Debug::DF_Debug()
{
	m_ports.addInputPort("in");
	m_ports.addOutputPort("out");
}

void DF_Debug::process()
{
	while(true) {

		// Node object for reading messages
		Node message;

		// Reading message from the input port
		m_ports["in"].receive(message);

		// Printing the message
		for(auto it = message.begin(); it != message.end(); it++) {
			std::cout << std::string(it.level(), ' ') << it->name() << ": " << *it << std::endl;
		}

		// Writing to the output port if it is connected
		if(m_ports["out"].isConnected()) {
			m_ports["out"].send(message);
		}
	}
}
