#pragma once
#ifndef THINGSPEAK_THINGSPEAK_CLIENT_H_INCLUDED
#define THINGSPEAK_THINGSPEAK_CLIENT_H_INCLUDED

// Standard includes
#include <cstdint>
#include <string>
#include <sstream>

// Project includes
#include "http_client.h"
#include "json.h"


/**
 * This class is used to create updates to a ThingSpeak channel.
 * The interface grants read-write access to the 8 fields of a channel,
 * and can be passed to a ThingSpeakClient object for transmitting after
 * the proper initialization.
 */
class ThingSpeakUpdate {
public:

	/**
	 * Constructs a ThingSpeak update object.
	 */
	ThingSpeakUpdate();

	/**
	 * Queries a reference to the field with the specified index.
	 * @param  index [in] The index of the field to update [1-8].
	 * @return A reference to the field, using indices outside [1-8] are undefined.
	 */
	std::string& operator[](uint8_t index);

	/**
	 * Queries a const reference to the field with the specified index.
	 * @param  index [in] The index of the field to update [1-8].
	 * @return A reference to the field, using indices outside [1-8] are undefined.
	 */
	const std::string& operator[](uint8_t index) const;

	/**
	 * Updates the value of the field with the specified index.
	 * @param  index [in] The index of the field to update [1-8].
	 * @param  value [in] The value to write into the field.
	 * @return True on success, false of failure.
	 */
	template <class Type>
	bool setField(uint8_t index, const Type& value)
	{
		// Checking field index bounds
		if (index == 0 || index > 8) return false;

		// Creating string stream to convert data to string representation
		std::stringstream stream; stream << value;

		// Writing to the specified field
		m_fields[index - 1] = stream.str();

		// Success
		return true;
	}

private:
	std::string m_fields[8]; /**< The fields of the ThingSpeak channel. */
};

/**
 * This class is used to represent the result of a data query from a ThingSpeak
 * channel. The interface grants read-only access to the results of the query in
 * string format.
 */
class ThingSpeakQuery {
public:

	/**
	 * Constructs a ThingSpeak query result object.
	 * @param fields
	 */
	ThingSpeakQuery(const std::string fields[8] = nullptr);

	/**
	 * Queries the value of the field with the specified index.
	 * @param  index [in] The index of the field to read [1-8].
	 * @return A reference to the field, using indices outside [1-8] are undefined.
	 */
	const std::string& operator[](uint8_t index) const;

	/**
	 * Queries the value of the field with the specified index.
	 * @param  index [in] The index of the field to read [1-8].
	 * @return The value of the field or an empty string on error.
	 */
	std::string getField(uint8_t index) const;

private:
	std::string m_fields[8]; /**< The fields of the ThingSpeak channel. */
};

class ThingSpeakClient {
public:

	/**
	 * Constructs a ThingSpeakClient object with default API-keys.
	 * @param readKey  [in] The default API-key for reading channel data (read methods can override).
	 * @param writeKey [in] The default API-key for writing channel data (write methods can override).
	 */
	ThingSpeakClient(const std::string& readKey = "", const std::string& writeKey = "");

	/**
	 * Reads all fields from the specified channel.
	 * @param  channelID [in] The ID of the channel to read data from.
	 * @param  readKey   [in] The read API-key for the channel (optional).
	 * @return A ThingSpeakQuery object containing the data from the channel.
	 */
	ThingSpeakQuery readChannel(uint64_t channelID, const std::string& readKey = "");

	/**
	 * Reads a single field (last non-null value) from the specified channel.
	 * @param  channelID [in] The ID of the channel to read data from.
	 * @param  fieldID   [in] The ID of the field to read data from.
	 * @param  readKey   [in] The read API-key for the channel (optional).
	 * @return The value of the channel in string format.
	 */
	std::string readField(uint64_t channelID, uint8_t fieldID, const std::string& readKey = "");

	/**
	 * Updates a channel with the specified data.
	 * @param  update   [in] The update object containing the data to write.
	 * @param  writeKey [in] The write API-key for the channel (optional).
	 * @return True on success, false on failure.
	 */
	bool writeChannel(const ThingSpeakUpdate& update, const std::string& writeKey = "");

	/**
	 * Updates a single field of a channel with the specified data.
	 * @param  fieldID  [in] The ID of the field to update.
	 * @param  value    [in] The value to write into the field.
	 * @param  writeKey [in] The write API-key for the channel (optional).
	 * @return True on success, false on failure.
	 */
	bool writeField(uint8_t fieldID, const std::string& value, const std::string& writeKey = "");

private:
	HttpClient  m_client;   /**< The HTTP client used for communicating with ThingSpeak servers. */
	std::string m_readKey;  /**< The default read API-key.                                       */
	std::string m_writeKey; /**< The default write API-key.                                      */
};

#endif // THINGSPEAK_THINGSPEAK_CLIENT_H_INCLUDED
