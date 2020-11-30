#pragma once
#ifndef BME280_BME280_H_INCLUDED
#define BME280_BME280_H_INCLUDED

// Standard includes
#include <cstdint>

// Project includes
#include "driver_interface.h"
#include "i2c/i2c_master.h"


// Macro definitions
/** @{ @name I2C parameters */
/** @brief Defines the default I2C address. */
#define BME280_I2C_ADDR             (uint8_t)(0x76)
/** @} */

/** @{ @name Register addresses */
/** @brief Address of HUM_LSB register. */
#define BME280_HUM_LSB              (uint8_t)(0xFE)
/** @brief Address of HUM_MSB register. */
#define BME280_HUM_MSB              (uint8_t)(0xFD)
/** @brief Address of TEMP_XLSB register. */
#define BME280_TEMP_XLSB            (uint8_t)(0xFC)
/** @brief Address of TEMP_LSB register. */
#define BME280_TEMP_LSB             (uint8_t)(0xFB)
/** @brief Address of TEMP_MSB register. */
#define BME280_TEMP_MSB             (uint8_t)(0xFA)
/** @brief Address of PRESS_XLSB register. */
#define BME280_PRESS_XLSB           (uint8_t)(0xF9)
/** @brief Address of PRESS_LSB register. */
#define BME280_PRESS_LSB            (uint8_t)(0xF8)
/** @brief Address of PRESS_MSB register. */
#define BME280_PRESS_MSB            (uint8_t)(0xF7)
/** @brief Address of CONFIG register. */
#define BME280_CONFIG               (uint8_t)(0xF5)
/** @brief Address of CTRL_MEAS register. */
#define BME280_CTRL_MEAS            (uint8_t)(0xF4)
/** @brief Address of STATUS register. */
#define BME280_STATUS               (uint8_t)(0xF3)
/** @brief Address of CTRL_HUM register. */
#define BME280_CTRL_HUM             (uint8_t)(0xF2)
/** @brief Address of RESET register. */
#define BME280_RESET                (uint8_t)(0xE0)
/** @brief Address of ID register. */
#define BME280_ID                   (uint8_t)(0xD0)
/** @} */

/** @{ @name Register bitmasks */
/** @brief Bitmask for OSRS_H bits of CTRL_HUM register. */
#define BME280_CTRL_HUM_OSRS_H      (uint8_t)(0x07)
/** @brief Bitmask for OSRS_T bits of CTRL_MEAS register. */
#define BME280_CTRL_MEAS_OSRS_T     (uint8_t)(0xE0)
/** @brief Bitmask for OSRS_P bits of CTRL_MEAS register. */
#define BME280_CTRL_MEAS_OSRS_P     (uint8_t)(0x1C)
/** @brief Bitmask for MODE bits of CTRL_MEAS register. */
#define BME280_CTRL_MEAS_MODE       (uint8_t)(0x03)
/** @brief Bitmask for MEASURING bit of the STATUS register. */
#define BME280_STATUS_MEASURING     (uint8_t)(0x08)
/** @brief Bitmask for IM_UPDATE bit of the STATUS register. */
#define BME280_STATUS_IM_UPDATE     (uint8_t)(0x01)
/** @brief Bitmask for T_SB bits of the CONFIG register. */
#define BME280_CONFIG_T_SB          (uint8_t)(0xE0)
/** @brief Bitmask for FILTER bits of the CONFIG register. */
#define BME280_CONFIG_FILTER        (uint8_t)(0x1C)
/** @brief Bitmask for SPI33W_EN bit of the CONFIG register. */
#define BME280_CONFIG_SPI3W_EN      (uint8_t)(0x01)
/** @} */

