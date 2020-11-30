#pragma once
#ifndef DRIVERS_GRAPHICS_H_INCLUDED
#define DRIVERS_GRAPHICS_H_INCLUDED

// Standard includes
#include <cstdint>
#include <cmath>

// Project includes
#include "driver_interface.h"


/**
 *
 */
class Color {
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	inline static Color white() { return {255, 255, 255}; }
	inline static Color black() { return { 0, 0, 0 }; }
};

/**
 * This structure describes the properties of a graphics symbol defined
 * in a byte-array. Information is provided where to find it in the stream,
 * and also the dimensions and padding options.
 */
struct Symbol {
	uint32_t offset;			/**< Bit offset from start of datastream. */
	uint8_t  width;				/**< Width of the symbol in pixels.       */
	uint8_t  height;			/**< Height of the symbol in pixels.      */
	uint8_t  paddingHorizontal;	/**< Horizontal padding in pixels.        */
	uint8_t  paddingVertical;	/**< Vertical padding in pixels.          */
};

/**
 * This structure describes the properties of a graphics font, which is a
 * collection on graphics symbols ordered in ASCII order.
 */
struct Font {
	const Symbol*  fontDescriptors; /**< Descriptors for the symbols.        */
	const uint8_t* fontData;		/**< Bytestream for the font symbols.    */
	uint8_t  fontHeight;	        /**< Height of the characters in pixels. */
};

/**
 *
 */
class GraphicsDevice {
public:

	// Construction & destruction

	/**
	 * Constructs a GraphicsDevice with the specified dimensions and interface.
	 * @param interface [in] The device communication interface.
	 * @param width     [in] The width of the display in pixels.
	 * @param height    [in] The height of the display in pixels.
	 */
	GraphicsDevice(DriverInterface* interface, uint16_t width, uint16_t height);

	/**
	 * Destroys the graphics device.
	 */
	inline virtual ~GraphicsDevice() {}

	// Pure virtual interface to implement by graphics devices

	/**
	 * Initializes the graphics device.
	 */
	virtual void initialize() = 0;

	/**
	 * Applies the changes in memory to the display.
	 */
	virtual void refresh() = 0;

	/**
	 * Clears the display with the default color.
	 */
	virtual void clear() = 0;

	/**
	 * Writes one pixel of data into the device memory.
	 * @param x     [in] The X coordinate of the pixel to draw.
	 * @param y     [in] The Y coordinate of the pixel to draw.
	 * @param color [in] The color of the pixel to draw.
	 */
	virtual void write_pixel(uint16_t x, uint16_t y, Color color) = 0;

	// Pre-implemented interface

	/**
	 * Draws a pixel to the specified display coordinates.
	 * @param x     [in] The X coordinate of the pixel to draw.
	 * @param y     [in] The Y coordinate of the pixel to draw.
	 * @param color [in] The color of the pixel to draw.
	 */
	void draw_pixel(uint16_t x, uint16_t y, Color color);

