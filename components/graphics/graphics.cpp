#include "graphics.h"
#include "font6x6.h"


GraphicsDevice::GraphicsDevice(DriverInterface* interface, uint16_t width, uint16_t height)
	: m_interface(interface),
	  m_width(width),
	  m_height(height),
	  m_text_cursor_x(0),
	  m_text_cursor_y(0),
	  m_font({font6x6_descriptors, font6x6_data, 6})
{}

void GraphicsDevice::draw_pixel(uint16_t x, uint16_t y, Color color)
{
	// Delegating to derived class implementation
	write_pixel(x, y, color);
}

void GraphicsDevice::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
	// Checking if can be implemented efficiently
	if (x0 == x1) {
		draw_line_vertical(x0, y0 < y1 ? y0 : y1, abs(y1 - y0), color); return;
	}
	if (y0 == y1) {
		draw_line_horizontal(x0 < x1 ? x0 : x1, y0, abs(x1 - x0), color); return;
	}

	// Bresenham's Line Drawing Algorithm
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) draw_line_bresenham_low(x1, y1, x0, y0, color);
		else         draw_line_bresenham_low(x0, y0, x1, y1, color);
	}
	else {
		if (y0 > y1) draw_line_bresenham_high(x1, y1, x0, y0, color);
		else         draw_line_bresenham_high(x0, y0, x1, y1, color);
	}
}

void GraphicsDevice::draw_circle(uint16_t x, uint16_t y, uint16_t radius, Color color)
{
	// Drawing circle at coordinates at roughly radius distance from the center point
	for (uint16_t i = 0; i < m_width; i++) {
		for (uint16_t j = 0; j < m_height; j++) {
			uint16_t distance = (uint16_t) sqrt((x - i) * (x - i) + (y - j) * (y - j));
			if (distance == radius) write_pixel(i, j, color);
		}
	}
}

void GraphicsDevice::draw_filled_circle(uint16_t x, uint16_t y, uint16_t radius, Color color)
{
	// Drawing filled circle at coordinates closer to the center point than radius
	for (uint16_t i = 0; i < m_width; i++) {
		for (uint16_t j = 0; j < m_height; j++) {
			uint16_t distance = (uint16_t) sqrt((x - i) * (x - i) + (y - j) * (y - j));
			if (distance <= radius) write_pixel(i, j, color);
		}
	}
}

void GraphicsDevice::draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
{
	// Drawing rectangle by checking if the current coordinate is one of the edges
	for (uint16_t i = 0; i < width; i++) {
		for (uint16_t j = 0; j < height; j++) {
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
				write_pixel(x + i, y + j, color);
			}
		}
	}
}

#include "esp_log.h"

void GraphicsDevice::draw_rectangle_coordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
	uint16_t top_left_x = x0 < x1 ? x0 : x1;
	uint16_t top_left_y = y0 < y1 ? y0 : y1;
	uint16_t bottom_right_x = x0 < x1 ? x1 : x0;
	uint16_t bottom_right_y = y0 < y1 ? y1 : y0;

	draw_rectangle(top_left_x, top_left_y, bottom_right_x - top_left_x, bottom_right_y - top_left_y, color);
}

void GraphicsDevice::draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
{
	// Drawing filled rectangle by coloring all coordinates inside the shape bounds
	for (uint16_t i = 0; i < width; i++) {
		for (uint16_t j = 0; j < height; j++) {
			write_pixel(x + i, y + j, color);
		}
	}
}

void GraphicsDevice::draw_filled_rectangle_coordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
	uint16_t top_left_x = x0 < x1 ? x0 : x1;
	uint16_t top_left_y = y0 < y1 ? y0 : y1;
	uint16_t bottom_right_x = x0 < x1 ? x1 : x0;
	uint16_t bottom_right_y = y0 < y1 ? y1 : y0;

	draw_filled_rectangle(top_left_x, top_left_y, bottom_right_x - top_left_x, bottom_right_y - top_left_y, color);
}

