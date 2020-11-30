#include "wifi.h"


/**
 * Constructs a network access point descriptor.
 * @param ssid     [in] The SSID of the access point.
 * @param authMode [in] The auth. mode of the access point.
 * @param rssi     [in] The RSSI of the access point.
 */
WiFi::AccessPoint::AccessPoint(const std::string& ssid, AuthenticationMode authMode, int8_t rssi)
	: m_authMode(authMode), m_ssid(ssid), m_rssi(rssi)
{}

/**
 * Queries the name (SSID) of the network access point.
 * @return The name (SSID) of the network access point.
 */
std::string WiFi::AccessPoint::getSSID() const
{
	return m_ssid;
}

/**
 * Queries the authentication mode of the network access point.
 * @return The authentication mode of the network access point.
 */
WiFi::AuthenticationMode WiFi::AccessPoint::getAuthenticationMode() const
{
	return m_authMode;
}

/**
 * Queries the received signal strength (RSSI) of the network access point.
 * @return The received signal strength (RSSI) of the network access point.
 */
int8_t WiFi::AccessPoint::getRSSI() const
{
	return m_rssi;
}

bool WiFi::start()
{
	// Checking if WiFi driver has been already initialized
	if(!s_initialized) {

		// Initializing NVS Flash for Access Point data storage
		if(nvs_flash_init() != ESP_OK) return false;

		// Creating event group to handle WiFi connection events
		s_wifi_event_group = xEventGroupCreate();

		// Initializing netif from ESP-IDF
		if(esp_netif_init() != ESP_OK) return false;

		// Creating WiFi event loop
		if(esp_event_loop_create_default() != ESP_OK) return false;

		// Creating WiFi Station mode controller
		esp_netif_create_default_wifi_sta();

		// Initializing WiFi
		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		if(esp_wifi_init(&cfg) != ESP_OK) return false;

		// Registering event handlers for WiFi and IP events
		esp_event_handler_instance_t instance_any_id;
		esp_event_handler_instance_t instance_got_ip;
		if(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id) != ESP_OK) return false;
		if(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip) != ESP_OK) return false;

		// Setting initialization indicator flag
		s_initialized = true;
	}

	// Success
	return true;
}

std::vector<WiFi::AccessPoint> WiFi::scan(uint16_t count)
{
	// Allocating memory for the AP descriptor array
	wifi_ap_record_t* accessPoints = new wifi_ap_record_t[count];

	// Clearing AP descriptor array
	memset(accessPoints, 0x00, count);

	// Creating access point vector to return
	std::vector<AccessPoint> results;

	// Starting WiFi in station mode and scanning
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&count, accessPoints));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&count));

    // Creating access point descriptors based on scanning results
    for(uint16_t i = 0; i < count; i++) {
    	results.push_back(AccessPoint(
    			(const char*) accessPoints[i].ssid,
    			(AuthenticationMode) accessPoints[i].authmode,
				accessPoints[i].rssi));
    }

    // Returning the results of the scan
    return results;
}

bool WiFi::connect(std::string ssid, std::string password, uint8_t tryCount)
{
	// Checking if WiFi is initialized
	if(!s_initialized) return false;

	// Checking if already connected
	if(s_connected) disconnect();

	// Setting connecting flag
	s_connecting = true;

	// Creating new WiFi configuration based on SSID and password
	wifi_config_t wifi_config = {};
	strcpy((char*) wifi_config.sta.ssid, ssid.c_str());
	strcpy((char*) wifi_config.sta.password, password.c_str());
	wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
	wifi_config.sta.pmf_cfg.capable = true;
	wifi_config.sta.pmf_cfg.required = false;

	// Applying configuration changes
	if(esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) return false;
	if(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) != ESP_OK) return false;
	if(esp_wifi_start() != ESP_OK) return false;

	// Resetting connection try count
	s_tryCount = tryCount;

	// Logging
	ESP_LOGI("WIFI", "WiFi station configuration finished, attempting to connect to AP.");

	// Connecting to WiFi
	esp_wifi_connect();

	// Waiting for WiFi event group to signal success or failure of connection
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
			WIFI_BITMASK_CONNECTED | WIFI_BITMASK_FAILED,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);

	// Checking if the occurred event is the WiFi connected event
	if (bits & WIFI_BITMASK_CONNECTED) {

		// Logging
		ESP_LOGI("WIFI", "Successfully connected to AP. SSID: %s", ssid.c_str());

		// Clearing event group bits
		xEventGroupClearBits(s_wifi_event_group, WIFI_BITMASK_CONNECTED | WIFI_BITMASK_FAILED);

		// Success, we have nothing else to do...
		return true;
	}

	// Checking if the occured event is the WiFi connection failed event
	else if (bits & WIFI_BITMASK_FAILED) {

		// Logging
		ESP_LOGE("WIFI", "Connection failed to AP, connection aborted.");

		// Clearing event group bits
		xEventGroupClearBits(s_wifi_event_group, WIFI_BITMASK_CONNECTED | WIFI_BITMASK_FAILED);

		// Failure, we have nothing else to do...
		return false;
	}

	// We should not get here...
	return false;
}

