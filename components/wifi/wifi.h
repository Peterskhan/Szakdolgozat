#pragma once
#ifndef WIFI_WIFI_H_INCLUDED
#define WIFI_WIFI_H_INCLUDED

// Standard includes
#include <cstring>
#include <string>
#include <vector>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

// ESP-IDF includes
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"


// Bitmask definition for event handling in the WiFi driver
#define WIFI_BITMASK_CONNECTED (BIT0) // Indicates that the driver connected and received IP address
#define WIFI_BITMASK_FAILED    (BIT1) // Indicates that the driver failed to connect


/**
 * This static class is used to access the WiFi functionality of the microcontroller.
 * The interface enables the user to start the WiFi in station mode, scan for available
 * access points and connect to them.
 */
class WiFi {
public:

	/**
	 * Defines the authentication types for WiFi connections.
	 */
	enum class AuthenticationMode {
		Open            = WIFI_AUTH_OPEN,
		WEP             = WIFI_AUTH_WEP,
		WPA_PSK         = WIFI_AUTH_WPA_PSK,
		WPA2_PSK        = WIFI_AUTH_WPA2_PSK,
		WPA_WPA2_PSK    = WIFI_AUTH_WPA_WPA2_PSK,
		WPA2_ENTERPRISE = WIFI_AUTH_WPA2_ENTERPRISE,
		WPA3_PSK        = WIFI_AUTH_WPA3_PSK,
		WPA2_WPA3_PSK   = WIFI_AUTH_WPA2_WPA3_PSK
	};

	/**
	 * This class describes a WiFi access point.
	 */
	class AccessPoint {
	public:

		/**
		 * Constructs a network access point descriptor.
		 * @param ssid     [in] The SSID of the access point.
		 * @param authMode [in] The auth. mode of the access point.
		 * @param rssi     [in] The RSSI of the access point.
		 */
		AccessPoint(const std::string& ssid, AuthenticationMode authMode, int8_t rssi);

		/**
		 * Queries the name (SSID) of the network access point.
		 * @return The name (SSID) of the network access point.
		 */
		std::string getSSID() const;

		/**
		 * Queries the authentication mode of the network access point.
		 * @return The authentication mode of the network access point.
		 */
		AuthenticationMode getAuthenticationMode() const;

		/**
		 * Queries the received signal strength (RSSI) of the network access point.
		 * @return The received signal strength (RSSI) of the network access point.
		 */
		int8_t getRSSI() const;

	private:
		AuthenticationMode m_authMode; /**< The authentication mode of the AP. */
		std::string        m_ssid;     /**< The SSID of the AP.                */
		int8_t             m_rssi;     /**< The RSSI of the AP.                */
	};

	/**
	 * Initializes the WiFi driver interface in station mode.
	 * @return True on success, false on any failure.
	 */
	static bool start();

	/**
	 * Scans the available network access points.
	 * @param  count [in] The number of access points to return.
	 * @return The list of available network access points.
	 */
	static std::vector<AccessPoint> scan(uint16_t count = 10);

	/**
	 * Attempts to connect to the specified WiFi network.
	 * @param  ssid     [in] The SSID of the network to connect to.
	 * @param  password [in] The password of the network to connect to.
	 * @param  tryCount [in] The number of attempts to try connecting to the network.
	 * @return True on success, false on failure.
	 */
	static bool connect(std::string ssid, std::string password, uint8_t tryCount);

	/**
	 * Disconnects from the current WiFi network.
	 * @return True on success, false on failure.
	 */
	static bool disconnect();

	/**
	 * Stops the WiFi driver and releases all allocated resources.
	 * @return True on success, false on failure.
	 */
	static bool stop();

private:

	static EventGroupHandle_t s_wifi_event_group; /**< Event group to handle WiFi events.                       */
	static bool               s_initialized;      /**< Flag to indicate whether the driver is initialized.      */
	static bool               s_connecting;       /**< Flag to indicate whether we are trying to connect to AP. */
	static bool               s_connected;        /**< Flag to indicate whether we are connected to an AP.      */
	static bool               s_disconnecting;    /**< Flag to indicate purposeful disconnections.              */
	static uint8_t            s_tryCount;         /**< The number of tries remaining for connection to an AP.   */

	/**
	 * Callback function to invoke when WiFi related events occur within the ESP-IDF implementation.
	 * @param arg        [in] Arguments to the event handler.
	 * @param event_base [in] The base type of the event.
	 * @param event_id   [in] The identifier of the specific event.
	 * @param event_data [in] The data content associated with the event.
	 */
	static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
};

#endif // WIFI_WIFI_H_INCLUDED