/** @{ @name Calibration parameters */
/** @brief Defines the starting address of lower calibration data registers. */
#define BME280_CALIBRATION_REGS_LOW             (uint8_t)(0x88)
/** @brief Defines the number of lower calibration data registers. */
#define BME280_CALIBRATION_REGS_LOW_COUNT       (uint8_t)(26)
/** @brief Defines the starting address of higher calibration data registers. */
#define BME280_CALIBRATION_REGS_HIGH            (uint8_t)(0xE1)
/** @brief Defines the number of higher calibration data registers. */
#define BME280_CALIBRATION_REGS_HIGH_COUNT      (uint8_t)(7)
/** @} */



/** @brief Defines the expected chip ID of the BME280. */
#define BME280_CHIP_ID                          (uint8_t)(0x60)

/** @brief Defines the specific reset byte for software reset. */
#define BME280_RESET_WORD                       (uint8_t)(0xB6)

/** @{ @name Humidity measurement modes */
/** @brief Defines humidity 1X oversampling mode. */
#define BME280_HUMIDITY_OVERSAMPLING_1X         (uint8_t)(0x01)
/** @brief Defines humidity 2X oversampling mode. */
#define BME280_HUMIDITY_OVERSAMPLING_2X         (uint8_t)(0x02)
/** @brief Defines humidity 4X oversampling mode. */
#define BME280_HUMIDITY_OVERSAMPLING_4X         (uint8_t)(0x03)
/** @brief Defines humidity 8X oversampling mode. */
#define BME280_HUMIDITY_OVERSAMPLING_8X         (uint8_t)(0x04)
/** @brief Defines humidity 16X oversampling mode. */
#define BME280_HUMIDITY_OVERSAMPLING_16X        (uint8_t)(0x05)
/** @brief Defines humidity measurements disabled. */
#define BME280_HUMIDITY_DISABLED                (uint8_t)(0x00)
/** @} */

/** @{ @name Temperature measurement modes */
/** @brief Defines temperature 1X oversampling mode. */
#define BME280_TEMPERATURE_OVERSAMPLING_1X      (uint8_t)(0x20)
/** @brief Defines temperature 2X oversampling mode. */
#define BME280_TEMPERATURE_OVERSAMPLING_2X      (uint8_t)(0x40)
/** @brief Defines temperature 4X oversampling mode. */
#define BME280_TEMPERATURE_OVERSAMPLING_4X      (uint8_t)(0x60)
/** @brief Defines temperature 8X oversampling mode. */
#define BME280_TEMPERATURE_OVERSAMPLING_8X      (uint8_t)(0x80)
/** @brief Defines temperature 16X oversampling mode. */
#define BME280_TEMPERATURE_OVERSAMPLING_16X     (uint8_t)(0xA0)
/** @brief Defines temperature measurements disabled. */
#define BME280_TEMPERATURE_DISABLED             (uint8_t)(0x00)
/** @} */

/** @{ @name Pressure measurement modes */
/** @brief Defines pressure 1X oversampling mode. */
#define BME280_PRESSURE_OVERSAMPLING_1X         (uint8_t)(0x04)
/** @brief Defines pressure 2X oversampling mode. */
#define BME280_PRESSURE_OVERSAMPLING_2X         (uint8_t)(0x08)
/** @brief Defines pressure 4X oversampling mode. */
#define BME280_PRESSURE_OVERSAMPLING_4X         (uint8_t)(0x0C)
/** @brief Defines pressure 8X oversampling mode. */
#define BME280_PRESSURE_OVERSAMPLING_8X         (uint8_t)(0x10)
/** @brief Defines pressure 16X oversampling mode. */
#define BME280_PRESSURE_OVERSAMPLING_16X        (uint8_t)(0x14)
/** @brief Defines pressure measurements disabled. */
#define BME280_PRESSURE_DISABLED                (uint8_t)(0x00)
/** @} */

/** @{ @name Measurement status */
/** @brief Defines that the device is currently measuring. */
#define BME280_MEASURING                        (uint8_t)(0x08)
/** @brief Defines that the device is currently not measuring. */
#define BME280_NOT_MEASURING                    (uint8_t)(0x00)
/** @} */

