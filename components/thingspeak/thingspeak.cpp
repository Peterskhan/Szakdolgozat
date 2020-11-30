#include <thingspeak.h>


ThingSpeakUpdate::ThingSpeakUpdate()
{
	for(std::string& value : m_fields) value = "";
}

std::string& ThingSpeakUpdate::operator[](uint8_t index)
{
	return m_fields[index - 1];
}

const std::string& ThingSpeakUpdate::operator[](uint8_t index) const
{
	return m_fields[index - 1];
}

ThingSpeakQuery::ThingSpeakQuery(const std::string fields[8])
{
	// Checking the provided array for nullptr
	if(fields) {

		// Copying the elements to the internal fields array
		for(std::size_t i = 0; i < 8; i++) {
			m_fields[i] = fields[i];
		}
	}
}

const std::string& ThingSpeakQuery::operator[](uint8_t index) const
{
	return m_fields[index - 1];
}

std::string ThingSpeakQuery::getField(uint8_t index) const
{
	// Checking index bounds
	if (index == 0 || index > 8) return "";

	// Returning field value
	return m_fields[index - 1];
}

/**
 * Constructs a ThingSpeakClient object with default API-keys.
 * @param readKey  [in] The default API-key for reading channel data (read methods can override).
 * @param writeKey [in] The default API-key for writing channel data (write methods can override).
 */
ThingSpeakClient::ThingSpeakClient(const std::string& readKey, const std::string& writeKey)
	: m_readKey(readKey), m_writeKey(writeKey)
{}

/**
 * Reads all fields from the specified channel.
 * @param  channelID [in] The ID of the channel to read data from.
 * @param  readKey   [in] The read API-key for the channel (optional).
 * @return A ThingSpeakQuery object containing the data from the channel.
 */
ThingSpeakQuery ThingSpeakClient::readChannel(uint64_t channelID, const std::string& readKey)
{
	// Creating request URL
	std::string url = "https://api.thingspeak.com/channels/" + std::to_string(channelID) + "/feeds.json";

	// Appending read API-key to the request
	if (readKey != "") url += ("?api_key=" + readKey);
	else               url += ("?api_key=" + m_readKey);

	// Appending number of queried results field to the URL
	url += "&results=1";

	// Creating HTTP request
	HttpRequest request(HttpRequest::Method::GET, url);

	// Performing the HTTP query request operation
	HttpResponse response = m_client.send_request(request);

	// Checking successful reading
	if(response.get_status_code() != 200) return ThingSpeakQuery();

	// Parsing returned JSON data from the server
	JsonObject data = JsonObject::parse(response.get_body());

	// Extracting returned data from the response
	std::string fields[8];
	fields[0] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field1"].getString();
	fields[1] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field2"].getString();
	fields[2] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field3"].getString();
	fields[3] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field4"].getString();
	fields[4] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field5"].getString();
	fields[5] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field6"].getString();
	fields[6] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field7"].getString();
	fields[7] = data["feeds"][0]["field1"].isNull() ? "" : data["feeds"][0]["field8"].getString();

	return ThingSpeakQuery(fields);
}

/**
 * Reads a single field (last non-null value) from the specified channel.
 * @param  channelID [in] The ID of the channel to read data from.
 * @param  fieldID   [in] The ID of the field to read data from.
 * @param  readKey   [in] The read API-key for the channel (optional).
 * @return The value of the channel in string format.
 */
std::string ThingSpeakClient::readField(uint64_t channelID, uint8_t fieldID, const std::string& readKey)
{
	// Creating request URL
	std::string url = "https://api.thingspeak.com/channels/" + std::to_string((int) channelID) +
		                                        "/fields/"   + std::to_string((int) fieldID)   + "/last.json";

	// Appending read API-key to the request
	if (readKey != "") url += ("?api_key=" + readKey);
	else               url += ("?api_key=" + m_readKey);

	// Appending number of queried results field to the URL
	url += "&results=1";

	// Creating HTTP request
	HttpRequest request(HttpRequest::Method::GET, url);

	// Performing the HTTP query request operation
	HttpResponse response = m_client.send_request(request);

	// Checking successful reading
	if(response.get_status_code() != 200) return "";

	// Parsing returned JSON data from the server
	JsonObject data = JsonObject::parse(response.get_body());

	// Extracting returned data from the response
	std::string result;
	switch(fieldID) {
	case 1: result = data["field1"].isNull() ? "" : data["field1"].getString(); break;
	case 2: result = data["field2"].isNull() ? "" : data["field2"].getString(); break;
	case 3: result = data["field3"].isNull() ? "" : data["field3"].getString(); break;
	case 4: result = data["field4"].isNull() ? "" : data["field4"].getString(); break;
	case 5: result = data["field5"].isNull() ? "" : data["field5"].getString(); break;
	case 6: result = data["field6"].isNull() ? "" : data["field6"].getString(); break;
	case 7: result = data["field7"].isNull() ? "" : data["field7"].getString(); break;
	case 8: result = data["field8"].isNull() ? "" : data["field8"].getString(); break;
	default: result = "";
	}

	return result;
}

/**
 * Updates a channel with the specified data.
 * @param  update   [in] The update object containing the data to write.
 * @param  writeKey [in] The write API-key for the channel (optional).
 * @return True on success, false on failure.
 */
bool ThingSpeakClient::writeChannel(const ThingSpeakUpdate& update, const std::string& writeKey)
{
	// Creating HTTP request
	HttpRequest request(HttpRequest::Method::POST, "https://api.thingspeak.com/update");
	request.set_header("Host", "api.thingspeak.com");
	request.set_header("Content-Type", "application/x-www-form-urlencoded");

	// Populating string stream with write API-key
	std::stringstream stream;
	if(writeKey != "") stream << "api_key=" << writeKey;
	else               stream << "api_key=" << m_writeKey;

	// Populating string stream with update data
	for(uint8_t i = 1; i <= 8; i++) {
		if(update[i] != "") {
			stream << "&field" << (int) i << "=" << update[i];
		}
	}

	// Setting update post data
	request.set_body(stream.str());

	// Performing the HTTP update request operation
	HttpResponse response = m_client.send_request(request);

	// Returning status information
	return response.get_status_code() == 200;
}

/**
 * Updates a single field of a channel with the specified data.
 * @param  fieldID  [in] The ID of the field to update.
 * @param  value    [in] The value to write into the field.
 * @param  writeKey [in] The write API-key for the channel (optional).
 * @return True on success, false on failure.
 */
bool ThingSpeakClient::writeField(uint8_t fieldID, const std::string& value, const std::string& writeKey)
{
	// Creating HTTP request
	HttpRequest request(HttpRequest::Method::POST, "https://api.thingspeak.com/update");
	request.set_header("Host", "api.thingspeak.com");
	request.set_header("Content-Type", "application/x-www-form-urlencoded");

	// Populating string stream with request post data
	std::stringstream stream;
	if(writeKey != "") stream << "api_key=" << writeKey   << "&" << "field" << (int) fieldID << "=" << value;
	else               stream << "api_key=" << m_writeKey << "&" << "field" << (int) fieldID << "=" << value;

	// Setting update post data
	request.set_body(stream.str());

	// Performing the HTTP update request operation
	HttpResponse response = m_client.send_request(request);

	// Returning status information
	return response.get_status_code() == 200;
}
