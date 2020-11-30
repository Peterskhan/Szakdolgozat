#include "dataflow_components/df_display.h"

#include "app_components.h"

// DEBUG
#include <stdio.h>
#include <fstream>
#include <time.h>

// Framework includes
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_sntp.h"

// Standard includes
#include <cmath>

// Including BME280 driver
#include "bme280.h"

// Including SSD1306 driver
#include "ssd1306.h"

// Including extra graphics fonts
#include "font6x6.h"
#include "font8x8.h"

// Including WiFi driver
#include "wifi.h"

// Including Thingspeak Client driver
#include "thingspeak.h"

// Including SD card driver
#include "sd_spi.h"

// I2C parameters
#define I2C_PORT    (1)
#define I2C_SCL_PIN (21)
#define I2C_SDA_PIN (22)
#define I2C_CLK_HZ  (100000)

// SPI parameters
#define SPI_CS_PIN   (12)
#define SPI_MISO_PIN (13)
#define SPI_MOSI_PIN (14)
#define SPI_CLK_PIN  (15)

// WiFi parameters
//#define WIFI_SSID "PCBDesign Guest"
//#define WIFI_PASS "pcbd3s1gn"

#define WIFI_SSID "UPC1471152"
#define WIFI_PASS "h8bAzcmsmzuj"

//#define WIFI_SSID "Gyulaip"
//#define WIFI_PASS "pjnhcoydeaf8sveu"

//#define WIFI_SSID "AndroidAP"
//#define WIFI_PASS "xacl7991"

// ThingSpeak parameters
#define THINGSPEAK_CHANNEL_ID (1168081)
#define THINGSPEAK_READ_KEY   ("EMA3X4Z014C05FSB")
#define THINGSPEAK_WRITE_KEY  ("5PVLUGAUWS0CCKKC")

// Deep-sleep duration parameter
#define DEEP_SLEEP_DURATION_SEC (600)
//#define DEEP_SLEEP_DURATION_SEC (16)


Font font8x8 = {font8x8_descriptors, font8x8_data, 8};
Font font6x6 = {font6x6_descriptors, font6x6_data, 6};

std::string roundString(double value, std::size_t decimals = 1) {
	char output[10]; sprintf(output, "%.*lf", decimals, value);
	return output;
}

void draw_data(GraphicsDevice& display, double temp, double press, double hum) {

	display.set_fonts(font8x8);
	display.draw_symbol(3, 0, CODEFONT_8X8_TEMPERATURE_GAUGE);
	display.draw_symbol(3, 20, CODEFONT_8X8_PRESSURE_GAUGE);
	display.draw_symbol(3, 42, CODEFONT_8X8_HUMIDITY_GAUGE);

	display.set_fonts(font6x6);
	display.draw_rectangle(0, 0, 128, 60, Color::white());

	char tempText[10]; sprintf(tempText, "%.1lf degC", temp);
	display.set_text_cursor(30, 4);
	display.print_text(tempText);

	char presText[10]; sprintf(presText, "%.1lf hPa", press / 100);
	display.set_text_cursor(30, 28);
	display.print_text(presText);

	char humText[10]; sprintf(humText, "%.1lf %%", hum);
	display.set_text_cursor(30, 50);
	display.print_text(humText);
}

uint16_t weather_icon(uint16_t weatherID) {
	if(weatherID >= 200 && weatherID < 300) return FONT6X6_THUNDERSTORM;
	if(weatherID >= 300 && weatherID < 400) return FONT6X6_RAIN;
	if(weatherID >= 500 && weatherID < 600) return FONT6X6_RAIN;
	if(weatherID >= 600 && weatherID < 700) return FONT6X6_SNOW;
	if(weatherID >= 700 && weatherID < 800) return FONT6X6_FOGGY;
	if(weatherID == 800)                    return FONT6X6_SUNNY;
	if(weatherID == 801)                    return FONT6X6_CLOUDY_SUN;
	if(weatherID >= 802 && weatherID < 900) return FONT6X6_CLOUDS;

	return '?';
}

