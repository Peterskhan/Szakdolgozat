#include "port.h"

Port::Port(Direction direction, const std::string& name, std::size_t queueSize)
	: m_direction(direction), m_name(name), m_connected(false)
{
	if(m_direction == Direction::INPUT) {
		m_queues.push_back(xQueueCreate(queueSize, sizeof(Node*)));
	}
}

Port::~Port()
{
	if(m_direction == Direction::INPUT) {
		//vQueueDelete(m_queue);
	}
}

bool Port::send(const Node& message)
{
	// Status flag to indicate sussessful write to all queues
	bool status = true;

	// Sending the message to all queues
	for(auto queue : m_queues) {

		// Making a copy of the message to send
		Node* copy = new Node(message);

		// Sending the message to the output queue
		status &= (xQueueSendToBack(queue, (void*) &copy, portMAX_DELAY) == pdTRUE);
	}

	return status;
}

bool Port::receive(Node& message)
{
	// Checking if the port is an input port
	if(m_direction != Direction::INPUT) return false;

	// Popping the message pointer from the queue
	Node* message_ptr = nullptr;
	bool status = xQueueReceive(m_queues[0], &message_ptr, portMAX_DELAY) == pdTRUE;

	// Returning the message
	if(status) message = *message_ptr;

	// Deleting the message copy
	delete message_ptr;

	// Returning the message receive status
	return status;
}

bool Port::isConnected() const noexcept
{
	return m_connected;
}

const Port::Direction& Port::direction() const noexcept
{
	return m_direction;
}

const std::string& Port::name() const noexcept
{
	return m_name;
}

void Port::operator>>(Port& other) noexcept
{
	// Checking if this Port is an output and the target is an input
	if(m_direction != Direction::OUTPUT || other.m_direction != Direction::INPUT) return;

	// Connecting the message queues of the ports
	m_queues.push_back(other.m_queues[0]);

	// Indicating connection status for both ports
	m_connected = true;
	other.m_connected = true;
}
