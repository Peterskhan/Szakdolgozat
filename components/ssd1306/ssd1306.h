#pragma once
#ifndef SSD1306_SSD1306_H_INCLUDED
#define SSD1306_SSD1306_H_INCLUDED

// Standard includes

// Project includes
#include "graphics.h"
#include "i2c_master.h"


/** @{ @name Default initialization settings */
 /** @brief Defines default value for clock division and oscillator frequency. */
#define SSD1306_CLOCK_DIV_OSC_DEFAULT       (uint8_t)(0x80)
/** @brief Defines default value for multiplex ratio. */
#define SSD1306_MULTIPLEX_RATIO_DEFAULT     (uint8_t)(0x3F)
/** @brief Defines default value for display offset. */
#define SSD1306_DISPLAY_OFFSET_DEFAULT      (uint8_t)(0x00)
/** @brief Defines default value for display start line. */
#define SSD1306_DISPLAY_START_LINE_DEFAULT  (uint8_t)(0x00)
/** @brief Defines default value for COM pins settings. */
#define SSD1306_COM_PINS_DEFAULT            (uint8_t)(0xDA)
/** @brief Defines default value for contrast. */
#define SSD1306_CONTRAST_DEFAULT            (uint8_t)(0xCF)
/** @brief Defines default value for precharge period. */
#define SSD1306_PRECHARGE_PERIOD_DEFAULT    (uint8_t)(0xF1)
/** @brief Defines default value for VCOM deselect settings. */
#define SSD1306_VCOM_DESELECT_DEFAULT       (uint8_t)(0x40)
/** @} */

/** @{ @name I2C parameters */
/** @brief Defines the default I2C address. */
#define SSD1306_I2C_ADDR                    (uint8_t)(0x3C)
/** @brief Defines the control byte for commands. */
#define SSD1306_COMMAND                     (uint8_t)(0x00)
/** @brief Defines the control byte for data. */
#define SSD1306_DATA                        (uint8_t)(0x40)
/** @brief Defines I2C read operation. */
#define SSD1306_I2C_READ                    (uint8_t)(0x01)
/** @brief Defines I2C write operation. */
#define SSD1306_I2C_WRITE                   (uint8_t)(0x00)
/** @} */

/** @{ @name Fundamental commands */
/** @brief Defines the command byte for contrast settings. */
#define SSD1306_CONTRAST                    (uint8_t)(0x81)
/** @brief Defines the command byte for entire display mode OFF. */
#define SSD1306_ENTIRE_DISPLAY_RESUME       (uint8_t)(0xA4)
/** @brief Defines the command byte for entire display mode ON. */
#define SSD1306_ENTIRE_DISPLAY_ON           (uint8_t)(0xA5)
/** @brief Defines the command byte for normal display mode. */
#define SSD1306_DISPLAY_NORMAL              (uint8_t)(0xA6)
/** @brief Defines the command byte for inverted display mode. */
#define SSD1306_DISPLAY_INVERSE             (uint8_t)(0xA7)
/** @brief Defines the command byte for display ON. */
#define SSD1306_DISPLAY_ON                  (uint8_t)(0xAE)
/** @brief Defines the command byte for display OFF. */
#define SSD1306_DISPLAY_OFF                 (uint8_t)(0xAF)
/** @} */

// Scrolling functionality is currently not supported.
/** @{ @name Scrolling commands */
/** @} */

/** @{ @name Addressing commands */
/** @brief Defines the command byte for setting lower nibble of column start address. */
#define SSD1306_LOWER_COLUMN_ADDR_START     (uint8_t)(0x00)
/** @brief Defines the command byte for setting higher nibble of column start address. */
#define SSD1306_HIGHER_COLUMN_ADDR_START    (uint8_t)(0x10)
/** @brief Defines the command byte for setting memory addressing mode. */
#define SSD1306_MEMORY_MODE                 (uint8_t)(0x20)
/** @brief Defines the command byte for horizontal addressing mode. */
#define SSD1306_MEMORY_MODE_HORIZONTAL      (uint8_t)(0x00)
/** @brief Defines the command byte for vertical addressing mode. */
#define SSD1306_MEMORY_MODE_VERTICAL        (uint8_t)(0x01)
/** @brief Defines the command byte for page addressing mode. */
#define SSD1306_MEMORY_MODE_PAGE            (uint8_t)(0x02)
/** @brief Defines the command byte for setting column address. */
#define SSD1306_COLUMN_ADDR                 (uint8_t)(0x21)
/** @brief Defines the command byte for setting page address. */
#define SSD1306_PAGE_ADDR                   (uint8_t)(0x22)
/** @brief Defines the command byte for setting page address start. */
#define SSD1306_PAGE_ADDR_START             (uint8_t)(0xB0)
/** @} */

