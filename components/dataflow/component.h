#pragma once
#ifndef DATAFLOW_COMPONENT_H_INCLUDED
#define DATAFLOW_COMPONENT_H_INCLUDED

// Standard includes
#include <map>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project includes
#include "port.h"


/**
 * The Component class provides a base class for creating dataflow components.
 * The logic of the components must be implemented inside the public process()
 * member function. Here the components can read/write input/output ports, perform
 * logic on their inputs to create outputs or generate side effects. The dataflow
 * implementation creates a new thread for each component instance and calls the
 * process() method, which is expected not to return for the useful lifetime of
 * the component. Input and output ports should be created and added in the
 * constructor of derived component classes.
 */
class Component {
public:

	// Forward declaration of the PortQuery class which is used to
	// query and connect named Ports from the Component.
	class PortQuery;

	/**
	 * Destroys the dataflow Component.
	 */
	inline virtual ~Component() { }

	/**
	 * Implements the core logic of the Component, reads input ports,
	 * performs logic and writes to the output ports. Derived classes
	 * need to implement this method as the main functionality of the
	 * Component.
	 */
	virtual void process() = 0;

	/**
	 * Queries the Component for the Port with the specified name.
	 * @param  name [in] The name of the Port to query from the Component.
	 * @return A PortQuery object which can be used to connect Ports.
	 */
	PortQuery operator[](const std::string& name);

	/**
	 * The PortQuery class represents the result of a named query for a
	 * Port of the component. The query object can be used to connect to
	 * the port, send initial messages and query for another port on the
	 * same component.
	 */
	class PortQuery {
	public:

		/**
		 * Constructs a PortQuery object referencing the parent component,
		 * and the result of the named port search. This constructor is used
		 * only by the Components themselves when they are queried for a Port.
		 * @param parent [in] Pointer to the parent component.
		 * @param left   [in] Pointer to the result of the Port query.
		 */
		PortQuery(Component* parent, Port* left);

		/**
		 * Queries the parent Component for another port, to support chaining
		 * syntax. The query will contain at most two Port references, the one
		 * which it was created with and the last user-indexed Port.
		 * @param  name [in] The name of the OUTPUT port to query next.
		 * @return Reference to this object after the query is performed.
		 */
		PortQuery& operator[](const std::string& name);

		/**
		 * Connects the OUTPUT Port referenced by this query to the INPUT Port
		 * referenced by the other query.
		 * @param  other [in] The other query referencing an INPUT Port.
		 * @return A copy of the other query for chaining connections.
		 */
		PortQuery operator>>(const PortQuery& other);

		/**
		 * Sends an initial message to the Port referenced by this query.
		 * @param message [in] Pointer to the message root Node to send.
		 */
		void sendInitialMessage(const Node& message);

	private:
		Component* m_parent; /**< Pointer to the parent component of the referenced Port(s). */
		Port*      m_left;   /**< Pointer to the left-side Port when making connections.     */
		Port*      m_right;  /**< Pointer to the right-side Port when making connections.    */
	};

	/**
	 * The PortContainer class implements a thin wrapper around an std::map,
	 * and is used to store the Ports of the Components. It provides an intuitive
	 * syntax for querying ports inside the Components themselves and convenience
	 * functions for adding INPUT and OUTPUT Ports during initialization.
	 */
	class PortContainer {
	public:

		/**
		 * Adds a named input Port to the Component with the specified message queue size.
		 * @param  name      [in] The name of the new Port to add to the Component.
		 * @param  queueSize [in] The size of the message queue for this Port.
		 * @return True when the Port has been added successfully.
		 */
		bool addInputPort(const std::string& name, std::size_t queueSize = 10);

		/**
		 * Adds a named output Port to the Component.
		 * @param  name [in] The name of the new Port to add to the Component.
		 * @return True when the Port has been added successfully.
		 */
		bool addOutputPort(const std::string& name);

		/**
		 * Queries the Port with the specified name, throws std::out_of_bounds when not found.
		 * This query operator should be used by the overridden process() method to access Ports.
		 * @param  name [in] The name of the Port to query.
		 * @return Reference to the resulting Port of the query.
		 */
		Port& operator[](const std::string& name);

	private:
		std::map<std::string, Port> m_ports; /**< The internal storage implementation. */
	};

protected:
	PortContainer m_ports; /**< The internal storage for storing Ports of the Component. */
};

#endif // DATAFLOW_COMPONENT_H_INCLUDED
