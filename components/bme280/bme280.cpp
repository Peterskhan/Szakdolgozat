#include "bme280.h"


BME280::BME280(DriverInterface* interface)
	: m_interface(interface)
{
	// Initializing the device with factory defaults
	if(m_interface != nullptr) initialize();
}

void BME280::setDriverInterface(DriverInterface* interface)
{
	m_interface = interface;
}

void BME280::initialize()
{
	// Reading calibration data
	uint8_t calibDataLow[BME280_CALIBRATION_REGS_LOW_COUNT];
	uint8_t calibDataHigh[BME280_CALIBRATION_REGS_HIGH_COUNT];
	register_read(BME280_CALIBRATION_REGS_LOW, calibDataLow, BME280_CALIBRATION_REGS_LOW_COUNT);
	register_read(BME280_CALIBRATION_REGS_HIGH, calibDataHigh, BME280_CALIBRATION_REGS_HIGH_COUNT);

	// Setting temperature compensation parameters
	m_calibration.dig_T1 = (uint16_t) calibDataLow[0]  | (uint16_t) calibDataLow[1] << 8;
	m_calibration.dig_T2 = (int16_t)  calibDataLow[2]  | (int16_t)  calibDataLow[3] << 8;
	m_calibration.dig_T3 = (int16_t)  calibDataLow[4]  | (int16_t)  calibDataLow[5] << 8;

	// Setting pressure compensation parameters
	m_calibration.dig_P1 = (uint16_t) calibDataLow[6]  | (uint16_t) calibDataLow[7] << 8;
	m_calibration.dig_P2 = (int16_t)  calibDataLow[8]  | (int16_t)  calibDataLow[9] << 8;
	m_calibration.dig_P3 = (int16_t)  calibDataLow[10] | (int16_t)  calibDataLow[11] << 8;
	m_calibration.dig_P4 = (int16_t)  calibDataLow[12] | (int16_t)  calibDataLow[13] << 8;
	m_calibration.dig_P5 = (int16_t)  calibDataLow[14] | (int16_t)  calibDataLow[15] << 8;
	m_calibration.dig_P6 = (int16_t)  calibDataLow[16] | (int16_t)  calibDataLow[17] << 8;
	m_calibration.dig_P7 = (int16_t)  calibDataLow[18] | (int16_t)  calibDataLow[19] << 8;
	m_calibration.dig_P8 = (int16_t)  calibDataLow[20] | (int16_t)  calibDataLow[21] << 8;
	m_calibration.dig_P9 = (int16_t)  calibDataLow[22] | (int16_t)  calibDataLow[23] << 8;

	// Setting humidity compensation parameters
	m_calibration.dig_H1 = (uint8_t)  calibDataLow[25];
	m_calibration.dig_H2 = (int16_t)  calibDataHigh[0]         | (int16_t) calibDataHigh[1] << 8;
	m_calibration.dig_H3 = (uint8_t)  calibDataHigh[2];
	m_calibration.dig_H4 = (int16_t) (calibDataHigh[4] | 0x0F) | (int16_t) calibDataHigh[3] << 4;
	m_calibration.dig_H5 = (int16_t) (calibDataHigh[4] | 0xF0) | (int16_t) calibDataHigh[5] << 4;
	m_calibration.dig_H6 = (int8_t)   calibDataHigh[6];

	// Performing software reset
	reset();

	// Enabling all measurements for ease of use
	set_temperature_oversampling(TemperatureOversampling::Temperature_ovrs_1x);
	set_pressure_oversampling(PressureOversampling::Pressure_ovrs_1x);
	set_humidity_oversampling(HumidityOversampling::Humidity_ovrs_1x);
}

void BME280::reset()
{
	// Sending reset command to the device
	const uint8_t resetWord = BME280_RESET_WORD;
	register_write(BME280_RESET, &resetWord, 1);
}

BME280::Mode BME280::get_mode()
{
	// Reading content of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

	// Returning measurement mode
	return (BME280::Mode)(ctrl_meas & BME280_CTRL_MEAS_MODE);
}

void BME280::set_mode(Mode mode)
{
	// Reading content of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

    // Writing MODE bits of CTRL_MEAS register
	ctrl_meas &= ~BME280_CTRL_MEAS_MODE;
	ctrl_meas |= mode;

	// Writing changes to the device
	register_write(BME280_CTRL_MEAS, &ctrl_meas, 1);
}