bool WiFi::disconnect()
{
	// Checking if we are properly initialized and connected to a network
	if(s_initialized && s_connected) {

		// Setting disconnection flag
		s_disconnecting = true;

		// Calling ESP-IDF WiFi disconnect implementation
		return (esp_wifi_disconnect() == ESP_OK);
	}

	// Not initialized or connected, disconnection failed
	return false;
}

bool WiFi::stop()
{
	// Checking if we are connected to a network
	if(s_connected || s_connecting) {

		// Clearing connection try counter
		s_tryCount = 0;

		// Disconnecting from the network
		disconnect();
	}

	// Stopping the WiFi driver
	if(esp_wifi_stop() != ESP_OK) return false;

	// Releasing allocated resources of the WiFi driver
	if(esp_wifi_deinit() != ESP_OK) return false;

	// Clearing status flags
	s_initialized   = false;
	s_connecting    = false;
	s_connected     = false;
	s_disconnecting = false;

	// Successfully stopped the WiFi
	return true;
}

// Definitions private static members
EventGroupHandle_t WiFi::s_wifi_event_group;
bool WiFi::s_initialized = false;
bool WiFi::s_connecting  = false;
bool WiFi::s_connected   = false;
bool WiFi::s_disconnecting = false;
uint8_t WiFi::s_tryCount = 0;

void WiFi::event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	// Checking if WiFi station mode started successfully
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {

    	// Logging
    	ESP_LOGI("WIFI", "WiFi started in station mode successfully.");
    }

    // Checking if WiFi connection has been dropped
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {

    	// Casting the received event data to the appropriate type
    	wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;

    	// Clearing connected flag
    	s_connected = false;

    	// Checking if the disconnection occurred on purpose
    	if(s_disconnecting) {

    		// Logging
    		ESP_LOGI("WIFI", "WiFi disconnected from AP successfully.");

    		// Clearing disconnection flag
    		s_disconnecting = false;

    		// Success, nothing else to do...
    		return;
    	}

    	// Checking if we are currently trying to connect to an AP
    	if(s_connecting && s_tryCount != 0) {

    		// Decreasing the number of remaining connection attempts
    		s_tryCount--;

    		// Checking if the disconnection occurred because the AP does not exist at all
    		if(event->reason == WIFI_REASON_NO_AP_FOUND) {

    			// Logging
    			ESP_LOGE("WIFI", "Failed to connect to AP, no AP found. Aborting connection...");

				// Clearing connection indication flag
				s_connecting = false;

				// Clearing remaining try counter
				s_tryCount = 0;

    			// Indicating that we have will not be able to connect
				xEventGroupSetBits(s_wifi_event_group, WIFI_BITMASK_FAILED);

				// We could not connect, nothing else to do...
				return;
    		}

    		// Logging
    		ESP_LOGI("WIFI", "Could not connect to AP, remaining attempts: %d", s_tryCount);

    		// Trying to establish the WiFi connection again
    		esp_wifi_connect();

    		// Success, nothing else to do...
    		return;
    	}

    	// Checking if we are currently trying to connect, but we have no further attempts remaining
    	else if(s_connecting && s_tryCount == 0) {

    		// Clearing connection indication flag
    		s_connecting = false;

    		// Logging
    		ESP_LOGE("WIFI", "Could not connect to AP, maximum attempts reached.");

    		// Indicating that we have run out of connection attempts
    		xEventGroupSetBits(s_wifi_event_group, WIFI_BITMASK_FAILED);

    		// We could not connect, nothing else to do...
    		return;
    	}

    	// Otherwise the event occurred during an active connection, trying to reconnect
    	else {

    		// Logging
    		ESP_LOGW("WIFI", "WiFi connection dropped unexpectedly, trying to reconnect... Reason code: %d", event->reason);

    		// Trying to reestablish connection
    		esp_wifi_connect();

    		// We are trying to reconnect, nothing else to do...
    		return;
    	}
    }

    // Checking if we have received IP address successfully
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {

    	// Casting the event data to the appropriate type
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;

        // Logging
        ESP_LOGI("WIFI", "IP-address received:" IPSTR, IP2STR(&event->ip_info.ip));

        // Clearing connecting flag
        s_connecting = false;

        // Setting connected flag
        s_connected = true;

        // Indicating successful connection in the WiFi event group
        xEventGroupSetBits(s_wifi_event_group, WIFI_BITMASK_CONNECTED);
    }
}
