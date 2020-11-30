#pragma once
#ifndef DATAFLOW_COMPONENTS_I2C_MASTER_H_INCLUDED
#define DATAFLOW_COMPONENTS_I2C_MASTER_H_INCLUDED

#include "i2c_master.h"
#include "dataflow.h"


/**
 * This class implements an I2C Master driver interface as a dataflow component.
 *
 * Ports:
 *
 * [output] "interface" - Upon startup, a DriverInterface pointer to this interface
 *                        is sent out on this port which can be used to feed into
 *                        I2C interface dependent components. After that, the component
 *                        suspends execution indefinitely.
 */
class DF_I2C_Master : public Component, public I2C_Master {
public:

	DF_I2C_Master(uint8_t port, uint8_t scl_pin, uint8_t sda_pin, std::size_t speed_hz);

	virtual void process() override;
};

#endif
