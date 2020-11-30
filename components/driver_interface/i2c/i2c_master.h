#pragma once
#ifndef DRIVER_INTERFACE_I2C_I2C_MASTER_H_INCLUDED
#define DRIVER_INTERFACE_I2C_I2C_MASTER_H_INCLUDED

/*********************************************************************************
 * Copyright (c) 2020 Peter Gyulai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
**********************************************************************************/

/**
 * @file    i2c_master.h
 * @author  Peter Gyulai
 * @version 0.0.1
 * @date    2020.10.12.
 * @brief   This file defines the driver interface for an I2C Master.
 */

// Standard includes
#include <cstdint>

// Project includes
#include "../driver_interface.h"

// Implementation includes
#include "driver/i2c.h"
#include "driver/gpio.h"


/**
 * @brief
 */
class I2C_Master : public DriverInterface {
public:

	enum acknowledge_t {
		nack_last = I2C_MASTER_LAST_NACK,
		nack      = I2C_MASTER_NACK,
		ack       = I2C_MASTER_ACK
	};

	/**
	 * @brief Constructs an I2C Master interface.
	 * @param port     [in] The I2C port to use.
	 * @param scl_pin  [in] The GPIO to use for the SCL pin.
	 * @param sda_pin  [in] The GPIO to use for the SDA pin.
	 * @param speed_hz [in] The speed of the SCL clock in Hertz.
	 */
	I2C_Master(uint8_t port, uint8_t scl_pin, uint8_t sda_pin, std::size_t speed_hz);

	/**
	 * @brief Queues the generation of a START condition on the bus.
	 */
	void queue_start();

	/**
	 * @brief Queues multiple byte writes on the bus.
	 * @param bytes        [in] Pointer to the array where the bytes are stored.
	 * @param length       [in] The number of bytes to write to the bus.
	 * @param acknowledged [in] True when the acknowledge bit needs to be checked.
	 */
	void queue_write(uint8_t bytes[], std::size_t length, bool acknowledge);

	/**
	 * @brief Queues the writing of one byte to the bus.
	 * @param byte         [in] The byte to write to the bus.
	 * @param acknowledged [in] True when the acknowledge bit needs to be checked.
	 */
	void queue_write_byte(uint8_t byte, bool acknowledged);

	/**
	 * @brief Queues the reading of multiple bytes from the bus.
	 * @param bytes       [out] Pointer to the array where the results are stored.
	 * @param length      [in]  The number of bytes to read.
	 * @param acknowledge [in]  The type of acknowledge to generate.
	 */
	void queue_read(uint8_t bytes[], std::size_t length, acknowledge_t acknowledge);

	/**
	 * @brief Queues the reading of one byte from the bus.
	 * @param byte        [out] Pointer to the byte where the result is stored.
	 * @param acknowledge [in]  The type of acknowledge to generate.
	 */
	void queue_read_byte(uint8_t* byte, acknowledge_t acknowledge);

	/**
	 * @brief Queues the generation of a STOP condition on the bus.
	 */
	void queue_stop();

	/**
	 * @brief  Executes the queued I2C operations and blocks until finished.
	 * @return
	 */
	bool execute();

	/**
	 * @brief Locks this I2C master interface from concurrent access.
	 */
	void lock();

	/**
	 * @brief Unlocks this I2C master interface.
	 */
	void unlock();

private:
	i2c_port_t        m_port;     /**< The I2C port used by this interface.         */
	i2c_cmd_handle_t  m_commands; /**< The I2C command link used by this interface. */
	SemaphoreHandle_t m_mutex;    /**< The mutex used for thread-safe access.       */
};

#endif // DRIVER_INTERFACE_I2C_I2C_MASTER_H_INCLUDED