/** @{ @name Image register status */
/** @brief Defines that the image registers are updating. */
#define BME280_IMAGE_UPDATING                   (uint8_t)(0x01)
/** @brief Defines that the image registers are not updating. */
#define BME280_IMAGE_NOT_UPDATING               (uint8_t)(0x00)
/** @} */

/** @{ @name Measurement modes */
/** @brief Define for Sleep mode. */
#define BME280_MODE_SLEEP                       (uint8_t)(0x00)
/** @brief Define for Forced mode. */
#define BME280_MODE_FORCED                      (uint8_t)(0x01)
/** @brief Define for Normal mode. */
#define BME280_MODE_NORMAL                      (uint8_t)(0x03)
/** @} */

/** @{ @name Standby intervals */
/** @brief Defines 5ms of standby time. */
#define BME280_STANDBY_5MS                      (uint8_t)(0x00)
/** @brief Defines 10ms of standby time. */
#define BME280_STANDBY_10MS                     (uint8_t)(0xC0)
/** @brief Defines 20ms of standby time. */
#define BME280_STANDBY_20MS                     (uint8_t)(0xE0)
/** @brief Defines 62.5ms of standby time. */
#define BME280_STANDBY_62_5MS                   (uint8_t)(0x20)
/** @brief Defines 125ms of standby time. */
#define BME280_STANDBY_125MS                    (uint8_t)(0x40)
/** @brief Defines 250ms of standby time. */
#define BME280_STANDBY_250MS                    (uint8_t)(0x60)
/** @brief Defines 500ms of standby time. */
#define BME280_STANDBY_500MS                    (uint8_t)(0x80)
/** @brief Defines 1000ms of standby time. */
#define BME280_STANDBY_1000MS                   (uint8_t)(0xA0)
/** @} */

/** @{ @name IIR filter coefficients */
/** @brief Defines IIR filter coefficient as 2. */
#define BME280_FILTER_COEFFICIENT_2             (uint8_t)(0x04)
/** @brief Defines IIR filter coefficient as 4. */
#define BME280_FILTER_COEFFICIENT_4             (uint8_t)(0x08)
/** @brief Defines IIR filter coefficient as 8. */
#define BME280_FILTER_COEFFICIENT_8             (uint8_t)(0x0C)
/** @brief Defines IIR filter coefficient as 16. */
#define BME280_FILTER_COEFFICIENT_16            (uint8_t)(0x10)
/** @brief Defines IIR filter as disabled. */
#define BME280_FILTER_OFF                       (uint8_t)(0x00)
/** @} */

/** @{ @name SPI 3-wire interface */
/** @brief Define for SPI 3-wire mode enabled. */
#define BME280_SPI3WIRE_ENABLED                 (uint8_t)(0x01)
/** @brief Define for SPI 3-wire mode disabled. */
#define BME280_SPI3WIRE_DISABLED                (uint8_t)(0x00)
/** @} */


/**
 * @brief
 */
class BME280 {
public:

	enum Mode {
		Normal = BME280_MODE_NORMAL,
		Forced = BME280_MODE_FORCED,
		Sleep  = BME280_MODE_SLEEP
	};

	enum StandbyTime {
		Standby_5ms = BME280_STANDBY_5MS,
		Standby_10ms = BME280_STANDBY_10MS,
		Standby_20ms = BME280_STANDBY_20MS,
		Standby_62_5ms = BME280_STANDBY_62_5MS,
		Standby_125ms = BME280_STANDBY_125MS,
		Standby_250ms = BME280_STANDBY_250MS,
		Standby_500ms = BME280_STANDBY_500MS,
		Standby_1000ms = BME280_STANDBY_1000MS,
	};

	enum FilterCoefficient {
		Filter_OFF = BME280_FILTER_OFF,
		Filter_2   = BME280_FILTER_COEFFICIENT_2,
		Filter_4   = BME280_FILTER_COEFFICIENT_4,
		Filter_8   = BME280_FILTER_COEFFICIENT_8,
		Filter_16  = BME280_FILTER_COEFFICIENT_16
	};

