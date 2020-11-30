#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_THINGSPEAK_WRITE_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_THINGSPEAK_WRITE_H_INCLUDED

#include "dataflow.h"
#include "thingspeak.h"

class DF_ThingspeakWrite : public Component {
public:

	DF_ThingspeakWrite(const std::string& writeKey);

	virtual void process() override;

private:
	ThingSpeakClient m_client;
};

#endif // DATAFLOW_COMPONENTS_DF_THINGSPEAK_READ_H_INCLUDED