BME280::StandbyTime BME280::get_standby_time()
{
	// Reading T_SB bits of CONFIG register
	uint8_t config = 0;
	register_read(BME280_CONFIG, &config, 1);

	// Returning standby time
	return (BME280::StandbyTime)(config & BME280_CONFIG_T_SB);
}

void BME280::set_standby_time(StandbyTime standbyTime)
{
	// Reading T_SB bits of CONFIG register
	uint8_t config = 0;
	register_read(BME280_CONFIG, &config, 1);

	// Writing T_SB bits of CONFIG register
	config &= ~BME280_CONFIG_T_SB;
	config |= standbyTime;

	// Writing changes to the device
	register_write(BME280_CONFIG, &config, 1);
}

uint8_t BME280::get_measurement_delay_ms()
{
    // Reading current oversampling configuration
    uint8_t temperature_ovrs = get_temperature_oversampling();
    uint8_t pressure_ovrs = get_pressure_oversampling();
    uint8_t humidity_ovrs = get_humidity_oversampling();

    // Parsing configuration
	switch(temperature_ovrs) {
	case BME280_TEMPERATURE_OVERSAMPLING_1X:  temperature_ovrs = 1; break;
	case BME280_TEMPERATURE_OVERSAMPLING_2X:  temperature_ovrs = 2; break;
	case BME280_TEMPERATURE_OVERSAMPLING_4X:  temperature_ovrs = 4; break;
	case BME280_TEMPERATURE_OVERSAMPLING_8X:  temperature_ovrs = 8; break;
	case BME280_TEMPERATURE_OVERSAMPLING_16X: temperature_ovrs = 16; break;
	case BME280_TEMPERATURE_DISABLED:         temperature_ovrs = 0; break;
	};

	switch(pressure_ovrs) {
	case BME280_PRESSURE_OVERSAMPLING_1X:  pressure_ovrs = 1; break;
	case BME280_PRESSURE_OVERSAMPLING_2X:  pressure_ovrs = 2; break;
	case BME280_PRESSURE_OVERSAMPLING_4X:  pressure_ovrs = 4; break;
	case BME280_PRESSURE_OVERSAMPLING_8X:  pressure_ovrs = 8; break;
	case BME280_PRESSURE_OVERSAMPLING_16X: pressure_ovrs = 16; break;
	case BME280_PRESSURE_DISABLED:         pressure_ovrs = 0; break;
	};

	switch(humidity_ovrs) {
	case BME280_HUMIDITY_OVERSAMPLING_1X:  humidity_ovrs = 1; break;
	case BME280_HUMIDITY_OVERSAMPLING_2X:  humidity_ovrs = 2; break;
	case BME280_HUMIDITY_OVERSAMPLING_4X:  humidity_ovrs = 4; break;
	case BME280_HUMIDITY_OVERSAMPLING_8X:  humidity_ovrs = 8; break;
	case BME280_HUMIDITY_OVERSAMPLING_16X: humidity_ovrs = 16; break;
	case BME280_HUMIDITY_DISABLED:         humidity_ovrs = 0; break;
	};

	// Calculating measurement delay components (values from the official datasheet)
	double temperature_delay = temperature_ovrs ? 2.3f * temperature_ovrs : 0;
	double pressure_delay = pressure_ovrs ? 2.3f * pressure_ovrs + 0.575 : 0;
	double humidity_delay = humidity_ovrs ? 2.3f * humidity_ovrs + 0.575 : 0;

	return 1.25 + temperature_delay + pressure_delay + humidity_delay;
}

void BME280::set_filter_coefficient(FilterCoefficient coefficient)
{
    // Reading FILTER bits of CONFIG register
	uint8_t config = 0;
	register_read(BME280_CONFIG, &config, 1);

    // Writing FILTER bits of CONFIG register
	config &= ~BME280_CONFIG_FILTER;
	config |= coefficient;

	// Writing changes to the device
	register_write(BME280_CONFIG, &config, 1);
}

BME280::FilterCoefficient BME280::get_filter_coefficient()
{
    // Reading FILTER bits of CONFIG register
	uint8_t config = 0;
	register_read(BME280_CONFIG, &config, 1);

	// Returning filter coefficient setting
    return (BME280::FilterCoefficient)(config & BME280_CONFIG_FILTER);
}