	enum TemperatureOversampling {
		Temperature_disabled = BME280_TEMPERATURE_DISABLED,
		Temperature_ovrs_1x  = BME280_TEMPERATURE_OVERSAMPLING_1X,
		Temperature_ovrs_2x  = BME280_TEMPERATURE_OVERSAMPLING_2X,
		Temperature_ovrs_4x  = BME280_TEMPERATURE_OVERSAMPLING_4X,
		Temperature_ovrs_8x  = BME280_TEMPERATURE_OVERSAMPLING_8X,
		Temperature_ovrs_16x = BME280_TEMPERATURE_OVERSAMPLING_16X
	};

	enum PressureOversampling {
		Pressure_disabled = BME280_PRESSURE_DISABLED,
		Pressure_ovrs_1x  = BME280_PRESSURE_OVERSAMPLING_1X,
		Pressure_ovrs_2x  = BME280_PRESSURE_OVERSAMPLING_2X,
		Pressure_ovrs_4x  = BME280_PRESSURE_OVERSAMPLING_4X,
		Pressure_ovrs_8x  = BME280_PRESSURE_OVERSAMPLING_8X,
		Pressure_ovrs_16x = BME280_PRESSURE_OVERSAMPLING_16X
	};

	enum HumidityOversampling {
		Humidity_disabled = BME280_HUMIDITY_DISABLED,
		Humidity_ovrs_1x  = BME280_HUMIDITY_OVERSAMPLING_1X,
		Humidity_ovrs_2x  = BME280_HUMIDITY_OVERSAMPLING_1X,
		Humidity_ovrs_4x  = BME280_HUMIDITY_OVERSAMPLING_4X,
		Humidity_ovrs_8x  = BME280_HUMIDITY_OVERSAMPLING_8X,
		Humidity_ovrs_16x = BME280_HUMIDITY_OVERSAMPLING_16X
	};

	/**
	 * Constructs a BME280 device driver object.
	 * @param interface [in] The I2C master interface used for communication.
	 */
	BME280(DriverInterface* interface = nullptr);

	/**
	 *
	 * @param interface
	 */
	void setDriverInterface(DriverInterface* interface);

	/**
	 * Initializes the communication interface and the device driver.
	 */
	void initialize();

	/**
	 * Resets the device to the factory default state.
	 */
	void reset();

	// Mode of operation

	/**
	 * Queries the current device operation mode setting.
	 * @return The current device operation mode setting.
	 */
	Mode get_mode();

	/**
	 * Modifies the device operation mode setting.
	 * @param mode [in] The new operation mode setting to apply.
	 */
	void set_mode(Mode mode);

	// Standby time for continuous mode

	/**
	 * Queries the current standby time setting used in Normal (continuous) mode.
	 * @return The current standby time setting.
	 */
	StandbyTime get_standby_time();

	/**
	 * Modifies the standby time setting used in Normal (continuous) mode.
	 * @param standbyTime [in] The new standby time setting to apply.
	 */
	void set_standby_time(StandbyTime standbyTime);

	// Measurement data acquisition delay

	/**
	 * Queries the current measurement delay in milliseconds.
	 * @return The current measurement delay in milliseconds.
	 */
	uint8_t get_measurement_delay_ms();

	// Integrated IIR filter settings

	/**
	 * Modifies the IIR filter coefficient setting.
	 * @param coefficient [in] The new IIR coefficient setting to apply.
	 */
	void set_filter_coefficient(FilterCoefficient coefficient);

	/**
	 * Queries the current IIR filter coefficient setting.
	 * @return The current IIR filter coefficient setting.
	 */
	FilterCoefficient get_filter_coefficient();

	// Temperature reading and settings

	/**
	 * Queries the current temperature measurement in the device registers.
	 * @return The current temperature in degrees Celsius.
	 */
	double get_temperature(void);

	/**
	 * Modifies the temperature oversampling setting.
	 * @param oversampling [in] The new oversampling setting to apply.
	 */
	void set_temperature_oversampling(TemperatureOversampling oversampling);

