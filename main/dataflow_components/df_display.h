#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_DISPLAY_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_DISPLAY_H_INCLUDED

#include <time.h>

#include "json.h"
#include "dataflow.h"
#include "ssd1306.h"
#include "font6x6.h"
#include "font8x8.h"


/**
 * This application specific component implements the OLED display which
 * is used to show the measured/acquired data to the user. The display uses the
 * SSD1306 OLED driver through I2C.
 *
 * Ports:
 *
 * [input] "interface" - Used to receive the communication driver interface for
 *                       communicating with the OLED display. The process() method
 *                       blocks on start until the interface (DriverInterface*) is
 *                       received.
 *
 * [input] "in"        - Used to receive data to be display or control messages.
 *                       The received data is buffered in static non-volatile-memory.
 *                       The input messages should have child nodes called:
 *                       "temperature" - Double, for measured temperature.
 *                       "pressure"    - Double, for measured pressure.
 *                       "humidity"    - Double, for measured humidity.
 *                       "forecast"    - std::string, forecast data in JSON format
 *                       "battery"     - uint16_t, for raw battery ADC measurements.
 *                        ...
 */
class DF_Display : public Component {
public:

	DF_Display() : m_display(nullptr)
	{
		m_ports.addInputPort("in");
		m_ports.addInputPort("interface", 1);
	}

	virtual void process() override
	{
		// Node object for reading messages
		Node message;

		// Reading I2C driver interface port
		m_ports["interface"].receive(message);

		// Creating SSD1306 display object
		m_display = new SSD1306((DriverInterface*) message);

		while(true) {

			// Reading message from the input port
			m_ports["in"].receive(message);

			// Checking if the message contains measurement data
			if(message.has_child("temperature") && message.has_child("pressure") && message.has_child("humidity"))
			{
				s_displayData.s_temperature = (double) message["temperature"];
				s_displayData.s_pressure = (double) message["pressure"];
				s_displayData.s_humidity = (double) message["humidity"];

				if(s_displayData.s_displayState == CURRENT_WEATHER) drawCurrentWeather(*m_display);
			}

			// Checking if the message contains forecast data
			if(message.has_child("query")) {

				// Parsing the forecast data
				JsonObject data = JsonObject::parse((std::string) message["query"]);

				// Extracting data from the forecast JSON
				s_displayData.s_temperature_1 = data["forecast"][0]["temperature"]["day"].getDouble();
				s_displayData.s_temperature_2 = data["forecast"][1]["temperature"]["day"].getDouble();
				s_displayData.s_temperature_3 = data["forecast"][2]["temperature"]["day"].getDouble();
				s_displayData.s_weatherID_1 = data["forecast"][0]["id"].getInteger();
				s_displayData.s_weatherID_2 = data["forecast"][1]["id"].getInteger();
				s_displayData.s_weatherID_3 = data["forecast"][2]["id"].getInteger();
				s_displayData.s_dayIndex_1 = data["forecast"][0]["day"].getInteger();
				s_displayData.s_dayIndex_2 = data["forecast"][1]["day"].getInteger();
				s_displayData.s_dayIndex_3 = data["forecast"][2]["day"].getInteger();

				if(s_displayData.s_displayState == FORECAST) drawWeatherForecast(*m_display);
			}

			// Checking if the message contains battery data
			if(message.has_child("battery")) {
				s_displayData.s_battery = (uint16_t) message["battery"];

				if(s_displayData.s_displayState == STATUS) drawStatus(*m_display);
			}

			// Checking if the message contains NEXT SCREEN request
			if(message.hasType<int>()) {
				s_displayData.s_displayState = (displayState)((s_displayData.s_displayState + 1) % 3);

				switch(s_displayData.s_displayState) {
				case CURRENT_WEATHER: drawCurrentWeather(*m_display); break;
				case FORECAST: drawWeatherForecast(*m_display); break;
				case STATUS: drawStatus(*m_display); break;
				}
			}

			// Checking if the message contains PREVIOUS SCREEN request
			// ???
		}
	}

private:

	/**
	 * Defines the possible states of the display.
	 */
	enum displayState { CURRENT_WEATHER, FORECAST, STATUS };

	/**
	 * Defines the display data to be persistently stored during deep sleep.
	 */
	struct DisplayData {
		double       s_temperature;
		double       s_pressure;
		double       s_humidity;
		uint8_t      s_dayIndex_1;
		uint8_t      s_dayIndex_2;
		uint8_t      s_dayIndex_3;
		uint16_t     s_weatherID_1;
		uint16_t     s_weatherID_2;
		uint16_t     s_weatherID_3;
		uint16_t     s_temperature_1;
		uint16_t     s_temperature_2;
		uint16_t     s_temperature_3;
		uint16_t     s_battery;
		displayState s_displayState;
	};

	// Font sets to use for displaying text and symbols
	const Font font8x8 = {font8x8_descriptors, font8x8_data, 8+1};
	const Font font6x6 = {font6x6_descriptors, font6x6_data, 6+1};

