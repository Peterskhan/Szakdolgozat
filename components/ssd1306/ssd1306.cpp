#include "ssd1306.h"


SSD1306::SSD1306(DriverInterface* interface, uint16_t width, uint16_t height)
	: GraphicsDevice(interface, width, height)
{
	// Initializing display
	initialize();
}

void SSD1306::initialize()
{
	// Initializing command sequence
	uint8_t commands[] = {
		SSD1306_DISPLAY_OFF,
		SSD1306_CLOCK_DIV_OSC,
		SSD1306_CLOCK_DIV_OSC_DEFAULT,
		SSD1306_MULTIPLEX_RATIO,
		SSD1306_MULTIPLEX_RATIO_DEFAULT,
		SSD1306_DISPLAY_OFFSET,
		SSD1306_DISPLAY_OFFSET_DEFAULT,
		SSD1306_DISPLAY_START_LINE | SSD1306_DISPLAY_START_LINE_DEFAULT,
		SSD1306_CHARGEPUMP,
		SSD1306_CHARGEPUMP_ON,
		SSD1306_MEMORY_MODE,
		SSD1306_MEMORY_MODE_HORIZONTAL,
		SSD1306_SEGMENT_REMAP_OFF,
		SSD1306_COM_SCAN_INC,
		SSD1306_COM_PINS,
		SSD1306_COM_PINS_DEFAULT,
		SSD1306_CONTRAST,
		SSD1306_CONTRAST_DEFAULT,
		SSD1306_PRECHARGE_PERIOD,
		SSD1306_PRECHARGE_PERIOD_DEFAULT,
		SSD1306_VCOM_DESELECT,
		SSD1306_VCOM_DESELECT_DEFAULT,
		SSD1306_LOWER_COLUMN_ADDR_START,
		SSD1306_HIGHER_COLUMN_ADDR_START,
		SSD1306_ENTIRE_DISPLAY_RESUME,
		SSD1306_DISPLAY_NORMAL,
		SSD1306_DISPLAY_ON
	};

	// Sending initialization commands
	write_commands(commands, 25);

	// Clearing display
	// clear();
}

void SSD1306::refresh()
{
	// Column address ranges from the first column to the last column
	uint8_t columnsMax = m_width - 1;

	// Page address ranges (blocks of 8 rows) from the first row to the last
	uint8_t pagesMax = (m_height / 8) - 1;

	// Commands to set the display data dimensions
	uint8_t commands[] = {
			SSD1306_COLUMN_ADDR, 0, columnsMax,
			SSD1306_PAGE_ADDR,   0, pagesMax
	};

	// Sending display dimension settings to the device
	write_commands(commands, sizeof(commands));

	// Sending data to the device
	write_data(m_pixelData, 1024);
}

void SSD1306::clear()
{
	// Clearing shadow buffer content
	for (uint32_t i = 0; i < 1024; i++) {
		m_pixelData[i] = 0x00;
	}

	// Refreshing the display to apply changes
	refresh();
}

void SSD1306::write_pixel(uint16_t x, uint16_t y, Color color)
{
	// Note: This method works for horizontal addressing mode only.

	// Coordinate bounds check
	if (x > m_width - 1 || y > m_height - 1) return;

	// Calculating page and column index
	uint8_t page = y / 8;
	uint8_t column = x;

	// Calculating byte index in pixelData & bitshift value
	uint16_t byteIndex = page * m_width + column;
	uint8_t bitIndex = y % 8;

	// Setting or clearing the selected bit
    if (color.r != 0 || color.g != 0 || color.b != 0) m_pixelData[byteIndex] |= (1 << bitIndex);
	else                                              m_pixelData[byteIndex] &= ~(1 << bitIndex);
}

void SSD1306::buffer_write(uint8_t type, uint8_t buffer[], uint16_t byteCount)
{
	// Checking the type of the communication interface
	if(m_interface->getType() == DriverInterface::Type::I2C) {

		// Locking the interface
		((I2C_Master*)m_interface)->lock();

		// Start condition
		((I2C_Master*)m_interface)->queue_start();

		// I2C bus address
		((I2C_Master*)m_interface)->queue_write_byte((SSD1306_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);

		// Writing message type (SSD1306_COMMAND or SSD1306_DATA)
		((I2C_Master*)m_interface)->queue_write_byte(type, true);

		// Writing register data
		((I2C_Master*)m_interface)->queue_write(buffer, byteCount, true);

		// Stop condition
		((I2C_Master*)m_interface)->queue_stop();

		// Executing command sequence
		((I2C_Master*)m_interface)->execute();

		// Unlocking the interface
		((I2C_Master*)m_interface)->unlock();
	}
}

void SSD1306::write_commands(uint8_t commands[], uint8_t count)
{
	// Writing command bytes prefixed with command identifier
	buffer_write(SSD1306_COMMAND, commands, count);
}

void SSD1306::write_data(uint8_t data[], uint16_t count)
{
	// Writing data bytes prefixed with data identifier
	buffer_write(SSD1306_DATA, data, count);
}
