#pragma once
#ifndef DRIVER_INTERFACE_SD_SPI_SD_SPI_H_INCLUDED
#define DRIVER_INTERFACE_SD_SPI_SD_SPI_H_INCLUDED

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
 * @file    sd_spi.h
 * @author  Peter Gyulai
 * @version 0.0.1
 * @date    2020.10..
 * @brief   This file defines the driver interface for an SD card through SPI.
 */

// Standard includes
#include <cstdint>
#include <string>

// Project includes
#include "../driver_interface.h"

// Implementation includes
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"


/**
 * @brief
 */
class SD_SPI : public DriverInterface {
public:

	/**
	 * Creates and initializes and SD card interface through SPI.
	 * @param miso_pin [in] The pin for the MISO line.
	 * @param mosi_pin [in] The pin for the MOSI line.
	 * @param sck_pin  [in] The pin for the SCK line.
	 * @param cs_pin   [in] The pin for the CS line.
	 */
	SD_SPI(uint8_t miso_pin, uint8_t mosi_pin, uint8_t sck_pin, uint8_t cs_pin);

	/**
	 * Destroys the SD card interface, unmounts the card and releases SPI resources.
	 */
	~SD_SPI();

	/**
	 * Mounts the SD cards as a FATFS filesystem drive with the specified name.
	 * @param  driveName [in] The prefix name for the drive to mount.
	 * @return True when the SD card is successfully mounted.
	 */
	bool mount(const std::string& driveName);

	/**
	 * Unmounts the SD card from the FATFS filesystem.
	 * @return True when the SD card is successfully unmounted.
	 */
	bool unmount();

	/**
	 * Queries whether the SD card is currently mounted to the FATFS filesystem.
	 * @return True when the SD card is mounted.
	 */
	bool isMounted() const;

	/**
	 * Queries the drive name prefix where the SD card is mounted.
	 * @return The prefix name of the drive or an empty string if unmounted.
	 */
	std::string getDriveName() const;

private:
	sdmmc_card_t* m_card;      /**< Pointer to the ESP-IDF SD_SPI implementation.       */
	sdmmc_host_t  m_host;      /**< ESP-IDF SPI host implementation.                    */
	std::string   m_driveName; /**< The drive name suffix where the SD card is mounted. */
	bool          m_isMounted; /**< Flag to indicate when the SD card drive is mounted. */
	uint8_t       m_cs_pin;    /**< The GPIO number for the SD card chip-select line.   */


};

#endif // DRIVER_INTERFACE_SD_SPI_SD_SPI_H_INCLUDED
