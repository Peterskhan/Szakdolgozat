#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_BME280_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_BME280_H_INCLUDED

// Project includes
#include "dataflow.h"
#include "bme280.h"


/**
 * This class implements the BME280 sensor functionality as a
 * dataflow component to measure temperature, pressure and humidity.
 *
 * Ports:
 *
 * [input] "interface" - Used to receive a DriverInterface pointer which is used
 *                       to communicate with the sensor device (I2C or SPI). The
 *                       component blocks on the start of process() until the
 *                       communication driver interface is received.
 *
 * [input] "in"        - Used as a trigger port to start the measurements. Messages
 *                       received on this port are dropped without accessing them
 *                       before measurements are initiated.
 *
 * [output] "out"      - Used to send out the measured temperature, pressure and
 *                       humidity values. The output message contains the "temperature",
 *                       "pressure" and "humidity" fields, which are of type double.
 */
class DF_BME280 : public BME280, public Component {
public:

	/**
	 *
	 */
	DF_BME280();

	/**
	 *
	 */
	virtual void process() override;
};

#endif // DATAFLOW_COMPONENTS_DF_BME280_H_INCLUDED