double BME280::get_temperature(void)
{
	// Reading temperature measurement registers
	uint8_t temp_xlsb = 0, temp_lsb = 0, temp_msb = 0;
	register_read(BME280_TEMP_XLSB, &temp_xlsb, 1);
	register_read(BME280_TEMP_LSB, &temp_lsb, 1);
	register_read(BME280_TEMP_MSB, &temp_msb, 1);

	// Preparing measurement register data
	uint32_t regData = temp_xlsb >> 4 |
					   temp_lsb  << 4 |
					   temp_msb  << 12;

	// Compensation algorithm from Bosch Sensortech
	// https://github.com/BoschSensortec/BME280_driver

	double var1;
	double var2;
	double temperature;
	double temperature_min = -40;
	double temperature_max = 85;

	var1 = ((double)regData) / 16384.0 - ((double)m_calibration.dig_T1) / 1024.0;
	var1 = var1 * ((double)m_calibration.dig_T2);
	var2 = (((double)regData) / 131072.0 - ((double)m_calibration.dig_T1) / 8192.0);
	var2 = (var2 * var2) * ((double)m_calibration.dig_T3);
	m_calibration.t_fine = (int32_t)(var1 + var2);
	temperature = (var1 + var2) / 5120.0;

	if      (temperature < temperature_min) temperature = temperature_min;
	else if (temperature > temperature_max) temperature = temperature_max;

	return temperature;
}

void BME280::set_temperature_oversampling(TemperatureOversampling oversampling)
{
    // Reading OSRS_T bits of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

    // Writing OSRS_T bits of CTRL_MEAS register
	ctrl_meas &= ~BME280_CTRL_MEAS_OSRS_T;
	ctrl_meas |= oversampling;

	// Writing changes to the device
	register_write(BME280_CTRL_MEAS, &ctrl_meas, 1);
}

BME280::TemperatureOversampling BME280::get_temperature_oversampling()
{
    // Reading OSRS_T bits of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

	// Returning temperature oversampling setting
    return (BME280::TemperatureOversampling)(ctrl_meas & BME280_CTRL_MEAS_OSRS_T);
}

double BME280::get_pressure(void)
{
	// Reading pressure measurement registers
	uint8_t press_xlsb = 0, press_lsb = 0, press_msb = 0;
	register_read(BME280_PRESS_XLSB, &press_xlsb, 1);
	register_read(BME280_PRESS_LSB, &press_lsb, 1);
	register_read(BME280_PRESS_MSB, &press_msb, 1);

	// Preparing measurement register data
	uint32_t regData = press_xlsb >> 4 |
					   press_lsb  << 4 |
			           press_msb  << 12;

	// Compensation algorithm from Bosch Sensortech
	// https://github.com/BoschSensortec/BME280_driver

	double var1;
	double var2;
	double var3;
	double pressure;
	double pressure_min = 30000.0;
	double pressure_max = 110000.0;

	var1 = ((double)m_calibration.t_fine / 2.0) - 64000.0;
	var2 = var1 * var1 * ((double)m_calibration.dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)m_calibration.dig_P5) * 2.0;
	var2 = (var2 / 4.0) + (((double)m_calibration.dig_P4) * 65536.0);
	var3 = ((double)m_calibration.dig_P3) * var1 * var1 / 524288.0;
	var1 = (var3 + ((double)m_calibration.dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0) * ((double)m_calibration.dig_P1);

	pressure = 1048576.0 - (double) regData;
	pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)m_calibration.dig_P9) * pressure * pressure / 2147483648.0;
	var2 = pressure * ((double)m_calibration.dig_P8) / 32768.0;
	pressure = pressure + (var1 + var2 + ((double)m_calibration.dig_P7)) / 16.0;

	if      (pressure < pressure_min) pressure = pressure_min;
	else if (pressure > pressure_max) pressure = pressure_max;

	return pressure;
}

void BME280::set_pressure_oversampling(PressureOversampling oversampling)
{
    // Reading OSRS_P bits of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

    // Writing OSRS_P bits of CTRL_MEAS register
	ctrl_meas &= ~BME280_CTRL_MEAS_OSRS_P;
	ctrl_meas |= oversampling;

	// Writing changes to the device
	register_write(BME280_CTRL_MEAS, &ctrl_meas, 1);
}

BME280::PressureOversampling BME280::get_pressure_oversampling()
{
    // Reading OSRS_P bits of CTRL_MEAS register
	uint8_t ctrl_meas = 0;
	register_read(BME280_CTRL_MEAS, &ctrl_meas, 1);

	// Returning pressure oversampling setting
    return (BME280::PressureOversampling)(ctrl_meas & BME280_CTRL_MEAS_OSRS_P);
}

