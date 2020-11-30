#pragma once
#ifndef COMMON_DRIVER_DRIVER_H_INCLUDED
#define COMMON_DRIVER_DRIVER_H_INCLUDED

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
 * @file    driver_interface.h
 * @author  Peter Gyulai
 * @version 0.0.1
 * @date    2020.10.12.
 * @brief   This file defines the base class for communication driver interfaces.
 */


/**
 * This class provides a common base for different communication driver interfaces.
 * It contains a member to identify the specific interface type implemented by the
 * derived classes, which can be used to upcast safely to the appropriate derived
 * type.
 *
 * The currently supported interfaces are:
 * - UART
 * - SPI
 * - I2C
 */
class DriverInterface {
public:

	/**
	 * Defines the types of driver interfaces supported by the implementation.
	 */
	enum class Type {
		UART,   /**< The interface implements UART.                 */
		SPI,    /**< The interface implements SPI Master.           */
		I2C,    /**< The interface implements I2C Master.           */
		SD_SPI  /**< The interface implements SD cards through SPI. */
	};

	/**
	 * Constructs a driver interface of the specified type.
	 * @param type [in] The type of the interface this driver implements.
	 */
	inline DriverInterface(Type type) : m_type(type) {}

	/**
	 * Destroys the driver interface.
	 */
	inline virtual ~DriverInterface() {};

	/**
	 * Queries the type of the interface this specific driver implements.
	 * @return The type of the interface implemented by this driver.
	 */
	inline Type getType() const { return m_type; }

private:
	Type m_type; /**< The type of the specific driver interface. */
};

#endif // COMMON_DRIVER_DRIVER_H_INCLUDED