	/**
	 * Queries the current temperature oversampling setting.
	 * @return The current temperature oversampling setting.
	 */
	TemperatureOversampling get_temperature_oversampling();

	// Pressure reading and settings

	/**
	 * Queries the current pressure measurement in the device registers.
	 * @return The current atmospheric pressure in Pascals.
	 */
	double get_pressure(void);

	/**
	 * Modifies the pressure oversampling setting.
	 * @param oversampling [in] The new oversampling setting to apply.
	 */
	void set_pressure_oversampling(PressureOversampling oversampling);

	/**
	 * Queries the current pressure oversampling setting.
	 * @return The current pressure oversampling setting.
	 */
	PressureOversampling get_pressure_oversampling();

	// Humidity reading and settings

	/**
	 * Queries the current humidity measurement in the device registers.
	 * @return The current relative humidity as a percentage.
	 */
	double get_humidity(void);

	/**
	 * Modifies the humidity oversampling setting.
	 * @param oversampling [in] The new oversampling setting to apply.
	 */
	void set_humidity_oversampling(HumidityOversampling oversampling);

	/**
	 * Queries the current humidity oversampling setting.
	 * @return The current humidity oversampling setting.
	 */
	HumidityOversampling get_humidity_oversampling();

private:

	/**
	 * Reads the specified number of register values (address auto-incremented).
	 * @param registerAddress [in]  The starting register address.
	 * @param buffer          [out] The buffer where register values are stored.
	 * @param byteCount	      [in]  The number of registers to read.
	 */
	void register_read(uint8_t registerAddress, uint8_t buffer[], uint8_t byteCount);

	/**
	 * Writes the specified number of register values (address auto-incremented).
	 * @param registerAddress [in] The starting register address.
	 * @param buffer          [in] The values to write into the registers.
	 * @param byteCount       [in] The number of values to write.
	 */
	void register_write(uint8_t registerAddress, const uint8_t buffer[], uint8_t byteCount);

	/**
	 * This structure holds the calibration constants for the different measurements.
	 */
	struct CalibrationStruct {
		uint16_t dig_T1; /**< Holds the dig_T1 temperature calibration constant. */
		int16_t  dig_T2; /**< Holds the dig_T2 temperature calibration constant. */
		int16_t  dig_T3; /**< Holds the dig_T3 temperature calibration constant. */
		uint16_t dig_P1; /**< Holds the dig_P1 pressure calibration constant.    */
		int16_t  dig_P2; /**< Holds the dig_P2 pressure calibration constant.    */
		int16_t  dig_P3; /**< Holds the dig_P3 pressure calibration constant.    */
		int16_t  dig_P4; /**< Holds the dig_P4 pressure calibration constant.    */
		int16_t  dig_P5; /**< Holds the dig_P5 pressure calibration constant.    */
		int16_t  dig_P6; /**< Holds the dig_P6 pressure calibration constant.    */
		int16_t  dig_P7; /**< Holds the dig_P7 pressure calibration constant.    */
		int16_t  dig_P8; /**< Holds the dig_P8 pressure calibration constant.    */
		int16_t  dig_P9; /**< Holds the dig_P9 pressure calibration constant.    */
		uint8_t  dig_H1; /**< Holds the dig_H1 humidity calibration constant.    */
		int16_t  dig_H2; /**< Holds the dig_H2 humidity calibration constant.    */
		uint8_t  dig_H3; /**< Holds the dig_H3 humidity calibration constant.    */
		int16_t  dig_H4; /**< Holds the dig_H4 humidity calibration constant.    */
		int16_t  dig_H5; /**< Holds the dig_H5 humidity calibration constant.    */
		int8_t   dig_H6; /**< Holds the dig_H6 humidity calibration constant.    */
		int32_t  t_fine; /**< Holds the fine temperature compensation value.     */
	};

	DriverInterface*  m_interface;   /**< Communication interface. */
	CalibrationStruct m_calibration; /**< Calibration constants    */
};

#endif // BME280_BME280_H_INCLUDED
