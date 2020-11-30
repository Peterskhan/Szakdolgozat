#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_SDSPI_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_SDSPI_H_INCLUDED

// Project includes
#include "dataflow.h"
#include "sd_spi.h"

class DF_SDSPI : public SD_SPI, public Component {
public:

	DF_SDSPI(uint8_t miso_pin, uint8_t mosi_pin, uint8_t sck_pin, uint8_t cs_pin);

	virtual void process() override;
};

#endif // DATAFLOW_COMPONENTS_DF_SDSPI_H_INCLUDED