	/**
	 * Draws a line between the specified coordinates.
	 * @param x0    [in] The starting X coordinate of the line.
	 * @param y0    [in] The starting Y coordinate of the line.
	 * @param x1    [in] The ending X coordinate of the line.
	 * @param y1    [in] The ending Y coordinate of the line.
	 * @param color [in] The color of the line.
	 */
	void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);

	/**
	 * Draws a circle outline at the specified coordinates.
	 * @param x      [in] The center X coordinate of the circle.
	 * @param y      [in] The center Y coordinate of the circle.
	 * @param radius [in] The radius of the circle.
	 * @param color  [in] The color of the circle outline.
	 */
	void draw_circle(uint16_t x, uint16_t y, uint16_t radius, Color color);

	/**
	 * Draws a filled circle at the specified coordinates.
	 * @param x      [in] The center X coordinate of the circle.
	 * @param y      [in] The center Y coordinate of the circle.
	 * @param radius [in] The radius of the circle.
	 * @param color  [in] The color of the circle.
	 */
	void draw_filled_circle(uint16_t x, uint16_t y, uint16_t radius, Color color);

	/**
	 * Draws a rectangle outline at the specified coordinates.
	 * @param x      [in] The upper left X coordinate of the rectangle.
	 * @param y      [in] The upper left Y coordinate of the rectangle.
	 * @param width  [in] The width of the rectangle.
	 * @param height [in] The height of the rectangle.
	 * @param color  [in] The color of the rectangle outline.
	 */
	void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);

	/**
	 * Draws a rectangle with the specified coordinates as the opposite corners.
	 * @param x0    [in] The first corner X coordinate of the rectangle.
	 * @param y0    [in] The first corner Y coordinate of the rectangle.
	 * @param x1    [in] The opposite corner X coordinate of the rectangle.
	 * @param y1    [in] The opposite corner Y coordinate of the rectangle.
	 * @param color [in] The color of the rectangle outline.
	 */
	void draw_rectangle_coordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);

	/**
	 * Draws a filled rectangle at the specified coordinates.
	 * @param x      [in] The upper left X coordinate of the rectangle.
	 * @param y      [in] The upper left Y coordinate of the rectangle.
	 * @param width  [in] The width of the rectangle.
	 * @param height [in] The height of the rectangle.
	 * @param color  [in] The color of the rectangle.
	 */
	void draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);

	/**
	 * Draws a filled rectangle with the specified coordinates as the opposite corners.
	 * @param x0    [in] The first corner X coordinate of the rectangle.
	 * @param y0    [in] The first corner Y coordinate of the rectangle.
	 * @param x1    [in] The opposite corner X coordinate of the rectangle.
	 * @param y1    [in] The opposite corner Y coordinate of the rectangle.
	 * @param color [in] The color of the rectangle outline.
	 */
	void draw_filled_rectangle_coordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);

	/**
	 * Draws a graphical symbol at the specified coordinates.
	 * @param x      [in] The upper left X coordinate of the symbol.
	 * @param y      [in] The upper left Y coordinate of the symbol.
	 * @param symbol [in] The descriptor for the symbol.
	 */
	void draw_symbol(uint16_t x, uint16_t y, Symbol symbol);

	void draw_symbol(uint16_t x, uint16_t y, uint16_t id);

	/**
	 * Sets the text printing cursor position.
	 * @param x [in] The new X position (upper left) of the text cursor.
	 * @param y [in] The new Y position (upper left) of the text cursor.
	 */
	void set_text_cursor(uint16_t x, uint16_t y);

	/**
	 * Sets the symbol fonts used by the graphics device.
	 * @param font [in] The new symbol fonts to use.
	 */
	void set_fonts(Font font);

	/**
	 * Prints a C-style array of ASCII-characters at the text cursor position.
	 * @param text [in]
	 */
	void print_text(const char* text);

protected:

	DriverInterface* m_interface;
	uint16_t         m_width;
	uint16_t         m_height;
	uint16_t         m_text_cursor_x;
	uint16_t         m_text_cursor_y;
	Font             m_font;

	// Helper interfaces for derived classes

	/**
	 *
	 * @param x0
	 * @param y0
	 * @param x1
	 * @param y1
	 * @param color
	 */
	void draw_line_horizontal(uint16_t x, uint16_t y, uint16_t length, Color color);

	/**
	 *
	 * @param x0
	 * @param y0
	 * @param x1
	 * @param y1
	 * @param color
	 */
	void draw_line_vertical(uint16_t x, uint16_t y, uint16_t height, Color color);

	/**
	 *
	 * @param x0
	 * @param y0
	 * @param x1
	 * @param y1
	 * @param color
	 */
	void draw_line_bresenham_high(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);

	/**
	 *
	 * @param x0
	 * @param y0
	 * @param x1
	 * @param y1
	 * @param color
	 */
	void draw_line_bresenham_low(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color);
};

#endif // DRIVERS_GRAPHICS_H_INCLUDED
