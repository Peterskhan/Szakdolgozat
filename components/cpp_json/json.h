#pragma once
#ifndef JSON_JSON_H_INCLUDED
#define JSON_JSON_H_INCLUDED

// Standard includes
#include <string>

// ESP-IDF includes
#include "cJSON.h"

class JsonObject {
public:

	// Creating JSON objects from strings

	/**
	 * Parses the provided string and constructs a JsonObject from it.
	 * @param  json [in] The JSON object in string format.
	 * @return The constructed object hierarchy.
	 */
	static JsonObject parse(const std::string& json);

	// Creating JSON objects based on type

	/**
	 * Creates a NULL object.
	 * @return The constructed NULL object.
	 */
	static JsonObject createNull();

	/**
	 * Creates a Boolean JsonObject with the specified value.
	 * @param  value [in] The value of the constructed object.
	 * @return The constructed boolean object.
	 */
	static JsonObject createBool(bool value);

	/**
	 * Creates a Number JsonObject with the specified value.
	 * @param  value [in] The value of the constructed object.
	 * @return The constructed number object.
	 */
	static JsonObject createNumber(double value);

	/**
	 * Creates a Number JsonObject with the specified value.
	 * @param  value [in] The value of the constructed object.
	 * @return The constructed number object.
	 */
	static JsonObject createNumber(int value);

	/**
	 * Creates a String JsonObject with the specified value.
	 * @param  value [in] The value of the constructed object.
	 * @return The constructed string object.
	 */
	static JsonObject createString(const std::string& value);

	/**
	 * Creates an empty Array JsonObject.
	 * @return The constructed array object.
	 */
	static JsonObject createArray();

	/**
	 * Creates an empty JsonObject.
	 * @return The constructed object.
	 */
	static JsonObject createObject();

	// Creating empty JSON objects

	/**
	 * Constructs a default JsonObject from a cJSON implementation pointer.
	 * @param object [in] Pointer to the cJSON implementation.
	 */
	JsonObject(cJSON* object = nullptr, bool isRoot = true);

	// Copying JSON objects

	/**
	 * Constructs a deep copy of the other object.
	 * @param other [in] The other JSON object to copy.
	 */
	JsonObject(const JsonObject& other);

	// Type-checking for objects

	/**
	 * Checks whether the object is invalid.
	 * @return True when the object is invalid.
	 */
	bool isInvalid() const;

	/**
	 * Checks whether the object is a boolean.
	 * @return True when the object is a boolean.
	 */
	bool isBool() const;

	/**
	 * Checks whether the object is NULL.
	 * @return True when the object is NULL.
	 */
	bool isNull() const;

	/**
	 * Checks whether the object is a number.
	 * @return True when the object is a number.
	 */
	bool isNumber() const;

	/**
	 * Checks whether the object is a string.
	 * @return True when the object is a string.
	 */
	bool isString() const;

	/**
	 * Checks whether the object is an array.
	 * @return True when the object is an array.
	 */
	bool isArray() const;

	/**
	 * Checks whether the object is a valid object.
	 * @return True when the object is a valid object.
	 */
	bool isObject() const;

	// Adding new fields to an object

	/**
	 * Adds a NULL object to this object with the specified name.
	 * @param name [in] The name of the NULL object.
	 */
	void addNull(const std::string& name);

	/**
	 * Adds a boolean object to this object with the specified name.
	 * @param name  [in] The name of the boolean object.
	 * @param value [in] The value of the boolean object.
	 */
	void addBool(const std::string& name, bool value);

	/**
	 * Adds a number object to this object with the specified name.
	 * @param name  [in] The name of the number object.
	 * @param value [in] The value of the number object.
	 */
	void addNumber(const std::string& name, double value);

	/**
	 * Adds a number object to this object with the specified name.
	 * @param name  [in] The name of the number object.
	 * @param value [in] The value of the number object.
	 */
	void addNumber(const std::string& name, int value);

	/**
	 * Adds a string object to this object with the specified name.
	 * @param name  [in] The name of the string object.
	 * @param value [in] The value of the string object.
	 */
	void addString(const std::string& name, const std::string& value);

	/**
	 * Adds an array object to this object with the specified name.
	 * @param name [in] The name of the array object.
	 */
	void addArray(const std::string& name);

	/**
	 * Adds an object to this object with the specified name.
	 * @param name [in] The name of the object.
	 */
	void addObject(const std::string& name);

	/**
	 * Adds an existing object to this object with the specified name.
	 * @param object
	 */
	void addObject(const std::string& name, JsonObject object);

	/**
	 * Adds an item to an array object.
	 * @param  item [in] The item to add into the array.
	 */
	void addItem(JsonObject item);

	// Printing JSON objects

	/**
	 * Prints the content of this object to a string.
	 * @return The content of this object in a JSON string.
	 */
	std::string print() const;

	// Querying JSON objects

	/**
	 * Queries this object for an object with the specified name.
	 * @param  name [in] The name of the object to query.
	 * @return The object when found, or a NULL object.
	 */
	JsonObject operator[](const std::string& name);

	/**
	 * Queries this object for an object with the specified name.
	 * @param  name [in] The name of the object to query.
	 * @return The object when found, or a NULL object.
	 */
	const JsonObject operator[](const std::string& name) const;

	/**
	 * Queries the item with the specified index from an array object.
	 * @param  index [in] The index of the item to query.
	 * @return The object at the specified index when found, or a NULL object.
	 */
	JsonObject operator[](std::size_t index);

	/**
	 * Queries the item with the specified index from an array object.
	 * @param  index [in] The index of the item to query.
	 * @return The object at the specified index when found, or a NULL object.
	 */
	const JsonObject operator[](std::size_t index) const;

	// Getting data from JSON objects

	/**
	 * Queries the boolean value stored in this object (not checked).
	 * @return The boolean value stored in this object.
	 */
	bool getBool() const;

	/**
	 * Queries the integer value stored in this object (not checked).
	 * @return The integer value stored in this object.
	 */
	int getInteger() const;

	/**
	 * Queries the double value stored in this object (not checked).
	 * @return The double value stored in this object.
	 */
	double getDouble() const;

	/**
	 * Queries the string value stored in this object (not checked).
	 * @return The string value stored in this object.
	 */
	std::string getString() const;

	// Comparing JSON objects

	/**
	 * Compares this JSON object with another object.
	 * @param  other [in] The other object to compare to.
	 * @return True when the two objects are equal.
	 */
	bool operator==(const JsonObject& other) const;

	// Destroy JSON objects

	/**
	 * Destroys a JsonObject.
	 */
	~JsonObject();

private:

	/**< The internal representation of the object. */
	cJSON* m_object;

	/**< Flag indicating whether this object is the root object. */
	bool m_isRoot;
};

#endif // JSON_JSON_H_INCLUDED
