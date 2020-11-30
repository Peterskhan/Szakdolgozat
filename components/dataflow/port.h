#pragma once
#ifndef DATAFLOW_PORT_H_INCLUDED
#define DATAFLOW_PORT_H_INCLUDED

// Standard includes
#include <string>
#include <vector>

// FreeRTOS include
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Project includes
#include "node.hpp"


/**
 * The Port class implements a generic input/output capability for
 * dataflow components. Ports are used to receive/send messages of
 * type Node via pointers to Node objects. Ports are created by the
 * components at initialization and stored inside the components
 * themselves in an inherited storage container. The internal message
 * passing mechanism uses thread-safe message queues from the RTOS.
 */
class Port {
public:

	/**
	 * Defines the possible dataflow direction of Ports.
	 */
	enum class Direction {
		INPUT, /**< INPUT, used to receive messages. */
		OUTPUT /**< OUTPUT, used to send messages.   */
	};

	/**
	 * Constructs a Port with the specified dataflow direction and name.
	 * @param direction [in] The dataflow direction of the Port.
	 * @param name      [in] The unique (component-wise) name of the Port.
	 * @param queueSize [in] Applicable only to input ports, the size of the message queue.
	 */
	Port(Direction direction, const std::string& name, std::size_t queueSize);

	/**
	 * Destroys the Port and releases internal resources (eg. RTOS message queues).
	 */
	~Port();

	/**
	 * Sends a message to all of the connected input ports. This operation
	 * blocks when the input port message queue is full.
	 * @param  message [in] Pointer to the message to send.
	 * @return True when the messages are sent successfully.
	 */
	bool send(const Node& message);

	/**
	 * Receives a message from the input port message queue.
	 * @param  message [in] The referenced pointer that will point to the message.
	 * @return True when the message is successfully received.
	 */
	bool receive(Node& message);

	/**
	 * Queries whether the Port is connected to another Port.
	 * @return True when this port is connected to another Port.
	 */
	bool isConnected() const noexcept;

	/**
	 * Quries the dataflow direction of this Port.
	 * @return The dataflow direction of this Port: input or output.
	 */
	const Direction& direction() const noexcept;

	/**
	 * Queries the name of this Port.
	 * @return The name of this Port.
	 */
	const std::string& name() const noexcept;

	/**
	 * Connects this input Port to the specified output Port.
	 * @param other [in] The other output port to connect to.
	 */
	void operator>>(Port& other) noexcept;

private:
	std::vector<QueueHandle_t> m_queues;    /**< The list of message queues that are connected    */
	Direction                  m_direction; /**< The dataflow direction of this port.             */
	std::string                m_name;      /**< The unique name of this port.                    */
	bool                       m_connected; /**< Flag to indicate whether this port is connected. */
};

#endif // DATAFLOW_PORT_H_INCLUDED
