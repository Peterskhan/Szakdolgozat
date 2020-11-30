#include "component.h"


Component::PortQuery::PortQuery(Component* parent, Port* left)
	: m_parent(parent), m_left(left), m_right(nullptr)
{}

Component::PortQuery& Component::PortQuery::operator[](const std::string& name)
{
	// Assume, that the specified output port does not exist
	m_right = nullptr;

	// Checking if the parent component has the specified output port
	if(m_parent->m_ports[name].direction() == Port::Direction::OUTPUT) {
		m_right = &m_parent->m_ports[name];
	}

	return *this;
}

Component::PortQuery Component::PortQuery::operator>>(const PortQuery& other)
{
	// Checking if the query contains a right-hand-side port and connecting them
	if(m_right != nullptr && other.m_left->direction() == Port::Direction::INPUT) {

		// Connecting the right-hand-side of this query to the left-hand-side of the other
		*(m_right) >> *(other.m_left);
	}

	// Otherwise make the connection from the left-hand-side
	else if(m_left != nullptr && other.m_left->direction() == Port::Direction::INPUT) {

		// Connecting the left-hand-side of this query to the left-hand-side of the other
		*(m_left) >> *(other.m_left);
	}

	return other;
}

void Component::PortQuery::sendInitialMessage(const Node& message)
{
	m_left->send(message);
}

Component::PortQuery Component::operator[](const std::string& name)
{
	return PortQuery(this, &m_ports[name]);
}

bool Component::PortContainer::addInputPort(const std::string& name, std::size_t queueSize)
{
	// Checking if a port already exists with the same name
	if(m_ports.count(name) != 0) return false;

	// Adding the input port to the component
	m_ports.emplace(std::pair<std::string, Port>(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(Port::Direction::INPUT, name, queueSize))
	);
	return true;
}

bool Component::PortContainer::addOutputPort(const std::string& name)
{
	// Checking if a port already exists with the same name
	if(m_ports.count(name) != 0) return false;

	// Adding the output port to the component
	m_ports.emplace(std::pair<std::string, Port>(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(Port::Direction::OUTPUT, name, 0))
	);
	return true;
}

Port& Component::PortContainer::operator[](const std::string& name)
{
	return m_ports.at(name);
}



