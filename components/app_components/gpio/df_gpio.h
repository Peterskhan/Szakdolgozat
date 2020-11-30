#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_GPIO_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_GPIO_H_INCLUDED

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "dataflow.h"


class DF_GPIO : public Component {
public:

	enum class Direction {
		INPUT  = GPIO_MODE_INPUT,
		OUTPUT = GPIO_MODE_OUTPUT
	};

	enum class PullMode {
		PULLUP,
		PULLDOWN,
		NONE
	};

	enum class TriggerType {
		DISABLED   = GPIO_INTR_DISABLE,
		POSEDGE    = GPIO_INTR_POSEDGE,
		NEGEDGE    = GPIO_INTR_NEGEDGE,
		ANYEDGE    = GPIO_INTR_ANYEDGE,
		LOW_LEVEL  = GPIO_INTR_LOW_LEVEL,
		HIGH_LEVEL = GPIO_INTR_HIGH_LEVEL
	};

	DF_GPIO(uint8_t gpioNum, Direction direction, PullMode pullMode = PullMode::NONE,
			TriggerType triggerType = TriggerType::DISABLED);

	virtual void process() override;

private:
	uint8_t      m_gpioNum;
	Direction    m_direction;
	TaskHandle_t m_thisTask;
	static bool  s_isrServiceInstalled;

	static void  interrupt_handler(void* params);
};

#endif // DATAFLOW_COMPONENTS_DF_GPIO_H_INCLUDED
