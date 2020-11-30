#pragma once
#ifndef HTTP_HTTP_RESPONSE_H_INCLUDED
#define HTTP_HTTP_RESPONSE_H_INCLUDED

// Standard includes
#include <cstdint>
#include <string>
#include <map>


/**
 * This class represents the response of a HTTP server upon receiving a request.
 * The interface grants read-only access to the status code, headers and body
 * content of the response. HTTP response objects are created by the HttpClient
 * class upon finishing a HTTP transaction.
 */
class HttpResponse {
public:

	/**	The underlying type for storing HTTP header entries. */
	using HeaderList = std::map<std::string, std::string>;

	/**
	 * Constructs a HttpResponse object with the specified status.
	 * @param statusCode [in] The numerical representation of the response status.
	 */
	HttpResponse(uint16_t statusCode = 0);

	/**
	 * Queries the HTTP response status code.
	 * @return The numeric representation of the response status.
	 */
	const uint16_t& get_status_code() const;

	/**
	 * Queries the header entries of the response.
	 * @return A map containing the header entries as key-value pairs.
	 */
	const HeaderList& get_headers() const;

	/**
	 * Queries the value of the specified HTTP header entry.
	 * @param  key [in] The name of the HTTP header entry.
	 * @return The value of the HTTP header entry or and empty string.
	 */
	std::string get_header(const std::string& key) const;

	/**
	 * Queries the body of the HTTP response.
	 * @return std::string
	 */
	const std::string& get_body() const;

private:

	/** Making the HttpClient class friend for constructing HTTP responses. */
	friend class HttpClient;

	uint16_t    m_statusCode;   /**< The HTTP response code.          */
	std::string m_body;         /**< The body of the request.         */
	HeaderList  m_headers;      /**< The HTTP headers of the request. */
};


#endif // HTTP_HTTP_RESPONSE_H_INCLUDED