	/**
	 * Queries the weather condition icon index for the specified weather ID.
	 * @param  weatherID [in] The weather ID to query the symbol index for.
	 * @return The symbol index for the corresponding weather symbol.
	 */
	uint16_t weather_icon(uint16_t weatherID) const noexcept {

		// Matching weather ID ranges to the corresponding FONT6X6 symbol index
		if(weatherID >= 200 && weatherID < 300) return FONT6X6_THUNDERSTORM;
		if(weatherID >= 300 && weatherID < 400) return FONT6X6_RAIN;
		if(weatherID >= 500 && weatherID < 600) return FONT6X6_RAIN;
		if(weatherID >= 600 && weatherID < 700) return FONT6X6_SNOW;
		if(weatherID >= 700 && weatherID < 800) return FONT6X6_FOGGY;
		if(weatherID == 800)                    return FONT6X6_SUNNY;
		if(weatherID == 801)                    return FONT6X6_CLOUDY_SUN;
		if(weatherID >= 802 && weatherID < 900) return FONT6X6_CLOUDS;

		// Unknown weather condition ID, return symbol index for question mark
		return '?';
	}

	/**
	 *
	 * @param display
	 */
	void drawCurrentWeather(GraphicsDevice& display) {

		display.clear();
		display.set_fonts(font8x8);
		display.draw_symbol(3, 0, CODEFONT_8X8_TEMPERATURE_GAUGE);
		display.draw_symbol(3, 20, CODEFONT_8X8_PRESSURE_GAUGE);
		display.draw_symbol(3, 42, CODEFONT_8X8_HUMIDITY_GAUGE);

		display.set_fonts(font6x6);
		display.draw_rectangle(0, 0, 128, 64, Color::white());

		char tempText[10]; sprintf(tempText, "%.1lf degC", s_displayData.s_temperature);
		display.set_text_cursor(30, 4);
		display.print_text(tempText);

		char presText[10]; sprintf(presText, "%.1lf hPa", s_displayData.s_pressure / 100);
		display.set_text_cursor(30, 28);
		display.print_text(presText);

		char humText[10]; sprintf(humText, "%.1lf %%", s_displayData.s_humidity);
		display.set_text_cursor(30, 50);
		display.print_text(humText);

		display.refresh();
	}

	void drawWeatherForecast(GraphicsDevice& display) {

		// Abbreviations for days of the week
		static const char* dayNames[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

		// Setting the 6x6 fontset with weather symbols
		display.set_fonts(font6x6);

		// Drawing screen outline
		display.clear();
		display.draw_rectangle(0, 0, 128, 64, Color::white());

		// Printing weather information for tomorrow
		char text_1[15]; sprintf(text_1, "%.0lf'C", round(s_displayData.s_temperature_1));
		display.draw_symbol(5, 5, weather_icon(s_displayData.s_weatherID_1));
		display.set_text_cursor(5, 54);
		display.print_text(text_1);
		sprintf(text_1, "%s", dayNames[s_displayData.s_dayIndex_1]);
		display.set_text_cursor(5, 45);
		display.print_text(text_1);

		// Printing weather information for 2 days from now
		char text_2[15]; sprintf(text_2, "%.0lf'C", round(s_displayData.s_temperature_2));
		display.draw_symbol(47, 5, weather_icon(s_displayData.s_weatherID_2));
		display.set_text_cursor(47, 54);
		display.print_text(text_2);
		sprintf(text_2, "%s", dayNames[s_displayData.s_dayIndex_2]);
		display.set_text_cursor(47, 45);
		display.print_text(text_2);

		// Printing weather information for 3 days from now
		char text_3[15]; sprintf(text_3, "%.0lf'C", round(s_displayData.s_temperature_3));
		display.draw_symbol(90, 5, weather_icon(s_displayData.s_weatherID_3));
		display.set_text_cursor(90, 54);
		display.print_text(text_3);
		sprintf(text_3, "%s", dayNames[s_displayData.s_dayIndex_3]);
		display.set_text_cursor(90, 45);
		display.print_text(text_3);

		display.refresh();
	}

	void drawStatus(GraphicsDevice& display)
	{
		display.clear();
		display.set_text_cursor(0, 0);

		// Getting time information
		time_t rawTime = time(NULL);
		struct tm* time = localtime(&rawTime);
		char timeStr[64];
		strftime(timeStr, 64, "%Y.%m.%d %H:%M", time);

		// Printing time information
		display.print_text("Last updated:\n");
		display.print_text(timeStr);
		display.print_text("\n\n");

		// Printing battery information
		display.print_text("Battery voltage:\n");
		display.print_text("3.3V");
		display.print_text("\n\n");

		display.refresh();
	}

	SSD1306* m_display; /**< Pointer to the specific display driver. */

	static RTC_DATA_ATTR DisplayData s_displayData;
};

// Definitions for static non-volatile storage display data
DF_Display::DisplayData DF_Display::s_displayData;

#endif // DATAFLOW_COMPONENTS_DF_DISPLAY_H_INCLUDED
