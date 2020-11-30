#include "http_response.h"


HttpResponse::HttpResponse(uint16_t statusCode)
	: m_statusCode(statusCode), m_body("")
{}

const uint16_t& HttpResponse::get_status_code() const
{
	return m_statusCode;
}

const HttpResponse::HeaderList& HttpResponse::get_headers() const
{
	return m_headers;
}

std::string HttpResponse::get_header(const std::string& key) const
{
	// Checking if the header entry is available
	if(m_headers.find(key) != m_headers.end()) return m_headers.at(key);

	// Entry not found, return empty string
	return "";
}

const std::string& HttpResponse::get_body() const
{
	return m_body;
}
