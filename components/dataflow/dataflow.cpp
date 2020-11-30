#include "dataflow.h"

void Dataflow::addComponent(Component* component)
{
	m_components.push_back(component);
}

void Dataflow::startFlow()
{
	for(Component* component : m_components) {
		xTaskCreatePinnedToCore(componentTaskFunction, "", 4096, component, 10, nullptr, 0);
	}
}

void Dataflow::componentTaskFunction(void* componentPtr)
{
	while(true) static_cast<Component*>(componentPtr)->process();
}
