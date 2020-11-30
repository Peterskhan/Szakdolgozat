#include "json.h"

/**
 * Parses the provided string and constructs a JsonObject from it.
 * @param  json [in] The JSON object in string format.
 * @return The constructed object hierarchy.
 */
JsonObject JsonObject::parse(const std::string& json)
{
	return JsonObject(cJSON_Parse(json.c_str()), true);
}

// Creating JSON objects based on type

/**
 * Creates a NULL object.
 * @return The constructed NULL object.
 */
JsonObject JsonObject::createNull()
{
	return JsonObject(cJSON_CreateNull(), true);
}

/**
 * Creates a Boolean JsonObject with the specified value.
 * @param  value [in] The value of the constructed object.
 * @return The constructed boolean object.
 */
JsonObject JsonObject::createBool(bool value)
{
	return JsonObject(cJSON_CreateBool(value), true);
}

/**
 * Creates a Number JsonObject with the specified value.
 * @param  value [in] The value of the constructed object.
 * @return The constructed number object.
 */
JsonObject JsonObject::createNumber(double value)
{
	return JsonObject(cJSON_CreateNumber(value), true);
}

/**
 * Creates a Number JsonObject with the specified value.
 * @param  value [in] The value of the constructed object.
 * @return The constructed number object.
 */
JsonObject JsonObject::createNumber(int value)
{
	return JsonObject(cJSON_CreateNumber(value), true);
}

/**
 * Creates a String JsonObject with the specified value.
 * @param  value [in] The value of the constructed object.
 * @return The constructed string object.
 */
JsonObject JsonObject::createString(const std::string& value)
{
	return JsonObject(cJSON_CreateString(value.c_str()), true);
}

/**
 * Creates an empty Array JsonObject.
 * @return The constructed array object.
 */
JsonObject JsonObject::createArray()
{
	return JsonObject(cJSON_CreateArray(), true);
}

/**
 * Creates an empty JsonObject.
 * @return The constructed object.
 */
JsonObject JsonObject::createObject()
{
	return JsonObject(cJSON_CreateObject(), true);
}

// Type-checking for objects

/**
 * Checks whether the object is invalid.
 * @return True when the object is invalid.
 */
bool JsonObject::isInvalid() const
{
	return cJSON_IsInvalid(m_object);
}

/**
 * Checks whether the object is a boolean.
 * @return True when the object is a boolean.
 */
bool JsonObject::isBool() const
{
	return cJSON_IsBool(m_object);
}

/**
 * Checks whether the object is NULL.
 * @return True when the object is NULL.
 */
bool JsonObject::isNull() const
{
	return (m_object == nullptr) || cJSON_IsNull(m_object);
}

/**
 * Checks whether the object is a number.
 * @return True when the object is a number.
 */
bool JsonObject::isNumber() const
{
	return cJSON_IsNumber(m_object);
}

/**
 * Checks whether the object is a string.
 * @return True when the object is a string.
 */
bool JsonObject::isString() const
{
	return cJSON_IsString(m_object);
}

/**
 * Checks whether the object is an array.
 * @return True when the object is an array.
 */
bool JsonObject::isArray() const
{
	return cJSON_IsArray(m_object);
}

/**
 * Checks whether the object is a valid object.
 * @return True when the object is a valid object.
 */
bool JsonObject::isObject() const
{
	return cJSON_IsObject(m_object);
}

// Adding new fields to an object

/**
 * Adds a NULL object to this object with the specified name.
 * @param name [in] The name of the NULL object.
 */
void JsonObject::addNull(const std::string& name)
{
	cJSON_AddNullToObject(m_object, name.c_str());
}

/**
 * Adds a boolean object to this object with the specified name.
 * @param name  [in] The name of the boolean object.
 * @param value [in] The value of the boolean object.
 */
void JsonObject::addBool(const std::string& name, bool value)
{
	cJSON_AddBoolToObject(m_object, name.c_str(), value);
}

/**
 * Adds a number object to this object with the specified name.
 * @param name  [in] The name of the number object.
 * @param value [in] The value of the number object.
 */
void JsonObject::addNumber(const std::string& name, double value)
{
	cJSON_AddNumberToObject(m_object, name.c_str(), value);
}

/**
 * Adds a number object to this object with the specified name.
 * @param name  [in] The name of the number object.
 * @param value [in] The value of the number object.
 */
void JsonObject::addNumber(const std::string& name, int value)
{
	cJSON_AddNumberToObject(m_object, name.c_str(), value);
}

/**
 * Adds a string object to this object with the specified name.
 * @param name  [in] The name of the string object.
 * @param value [in] The value of the string object.
 */