void GraphicsDevice::draw_symbol(uint16_t x, uint16_t y, Symbol symbol)
{
	// Calculating coordinate limits
	uint16_t limitHorizontal = x + symbol.width + symbol.paddingHorizontal;
	uint16_t limitVertical = y + symbol.height + symbol.paddingVertical;

	for (uint16_t j = y + symbol.paddingVertical; j < limitVertical; j++) {
		for (uint16_t i = x + symbol.paddingHorizontal; i < limitHorizontal; i++) {

			// Calculating pixel databit position
			uint32_t byteIndex = symbol.offset / 8;
			uint8_t  bitIndex  = symbol.offset % 8;

			// Printing next pixel
			if (m_font.fontData[byteIndex] & (1 << (7 - bitIndex))) {
				write_pixel(i, j, Color::white());
			}
			else {
				write_pixel(i, j, Color::black());
			}

			// Increasing offset
			symbol.offset++;
		}
	}
}

void GraphicsDevice::draw_symbol(uint16_t x, uint16_t y, uint16_t id)
{
	draw_symbol(x, y, m_font.fontDescriptors[id]);
}

void GraphicsDevice::set_text_cursor(uint16_t x, uint16_t y)
{
	m_text_cursor_x = x;
	m_text_cursor_y = y;
}

void GraphicsDevice::set_fonts(Font font)
{
	m_font = font;
}

void GraphicsDevice::print_text(const char* text)
{
	while (*text != '\0') {

		// Handling newline character
		if (*text == '\n') {
			m_text_cursor_x = 0;
			m_text_cursor_y += m_font.fontHeight;
			text++;
			continue;
		}

		// Handling unprintable characters
		else if(*text < 32) {
			text++;
			continue;
		}

		// Getting the ID of the next character
		uint16_t id = *text++ - 32;

		// Getting symbol to print
		Symbol symbol = m_font.fontDescriptors[id];

		// Checking display bounds
		if (m_text_cursor_x + symbol.width > m_width) {
			m_text_cursor_x = 0;
			m_text_cursor_y += symbol.height + symbol.paddingVertical;
		}

		// Printing symbol
		draw_symbol(m_text_cursor_x, m_text_cursor_y, id);

		// Moving cursor
		m_text_cursor_x += symbol.width + symbol.paddingHorizontal;
	}
}

void GraphicsDevice::draw_line_horizontal(uint16_t x, uint16_t y, uint16_t length, Color color)
{
	// Drawing horizontal line
	for (uint16_t i = 0; i < length; i++) {
		write_pixel(x + i, y, color);
	}
}

void GraphicsDevice::draw_line_vertical(uint16_t x, uint16_t y, uint16_t height, Color color)
{
	// Drawing vertical line
	for (uint16_t i = 0; i < height; i++) {
		write_pixel(x, y + i, color);
	}
}

void GraphicsDevice::draw_line_bresenham_high(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
	// Bresenham's Line Drawing Algorithm for positive slopes
	int32_t dx = x1 - x0;
	int32_t dy = y1 - y0;
	int32_t xi = 1;

	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int32_t D = 2 * dx - dy;
	int32_t x = x0;

	for (int32_t y = y0; y < y1; y++) {
		write_pixel(x, y, color);
		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}

		D = D + 2 * dx;
	}
}

void GraphicsDevice::draw_line_bresenham_low(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{
	// Bresenham's Line Drawing Algorithm for negative slopes
	int32_t dx = x1 - x0;
	int32_t dy = y1 - y0;
	int32_t yi = 1;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int32_t D = 2 * dy - dx;
	int32_t y = y0;

	for (int32_t x = x0; x < x1; x++) {
		write_pixel(x, y, color);
		if (D > 0) {
			y = y + yi;
			D = D - 2 * dx;
		}

		D = D + 2 * dy;
	}
}
