#include "http_request.h"


/**
 * Construct a HttpRequest object.
 * @param method [in] The HTTP method to use when sending the request.
 * @param url    [in] The target URL used to send the request.
 */
HttpRequest::HttpRequest(Method method, const std::string& url)
	: m_method(method), m_url(url), m_body("")
{}

/**
 * Queries the HTTP method currently set for the request.
 * @return HttpRequest::Method
 */
const HttpRequest::Method& HttpRequest::get_method() const
{
	return m_method;
}

/**
 * Sets the HTTP method used for sending the request.
 * @param method [in] The new HTTP method for sending the request.
 */
void HttpRequest::set_method(Method method)
{
	m_method = method;
}

/**
 * Queries the current target URL of the request.
 * @return std::string
 */
const std::string& HttpRequest::get_url() const
{
	return m_url;
}

/**
 * Sets the target URL for the request.
 * @param url [in] The new target URL for the request.
 */
void HttpRequest::set_url(const std::string& url)
{
	m_url = url;
}

/**
 * Queries the header entries of the request.
 * @return A map containing the header entries as key-value pairs.
 */
const HttpRequest::HeaderList& HttpRequest::get_headers() const
{
	return m_headers;
}

/**
 * Queries the value of the specified HTTP header entry.
 * @param  key [in] The name of the HTTP header entry.
 * @return The value of the HTTP header entry or and empty string.
 */
std::string HttpRequest::get_header(const std::string& key) const
{
	// Checking if the header entry is available
	if(m_headers.find(key) != m_headers.end()) return m_headers.at(key);

	// Entry not found, return empty string
	return "";
}

/**
 * Sets the value of the specified HTTP header entry (creates new entry, if does not exist).
 * @param key   [in] The name of the HTTP header entry.
 * @param value [in] The new value of the HTTP header entry.
 */
void HttpRequest::set_header(const std::string& key, const std::string& value)
{
	m_headers[key] = value;
}

/**
 * Deletes the HTTP header entry with the specified name.
 * @param key [in] The name of the HTTP header entry.
 */
void HttpRequest::delete_header(const std::string& key)
{
	m_headers.erase(key);
}

/**
 * Queries the current body of the HTTP request.
 * @return std::string
 */
const std::string& HttpRequest::get_body() const
{
	return m_body;
}

/**
 * Sets the body of the HTTP request to the specified string.
 * @param data [in] The new content of the HTTP body.
 */
void HttpRequest::set_body(const std::string& data)
{
	m_body = data;
}

/**
 * Sets the body of the HTTP request to the specified data.
 * @param data   [in] An array of bytes to copy into the body.
 * @param length [in] The number of bytes in the data array.
 */
void HttpRequest::set_body(const uint8_t data[], std::size_t length)
{
	m_body = std::string((const char*) data, (const char*) data + length);
}