double BME280::get_humidity(void)
{
	// Reading humidity measurement registers
	uint8_t hum_lsb = 0, hum_msb = 0;
	register_read(BME280_HUM_LSB, &hum_lsb, 1);
	register_read(BME280_HUM_MSB, &hum_msb, 1);

	// Preparing measurement register data
	uint32_t regData = hum_lsb | hum_msb << 8;

	// Compensation algorithm from Bosch Sensortech
	// https://github.com/BoschSensortec/BME280_driver

	double humidity;
	double humidity_min = 0.0;
	double humidity_max = 100.0;
	double var1;
	double var2;
	double var3;
	double var4;
	double var5;
	double var6;

	var1 = ((double)m_calibration.t_fine) - 76800.0;
	var2 = (((double)m_calibration.dig_H4) * 64.0 + (((double)m_calibration.dig_H5) / 16384.0) * var1);
	var3 = regData - var2;
	var4 = ((double)m_calibration.dig_H2) / 65536.0;
	var5 = (1.0 + (((double)m_calibration.dig_H3) / 67108864.0) * var1);
	var6 = 1.0 + (((double)m_calibration.dig_H6) / 67108864.0) * var1 * var5;
	var6 = var3 * var4 * (var5 * var6);
	humidity = var6 * (1.0 - ((double)m_calibration.dig_H1) * var6 / 524288.0);

	if      (humidity > humidity_max) humidity = humidity_max;
	else if (humidity < humidity_min) humidity = humidity_min;

	return humidity;
}

void BME280::set_humidity_oversampling(HumidityOversampling oversampling)
{
    // Reading OSRS_H bits of CTRL_HUM register
	uint8_t ctrl_hum = 0;
	register_read(BME280_CTRL_HUM, &ctrl_hum, 1);

    // Writing OSRS_H bits of CTRL_HUM register
	ctrl_hum &= ~BME280_CTRL_HUM_OSRS_H;
	ctrl_hum |= oversampling;

	// Writing changes to the device
	register_write(BME280_CTRL_HUM, &ctrl_hum, 1);
}

BME280::HumidityOversampling BME280::get_humidity_oversampling()
{
    // Reading OSRS_H bits of CTRL_HUM register
	uint8_t ctrl_hum = 0;
	register_read(BME280_CTRL_HUM, &ctrl_hum, 1);

	// Returning humidity oversampling setting
    return (BME280::HumidityOversampling)(ctrl_hum & BME280_CTRL_HUM_OSRS_H);
}

void BME280::register_read(uint8_t registerAddress, uint8_t buffer[], uint8_t byteCount) {

	// Checking the type of the communication interface
	if(m_interface->getType() == DriverInterface::Type::I2C) {

		// Locking the interface
		((I2C_Master*)m_interface)->lock();

		// Start condition
		((I2C_Master*)m_interface)->queue_start();

		// I2C bus address
		((I2C_Master*)m_interface)->queue_write_byte((BME280_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);

		// Device register address
		((I2C_Master*)m_interface)->queue_write_byte(registerAddress, true);

		// Restart condition
		((I2C_Master*)m_interface)->queue_start();

		// I2C bus address
		((I2C_Master*)m_interface)->queue_write_byte((BME280_I2C_ADDR << 1) | I2C_MASTER_READ, true);

		// Reading data
		((I2C_Master*)m_interface)->queue_read(buffer, byteCount, I2C_Master::acknowledge_t::nack_last);

		// Stop condition
		((I2C_Master*)m_interface)->queue_stop();

		// Executing command sequence
		((I2C_Master*)m_interface)->execute();

		// Unlocking the interface
		((I2C_Master*)m_interface)->unlock();
	}
}

void BME280::register_write(uint8_t registerAddress, const uint8_t buffer[], uint8_t byteCount)
{
	// Checking the type of the communication interface
	if(m_interface->getType() == DriverInterface::Type::I2C) {

		// Locking the interface
		((I2C_Master*)m_interface)->lock();

		// Start condition
		((I2C_Master*)m_interface)->queue_start();

		// I2C bus address
		((I2C_Master*)m_interface)->queue_write_byte((BME280_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);

		// Sending data
		for(uint8_t i = 0; i < byteCount; i++) {

			// Writing register address
			((I2C_Master*)m_interface)->queue_write_byte(registerAddress++, true);

			// Writing register data
			((I2C_Master*)m_interface)->queue_write_byte(buffer[i], true);
		}

		// Stop condition
		((I2C_Master*)m_interface)->queue_stop();

		// Executing command sequence
		((I2C_Master*)m_interface)->execute();

		// Unlocking the interface
		((I2C_Master*)m_interface)->unlock();
	}
}
