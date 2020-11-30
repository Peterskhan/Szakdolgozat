#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_FUNCTION_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_FUNCTION_H_INCLUDED

#include <functional>
#include "dataflow.h"


class DF_Function : public Component {
public:

	template <class Callable>
	DF_Function(Callable&& callable)
		: m_function(callable)
	{
		// Adding generic input port
		m_ports.addInputPort("in");

		// Adding generic output port
		m_ports.addOutputPort("out");
	}

	virtual void process() override;

private:
	std::function<void(Component::PortContainer&)> m_function;
};

#endif // DATAFLOW_COMPONENTS_DF_FUNCTION_H_INCLUDED