/** @{ @name Hardware configuration commands */
/** @brief Defines the command byte for display start line settings. */
#define SSD1306_DISPLAY_START_LINE          (uint8_t)(0x40)
/** @brief Defines the command byte for segment remapping OFF. */
#define SSD1306_SEGMENT_REMAP_OFF           (uint8_t)(0xA0)
/** @brief Defines the command byte for segment remapping ON. */
#define SSD1306_SEGMENT_REMAP_ON            (uint8_t)(0xA1)
/** @brief Defines the command byte for multiplex ratio settings. */
#define SSD1306_MULTIPLEX_RATIO             (uint8_t)(0xA8)
/** @brief Defines the command byte for incremental COM scan. */
#define SSD1306_COM_SCAN_INC                (uint8_t)(0xC0)
/** @brief Defines the command byte for decremental COM scan. */
#define SSD1306_COM_SCAN_DEC                (uint8_t)(0xC8)
/** @brief Defines the command byte for display offset settings. */
#define SSD1306_DISPLAY_OFFSET              (uint8_t)(0xD3)
/** @brief Defines the command byte for COM pins configuration. */
#define SSD1306_COM_PINS                    (uint8_t)(0xDA)
/** @brief Defines the command byte for chargepump settings. */
#define SSD1306_CHARGEPUMP                  (uint8_t)(0x8D)
/** @brief Defines the command byte for chargepump ON. */
#define SSD1306_CHARGEPUMP_ON               (uint8_t)(0x14)
/** @brief Defines the command byte for chargepump OFF. */
#define SSD1306_CHARGEPUMP_OFF              (uint8_t)(0x10)
/** @} */

/** @{ @name Timing & Driving Scheme commands */
/** @brief Defines the command byte for clock division and oscillator settings. */
#define SSD1306_CLOCK_DIV_OSC               (uint8_t)(0xD5)
/** @brief Defines the command byte for precharge period settings. */
#define SSD1306_PRECHARGE_PERIOD            (uint8_t)(0xD9)
/** @brief Defines the command byte for VCOM deselect settings. */
#define SSD1306_VCOM_DESELECT               (uint8_t)(0xDB)
/** @} */

class SSD1306 : public GraphicsDevice {
public:

	/**
	 * Constructs an SSD1306 graphics display object.
	 * @param interface [in] The communication interface.
	 * @param width     [in] The width of the display in pixels.
	 * @param height    [in] The height of the display in pixels.
	 */
	SSD1306(DriverInterface* interface, uint16_t width = 128, uint16_t height = 64);

	/**
	 * Initializes the graphics device.
	 */
	virtual void initialize() override;

	/**
	 * Applies the changes in memory to the display.
	 */
	virtual void refresh() override;

	/**
	 * Clears the display with the default color.
	 */
	virtual void clear() override;

	/**
	 * Writes one pixel of data into the device memory.
	 * @param x     [in] The X coordinate of the pixel to draw.
	 * @param y     [in] The Y coordinate of the pixel to draw.
	 * @param color [in] The color of the pixel to draw.
	 */
	virtual void write_pixel(uint16_t x, uint16_t y, Color color) override;

private:
	uint8_t m_pixelData[1024]; /**< Display shadow-buffer. */

	/**
	 * Writes a buffer of commands or data to the device.
	 * @param type      [in] The type of the data - SSD1306_COMMAND or SSD1306_DATA.
	 * @param buffer    [in] The buffer of bytes to write to the device.
	 * @param byteCount [in] The number of bytes to write to the device.
	 */
	void buffer_write(uint8_t type, uint8_t buffer[], uint16_t byteCount);

	/**
	 *
	 * @param commands
	 * @param count
	 */
	void write_commands(uint8_t commands[], uint8_t count);

	/**
	 *
	 * @param data
	 * @param count
	 */
	void write_data(uint8_t data[], uint16_t count);
};

#endif // SSD1306_SSD1306_H_INCLUDED