void draw_forecast(GraphicsDevice& display, const std::string& forecast) {

	// Abbreviations for days of the week
	static const char* dayNames[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

	// Setting the 6x6 fontset with weather symbols
	display.set_fonts(font6x6);

	// Parsing the forecast data JSON string
	JsonObject data = JsonObject::parse(forecast);

	// Data for today
	int nowID = data["now"]["id"].getInteger();
	int nowClouds = data["now"]["clouds"].getInteger();
	ESP_LOGI("NOW", "ID: %d, Clouds: %d%%", nowID, nowClouds);

	// Data for tomorrow
	int id_1 = data["forecast"][0]["id"].getInteger();
	int dayName_1 = data["forecast"][0]["day"].getInteger();
	double day_1 = data["forecast"][0]["temperature"]["day"].getDouble();
	ESP_LOGI("1st", "ID: %d, day: %lf Celsius", id_1, day_1);

	// Data for 2 days from now
	int id_2 = data["forecast"][1]["id"].getInteger();
	int dayName_2 = data["forecast"][1]["day"].getInteger();
	double day_2 = data["forecast"][1]["temperature"]["day"].getDouble();
	ESP_LOGI("2nd", "ID: %d, day: %lf Celsius", id_2, day_2);

	// Data for 3 days from now
	int id_3 = data["forecast"][2]["id"].getInteger();
	int dayName_3 = data["forecast"][2]["day"].getInteger();
	double day_3 = data["forecast"][2]["temperature"]["day"].getDouble();
	ESP_LOGI("3rd", "ID: %d, day: %lf Celsius", id_3, day_3);

	// Drawing screen outline
	display.clear();
	display.draw_rectangle(0, 0, 128, 64, Color::white());

	// Printing weather information for tomorrow
	char text_1[15]; sprintf(text_1, "%.0lf'C", round(day_1));
	display.draw_symbol(5, 5, weather_icon(id_1));
	display.set_text_cursor(5, 54);
	display.print_text(text_1);
	sprintf(text_1, "%s", dayNames[dayName_1]);
	display.set_text_cursor(5, 45);
	display.print_text(text_1);

	// Printing weather information for 2 days from now
	char text_2[15]; sprintf(text_2, "%.0lf'C", round(day_2));
	display.draw_symbol(47, 5, weather_icon(id_2));
	display.set_text_cursor(47, 54);
	display.print_text(text_2);
	sprintf(text_2, "%s", dayNames[dayName_2]);
	display.set_text_cursor(47, 45);
	display.print_text(text_2);

	// Printing weather information for 3 days from now
	char text_3[15]; sprintf(text_3, "%.0lf'C", round(day_3));
	display.draw_symbol(90, 5, weather_icon(id_3));
	display.set_text_cursor(90, 54);
	display.print_text(text_3);
	sprintf(text_3, "%s", dayNames[dayName_3]);
	display.set_text_cursor(90, 45);
	display.print_text(text_3);

	// Refreshing the screen display
	display.refresh();
}

void weatherTaskFunction(void *pvParameters) {

	// Status flag to indicate successful connection to WiFI
	bool wifiConnected = false;

	// Status flag to indicate successful mounting of SD card
	bool cardMounted = false;

	// Starting WiFi and connecting to access point
	WiFi::start();
	wifiConnected = WiFi::connect(WIFI_SSID, WIFI_PASS, 10);

	// Creating ThingSpeak client to push measurements
	ThingSpeakClient client(THINGSPEAK_READ_KEY, THINGSPEAK_WRITE_KEY);

	// Creating I2C Master interface
	I2C_Master master(I2C_PORT, I2C_SCL_PIN, I2C_SDA_PIN, I2C_CLK_HZ);

	// Creating and initializing SD card interface (mounting point is: /sd)
	SD_SPI card(SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CLK_PIN, SPI_CS_PIN);
	cardMounted = card.mount("/sd");

	// Creating BME280 sensor driver object
	BME280 sensor(&master);

	// Creating SSD1306 display driver object
	SSD1306 display(&master);

	// Requesting measurement & waiting for completion
	sensor.set_mode(BME280::Mode::Forced);
	vTaskDelay((sensor.get_measurement_delay_ms() + 100) / portTICK_RATE_MS);

	// Reading environmental measurements
	double temperature = sensor.get_temperature();
	double pressure    = sensor.get_pressure();
	double humidity    = sensor.get_humidity();

	// Logging measurement data for debug
	ESP_LOGI("TEMP", "%lf Celsius", temperature);
	ESP_LOGI("PRES", "%lf Pa",      pressure);
	ESP_LOGI("HUM ", "%lf %%",      humidity);

	if(cardMounted) {

		// Opening log file on SD card
		FILE* sd_card = fopen("/sd/data.csv", "a");

		// Logging measurement data to SD card
		fprintf(sd_card, "%.1f; %.0f; %.1f\n", temperature, pressure, humidity);

		// Closing measurement log file and unmounting the SD card
		fclose(sd_card);
		card.unmount();
	}

	// Drawing to the display
	draw_data(display, temperature, pressure, humidity);

	// Refreshing display
	display.refresh();

	if(wifiConnected) {

		// Pushing measurement data to ThingSpeak
		ThingSpeakUpdate update;
		update.setField(1, roundString(temperature));
		update.setField(2, roundString(pressure));
		update.setField(3, roundString(humidity));
		client.writeChannel(update);

		// Read forecast data from ThingSpeak
		draw_forecast(display, client.readField(THINGSPEAK_CHANNEL_ID, 4));
	}

	// Wake-up in 16 seconds
	esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION_SEC * 1000 * 1000);

	// Starting deep sleep
	esp_deep_sleep_start();
}

