#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_WATCHDOG_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_WATCHDOG_H_INCLUDED

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "dataflow.h"

class DF_Watchdog : public Component {
public:

	DF_Watchdog(uint64_t period_ms);

	virtual void process() override;

private:
	uint64_t      m_period_ms;
	TimerHandle_t m_timerHandle;

	static void watchdog_callback(TimerHandle_t timer);
};

#endif // DATAFLOW_COMPONENTS_DF_WATCHDOG_H_INCLUDED
