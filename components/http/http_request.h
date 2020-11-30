#pragma once
#ifndef HTTP_CLIENT_HTTP_REQUEST_H_INCLUDED
#define HTTP_CLIENT_HTTP_REQUEST_H_INCLUDED

// Standard includes
#include <string>
#include <map>

// ESP-IDF includes
#include "esp_http_client.h"


/**
 * This class represents a complete HTTP request that can be sent to a server,
 * to trigger a response. The interface specifies the target URL, the HTTP method,
 * headers and body content. The request must be kept alive until the HTTP transfer
 * finishes.
 */
class HttpRequest {
public:

	/**
	 * The Method enumeration defines the supported methods for sending a HTTP request to a server.
	 */
	enum class Method {
		GET         = HTTP_METHOD_GET,         /**< HTTP method: GET         */
		POST        = HTTP_METHOD_POST,        /**< HTTP method: POST        */
		PUT         = HTTP_METHOD_PUT,         /**< HTTP method: PUT         */
		PATCH       = HTTP_METHOD_PATCH,       /**< HTTP method: PATCH       */
		DELETE      = HTTP_METHOD_DELETE,      /**< HTTP method: DELETE      */
		HEAD        = HTTP_METHOD_HEAD,        /**< HTTP method: HEAD        */
		NOTIFY      = HTTP_METHOD_NOTIFY,      /**< HTTP method: NOTIFY      */
		SUBSCRIBE   = HTTP_METHOD_SUBSCRIBE,   /**< HTTP method: SUBSCRIBE   */
		UNSUBSCRIBE = HTTP_METHOD_UNSUBSCRIBE, /**< HTTP method: UNSUBSCRIBE */
		OPTIONS     = HTTP_METHOD_OPTIONS,     /**< HTTP method: OPTIONS     */
		COPY        = HTTP_METHOD_COPY,        /**< HTTP method: COPY        */
		MOVE        = HTTP_METHOD_MOVE,        /**< HTTP method: MOVE        */
		LOCK        = HTTP_METHOD_LOCK,        /**< HTTP method: LOCK        */
		UNLOCK      = HTTP_METHOD_UNLOCK,      /**< HTTP method: UNLOCK      */
		PROPFIND    = HTTP_METHOD_PROPFIND,    /**< HTTP method: PROPFIND    */
		PROPPATCH   = HTTP_METHOD_PROPPATCH,   /**< HTTP method: PROPPATCH   */
		MKCOL       = HTTP_METHOD_MKCOL        /**< HTTP method: MKCOL       */
	};

	/**	The underlying type for storing HTTP header entries. */
	using HeaderList = std::map<std::string, std::string>;

	/**
	 * Construct a HttpRequest object.
	 * @param method [in] The HTTP method to use when sending the request.
	 * @param url    [in] The target URL used to send the request.
	 */
	HttpRequest(Method method = Method::GET, const std::string& url = "");

	/**
	 * Queries the HTTP method currently set for the request.
	 * @return HttpRequest::Method
	 */
	const Method& get_method() const;

	/**
	 * Sets the HTTP method used for sending the request.
	 * @param method [in] The new HTTP method for sending the request.
	 */
	void set_method(Method method);

	/**
	 * Queries the current target URL of the request.
	 * @return std::string
	 */
	const std::string& get_url() const;

	/**
	 * Sets the target URL for the request.
	 * @param url [in] The new target URL for the request.
	 */
	void set_url(const std::string& url);

	/**
	 * Queries the header entries of the request.
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
	 * Sets the value of the specified HTTP header entry (creates new entry, if does not exist).
	 * @param key   [in] The name of the HTTP header entry.
	 * @param value [in] The new value of the HTTP header entry.
	 */
	void set_header(const std::string& key, const std::string& value);

	/**
	 * Deletes the HTTP header entry with the specified name.
	 * @param key [in] The name of the HTTP header entry.
	 */
	void delete_header(const std::string& key);

	/**
	 * Queries the current body of the HTTP request.
	 * @return std::string
	 */
	const std::string& get_body() const;

	/**
	 * Sets the body of the HTTP request to the specified string.
	 * @param data [in] The new content of the HTTP body.
	 */
	void set_body(const std::string& data);

	/**
	 * Sets the body of the HTTP request to the specified data.
	 * @param data   [in] An array of bytes to copy into the body.
	 * @param length [in] The number of bytes in the data array.
	 */
	void set_body(const uint8_t data[], std::size_t length);

private:

	Method      m_method;  /**< The HTTP method of the request.  */
	std::string m_url;     /**< The target URL for the request.  */
	std::string m_body;    /**< The body of the request.         */
	HeaderList  m_headers; /**< The HTTP headers of the request. */
};

#endif // HTTP_CLIENT_HTTP_REQUEST_H_INCLUDED