void JsonObject::addString(const std::string& name, const std::string& value)
{
	cJSON_AddStringToObject(m_object, name.c_str(), value.c_str());
}

/**
 * Adds an array object to this object with the specified name.
 * @param name [in] The name of the array object.
 */
void JsonObject::addArray(const std::string& name)
{
	cJSON_AddArrayToObject(m_object, name.c_str());
}

/**
 * Adds an object to this object with the specified name.
 * @param name [in] The name of the object.
 */
void JsonObject::addObject(const std::string& name)
{
	cJSON_AddObjectToObject(m_object, name.c_str());
}

/**
 * Adds an existing object to this object with the specified name.
 * @param object
 */
void JsonObject::addObject(const std::string& name, JsonObject object)
{
	// Checking if the object is a valid object
	if (isObject()) {

		// Adding object to this object
		cJSON_AddItemToObject(m_object, name.c_str(), object.m_object);

		// Indicating that this object took ownership of the object
		object.m_isRoot = false;
	}
}

/**
 * Adds an item to an array object.
 * @param  item [in] The item to add into the array.
 */
void JsonObject::addItem(JsonObject item)
{
	// Checking if the object is an array
	if (isArray()) {

		// Adding item to the array
		cJSON_AddItemToArray(m_object, item.m_object);

		// Indicating that the array took ownership of the item
		item.m_isRoot = false;
	}
}

// Printing JSON objects

/**
 * Prints the content of this object to a string.
 * @return The content of this object in a JSON string.
 */
std::string JsonObject::print() const {

	// Allocating memory and printing JSON string
	char* json = cJSON_Print(m_object);

	// Creating std::string from char array
	std::string result(json);

	// Releasing memory of the allocated string
	free(json);

	// Returning printed JSON object string
	return result;
}

// Querying JSON objects

/**
 * Queries this object for an object with the specified name.
 * @param  name [in] The name of the object to query.
 * @return The object when found, or a NULL object.
 */
JsonObject JsonObject::operator[](const std::string& name)
{
	return JsonObject(cJSON_GetObjectItemCaseSensitive(m_object, name.c_str()), false);
}

/**
 * Queries this object for an object with the specified name.
 * @param  name [in] The name of the object to query.
 * @return The object when found, or a NULL object.
 */
const JsonObject JsonObject::operator[](const std::string& name) const
{
	return JsonObject(cJSON_GetObjectItemCaseSensitive(m_object, name.c_str()), false);
}

/**
 * Queries the item with the specified index from an array object.
 * @param  index [in] The index of the item to query.
 * @return The object at the specified index when found, or a NULL object.
 */
JsonObject JsonObject::operator[](std::size_t index)
{
	return JsonObject(cJSON_GetArrayItem(m_object, index), false);
}

/**
 * Queries the item with the specified index from an array object.
 * @param  index [in] The index of the item to query.
 * @return The object at the specified index when found, or a NULL object.
 */
const JsonObject JsonObject::operator[](std::size_t index) const
{
	return JsonObject(cJSON_GetArrayItem(m_object, index), false);
}

// Getting data from JSON objects

/**
 * Queries the boolean value stored in this object (not checked).
 * @return The boolean value stored in this object.
 */
bool JsonObject::getBool() const
{
	return cJSON_IsTrue(m_object);
}

/**
 * Queries the integer value stored in this object (not checked).
 * @return The integer value stored in this object.
 */
int JsonObject::getInteger() const
{
	return m_object->valueint;
}

/**
 * Queries the double value stored in this object (not checked).
 * @return The double value stored in this object.
 */
double JsonObject::getDouble() const
{
	return m_object->valuedouble;
}

/**
 * Queries the string value stored in this object (not checked).
 * @return The string value stored in this object.
 */
std::string JsonObject::getString() const
{
	return std::string(m_object->valuestring);
}

// Comparing JSON objects

/**
 * Compares this JSON object with another object.
 * @param  other [in] The other object to compare to.
 * @return True when the two objects are equal.
 */
bool JsonObject::operator==(const JsonObject& other) const
{
	return cJSON_Compare(m_object, other.m_object, true);
}

JsonObject::JsonObject(cJSON* object, bool isRoot)
	: m_object(object), m_isRoot(isRoot)
{
	if(object == nullptr) {
		m_object = cJSON_CreateObject();
		m_isRoot = true;
	}
}

JsonObject::JsonObject(const JsonObject& other)
	: m_object(cJSON_Duplicate(other.m_object, true)), m_isRoot(other.m_isRoot)
{}

JsonObject::~JsonObject()
{
	//if(m_isRoot) cJSON_Delete(m_object);
}
