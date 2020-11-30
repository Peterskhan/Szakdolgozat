#pragma once
#ifndef DATAFLOW_DATAFLOW_H_INCLUDED
#define DATAFLOW_DATAFLOW_H_INCLUDED

// Standard includes
#include <vector>

// FreeRTOS includes

// Project includes
#include "component.h"


class Dataflow {
public:

	void addComponent(Component* component);

	void startFlow();

private:

	static void componentTaskFunction(void* componentPtr);

	std::vector<Component*> m_components;
};

#endif // DATAFLOW_DATAFLOW_H_INCLUDED
