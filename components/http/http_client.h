#pragma once
#ifndef HTTP_CLIENT_HTTP_CLIENT_H
#define HTTP_CLIENT_HTTP_CLIENT_H

// Standard includes
#include <cstdio>
#include <cstring>
#include <string>

// ESP-IDF includes
#include "esp_log.h"
#include "esp_http_client.h"

// Project includes
#include "http_request.h"
#include "http_response.h"


class HttpClient {
public:

	HttpClient();

	~HttpClient();

	/**
	 * Sends the specified HTTP request and reads the server response.
	 * @param  request [in] The HTTP request to send.
	 * @return The response of the server.
	 */
	HttpResponse send_request(const HttpRequest& request);

private:

	/** Handle to the ESP-IDF HTTP client structure. */
	esp_http_client_handle_t m_handle;

	/** HTTP response object for the current request. */
	HttpResponse m_response;

	/**
	 * Event handler to pass to ESP-IDF implementation
	 * @param  evt [in] The HTTP event to process.
	 * @return Status code.
	 */
	static esp_err_t event_handler(esp_http_client_event_t *evt);

};


#endif // HTTP_CLIENT_HTTP_CLIENT_H
