#include "df_watchdog.h"

DF_Watchdog::DF_Watchdog(uint64_t period_ms)
	: m_period_ms(period_ms), m_timerHandle(nullptr)
{
	m_ports.addInputPort("in");
	m_ports.addOutputPort("out");
}

void DF_Watchdog::process()
{
	// Creating and starting the watchdog timer
	m_timerHandle = xTimerCreate("DF_Watchdog", pdMS_TO_TICKS(m_period_ms), pdFALSE, this, watchdog_callback);
	xTimerStart(m_timerHandle, portMAX_DELAY);

	// Indefinitely performing the watchdog function
	while(true) {

		// Node object to receive messages
		Node message;

		// Waiting for message on the input port
		m_ports["in"].receive(message);

		// Resetting the watchdog timer
		xTimerReset(m_timerHandle, portMAX_DELAY);
	}
}

void DF_Watchdog::watchdog_callback(TimerHandle_t timer)
{
	// Getting pointer to the watchdog component of the expired timer
	DF_Watchdog* watchdog = static_cast<DF_Watchdog*>(pvTimerGetTimerID(timer));

	// Writing to the output of the watchdog component
	watchdog->m_ports["out"].send(Node("watchdog"));
}
