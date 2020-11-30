#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_THINGSPEAK_READ_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_THINGSPEAK_READ_H_INCLUDED

#include "dataflow.h"
#include "thingspeak.h"

class DF_ThingspeakRead : public Component {
public:

	DF_ThingspeakRead(uint64_t channelID, uint8_t fieldID, const std::string& readKey);

	virtual void process() override;

private:
	uint64_t         m_channelID;
	uint8_t          m_fieldID;
	ThingSpeakClient m_client;
};

#endif // DATAFLOW_COMPONENTS_DF_THINGSPEAK_READ_H_INCLUDED