void dataflow_test(void *pvParameters) {

	// Debug component for printing debug messages
	DF_Debug debug;

	// I2C master for usage with the BME280 sensor and the SSD1306 display
	DF_I2C_Master master(I2C_PORT, I2C_SCL_PIN, I2C_SDA_PIN, I2C_CLK_HZ);

	// Display for graphical information
	DF_Display display;

	// BME280 sensor dataflow component for measuring weather parameters
	DF_BME280 sensor;

	// WiFi connection component
	DF_WifiConnect wifi(WIFI_SSID, WIFI_PASS);

	// Thingspeak Read Field client for reading weather forecast data
	DF_ThingspeakRead forecastReader(THINGSPEAK_CHANNEL_ID, 4, THINGSPEAK_READ_KEY);

	// This component prepares the Thingspeak update from the sensor data
	DF_Function thingspeakPostPrepare([](Component::PortContainer& ports){

		// Node object to read messages
		Node message;

		while(true) {

			// Reading message from the input port
			ports["in"].receive(message);

			// Extracting temperature, pressure and humidity data from the message
			double temperature = (double) message["temperature"];
			double pressure    = (double) message["pressure"];
			double humidity    = (double) message["humidity"];

			// Creating output message
			message.clear();
			message["update"][0] = temperature;
			message["update"][1] = pressure;
			message["update"][2] = humidity;

			// Sending output message
			ports["out"].send(message);
		}
	});

	// Thingspeak update component for writing sensor readings to Thingspeak
	DF_ThingspeakWrite readingPoster(THINGSPEAK_WRITE_KEY);

	// Test GPIO for switching display menus
	DF_GPIO gpio(GPIO_NUM_0, DF_GPIO::Direction::INPUT, DF_GPIO::PullMode::PULLUP, DF_GPIO::TriggerType::POSEDGE);

	// Simple software debouncing for GPIO input
	DF_Debounce debouncer(50);

	// Simple watchdog timer which fires after 10 seconds of inactivity
	DF_Watchdog inactivityTimer(10 * 1000);

	DF_Function deepSleepStart([](Component::PortContainer& ports) {

			// Node object to receive messages
			Node message;

			// Reading input messages
			ports["in"].receive(message);

			// Debug printing
			std::cout << "Starting deep sleep (10 minutes)" << std::endl;

			// Wake-up 10 minutes
			esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION_SEC * 1000 * 1000);
			esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);

			// Starting deep sleep
			esp_deep_sleep_start();
	});

	DF_Function logger([](Component::PortContainer& ports) {

		// Creating SD-SPI interface
		SD_SPI sdCard(SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CLK_PIN, SPI_CS_PIN);

		// Mounting the SD card as a partition
		if(!sdCard.mount("/sd")) vTaskSuspend(xTaskGetCurrentTaskHandle());

		// Node object for reading messages
		Node message;

		while(true) {

			// Reading measurement data from input
			ports["in"].receive(message);

			// Opening measurement file in append mode
			FILE* fp = fopen("/sd/data.csv", "a");

			// Reading measurement data from the message
			double temperature = (double) message["temperature"];
			double pressure    = (double) message["pressure"];
			double humidity    = (double) message["humidity"];

			// Getting time information
			time_t rawTime = time(NULL);
			struct tm* time = localtime(&rawTime);
			char timeStr[64];
			strftime(timeStr, 64, "%Y.%m.%d %H:%M:%S", time);

			// Writing measurement data to file
			if(fp) fprintf(fp, "%s; %.1lf; %.0lf; %.1lf;\n", timeStr, temperature, pressure, humidity);

			// Closing the file and flushing output
			fclose(fp);
		}
	});

	DF_Function timesync([](Component::PortContainer& ports){

		// Node object to read input messages
		Node message;

		// Reading input message to trigger time synchronisation
		ports["in"].receive(message);

		// Initializing SNTP for time synchronisation
		sntp_setoperatingmode(SNTP_OPMODE_POLL);
		sntp_setservername(0, "pool.ntp.org");
		sntp_init();
		setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
		tzset();

		vTaskSuspend(xTaskGetCurrentTaskHandle());
	});

	// Connecting component outputs which should reset the inactivity timer
	wifi["out"] >> inactivityTimer["in"];
	gpio["out"] >> inactivityTimer["in"];
	inactivityTimer["out"] >> deepSleepStart["in"];

	// Connecting GPIO input to the display
	gpio["out"] >> debouncer["in"]["out"] >> display["in"];

	// Specifying dataflow connections
	master["interface"] >> sensor["interface"];
	master["interface"] >> display["interface"];

	// Logging sensor data to SD card
	sensor["out"] >> logger["in"];

	// When we connected to the WiFi read the forecast data from Thingspeak
	wifi["out"] >> forecastReader["in"]["out"] >> display["in"];

	// When we connected to the WiFi, take sensor readings and send a Thingspeak update
	wifi["out"] >> sensor["in"]["out"] >> thingspeakPostPrepare["in"]["out"] >> debug["in"]["out"]
				>> readingPoster["in"];

	wifi["out"] >> timesync["in"];

	// Display the sensor readings on the display as well
	sensor["out"] >> display["in"];

	// Kickstarting the flow by sending an initial message to the sensor
	wifi["in"].sendInitialMessage(nullptr);

	// Creating dataflow manager object
	Dataflow flow;

	// Adding dataflow components to the manager
	flow.addComponent(&master);
	flow.addComponent(&sensor);
	flow.addComponent(&display);
	flow.addComponent(&debug);
	flow.addComponent(&forecastReader);
	flow.addComponent(&thingspeakPostPrepare);
	flow.addComponent(&readingPoster);
	flow.addComponent(&wifi);
	flow.addComponent(&gpio);
	flow.addComponent(&debouncer);
	flow.addComponent(&inactivityTimer);
	flow.addComponent(&deepSleepStart);
	flow.addComponent(&logger);
	flow.addComponent(&timesync);

	// Starting the dataflow execution
	flow.startFlow();

	// Suspending the current task to let the dataflow execute
	vTaskSuspend(xTaskGetCurrentTaskHandle());
}

extern "C" void app_main(void)
{
	xTaskCreatePinnedToCore(dataflow_test, "dataflow_task", 8192, nullptr, 2, nullptr, 0);

	// Creating application task
	// xTaskCreatePinnedToCore(weatherTaskFunction, "weather_task", 8192, nullptr, 2, nullptr, 0);

	// Yielding indefinitely from the main task
	while(true) vTaskDelay(1000);
}



