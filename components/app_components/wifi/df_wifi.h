#pragma once
#ifndef DATAFLOW_COMPONENTS_WIFI_H_INCLUDED
#define DATAFLOW_COMPONENTS_WIFI_H_INCLUDED

#include "dataflow.h"
#include "wifi.h"


class DF_WifiConnect : public Component {
public:

	DF_WifiConnect(const std::string& ssid, const std::string& password);

	virtual void process() override;

private:
	std::string m_ssid;
	std::string m_password;
};

#endif // DATAFLOW_COMPONENTS_WIFI_H_INCLUDED
