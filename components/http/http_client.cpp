#include "http_client.h"

HttpClient::HttpClient()
	: m_handle(nullptr)
{

}

HttpClient::~HttpClient()
{

}

HttpResponse HttpClient::send_request(const HttpRequest& request)
{
	// Creating HTTP client configuration
	esp_http_client_config_t config = {};

	// Specifying target URL based on request
	config.url = request.get_url().c_str();

	// Specifying this object for use in the event handler
	config.user_data = (void*) this;

	// Specifying the event handler callback
	config.event_handler = event_handler;

	// Specifying the HTTP method based on request
	config.method = (esp_http_client_method_t) request.get_method();

	// Initializing ESP-IDF HTTP client handle based on configuration
	m_handle = esp_http_client_init(&config);

	// Adding request headers
	for(auto entry : request.get_headers()) {
		esp_http_client_set_header(m_handle, entry.first.c_str(), entry.second.c_str());
	}

	// Adding request body
	esp_http_client_set_post_field(m_handle, request.get_body().c_str(), request.get_body().size());

	// Clearing response object
	m_response.m_body.clear();
	m_response.m_headers.clear();

	// Sending request and reading response
	esp_http_client_perform(m_handle);

	// Setting response status code
	m_response.m_statusCode = esp_http_client_get_status_code(m_handle);

	// Cleaning up HTTP session
	esp_http_client_cleanup(m_handle);

	// Returning the HTTP server response
	return m_response;
}

esp_err_t HttpClient::event_handler(esp_http_client_event_t *evt)
{
	// Getting pointer to client from user data pointer
	HttpClient* client = static_cast<HttpClient*>(evt->user_data);

	// Handling HTTP event based on event type
	switch(evt->event_id) {

		// Handling HTTP error event
		case HTTP_EVENT_ERROR: {
			ESP_LOGE("HTTP", "HTTP client [%p] error occurred.", client);
			break;
		}

		// Handling HTTP connected event
		case HTTP_EVENT_ON_CONNECTED: {
			ESP_LOGI("HTTP", "HTTP client [%p] connected to server.", client);
			break;
		}

		// Handling HTTP header sent event
		case HTTP_EVENT_HEADER_SENT: {
			ESP_LOGI("HTTP", "HTTP client [%p] finished sending a header.", client);
			break;
		}

		// Handling HTTP header received event
		case HTTP_EVENT_ON_HEADER: {
			ESP_LOGI("HTTP", "HTTP client [%p] received a header.", client);
			client->m_response.m_headers[evt->header_key] = evt->header_value;
			break;
		}

		// Handling HTTP data received event
		case HTTP_EVENT_ON_DATA: {
			ESP_LOGI("HTTP", "HTTP client [%p] received body data.", client);
			client->m_response.m_body += std::string((char*) evt->data, (char*) evt->data + evt->data_len);
			break;
		}

		// Handling HTTP transfer finished event
		case HTTP_EVENT_ON_FINISH: {
			ESP_LOGI("HTTP", "HTTP client [%p] finished the transfer.", client);
			break;
		}

		// Handling HTTP disconnected event
		case HTTP_EVENT_DISCONNECTED: {
			ESP_LOGI("HTTP", "HTTP client [%p] disconnected from the server.", client);
			break;
		}
	}

	// Success
	return ESP_OK;
}

