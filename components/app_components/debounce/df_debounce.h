#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_DEBOUNCE_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_DEBOUNCE_H_INCLUDED

#include "dataflow.h"

class DF_Debounce : public Component {
public:

	DF_Debounce(uint8_t debounce_ms);

	virtual void process() override;

private:
	uint64_t m_lastDebounced;
	uint8_t  m_debounce_ms;
};

#endif // DATAFLOW_COMPONENTS_DF_DEBOUNCE_H_INCLUDED
