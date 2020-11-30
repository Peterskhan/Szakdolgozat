#pragma once
#ifndef DATAFLOW_ANY_HPP_INCLUDED
#define DATAFLOW_ANY_HPP_INCLUDED

// Determining whether Runtime-Type-Information (RTTI) is enabled
#if defined(__clang__)
  #if __has_feature(cxx_rtti)
    #define RTTI_ENABLED
  #endif
#elif defined(__GNUG__)
  #if defined(__GXX_RTTI)
    #define RTTI_ENABLED
  #endif
#elif defined(_MSC_VER)
  #if defined(_CPPRTTI)
    #define RTTI_ENABLED
  #endif
#endif

// Determining whether C++ exceptions are enabled
#if defined(__clang__)
  #if defined(__cpp_exceptions)
    #define EXCEPTIONS_ENABLED
  #endif
#elif defined(__GNUG__)
  #if defined(__cpp_exceptions)
    #define EXCEPTIONS_ENABLED
  #endif
#endif



// Standard includes
#include <typeinfo>
#include <iostream>
#include <string>


/**
 * @brief The is_printable traits class is used to determine if a value
 *        of a specific type is printable to an std::ostream. This class
 *        serves as the base type for unprintable types.
 */
template<typename T, typename = void>
struct is_printable: std::false_type {};

/**
 * @brief This template specialization of the is_printable traits class
 *        implements the compile-time logic to decide if a specific type
 *        is printable to an std::ostream.
 */
template<typename T>
struct is_printable<T, decltype(std::cout << std::declval<T>(), void())> : std::true_type {};


/**
 * @brief The any class implements a type-safe generic storage class which is
 *        capable of storing values of any type. Depending on the stored type,
 *        objects of this class are constructable, copyable and assignable.
 *        Values stored in an any object are printable to an std::ostream, if
 *        the stored type supports it. The stored values are retrieveable from
 *        the any object in a type-safe fashion by explicitly casting to the
 *        exact type of the stored value.
 */
class any {
public:

    /**
     * @brief Constructs an empty any object, not storing any values.
     */
    any() noexcept;

    /**
     * @brief Constructs an any object by copying another any object.
     * @param other [in] The other any object to copy.
     */
    any(const any& other);

    /**
     * @brief Constructs an any by moving from another any object.
     * @param other [in] The other any object to move from.
     */
    any(any&& other) noexcept;

    /**
     * @brief Constructs an any object from the specified value of arbitrary type.
     * @param value [in] The value to store in the constructed any object.
     */
    template <class Type>
    explicit any(Type&& value) : m_object(nullptr), m_vtable(nullptr)
    {
        // Making a copy of the supplied value (reference and CV qualifiers removed)
        helper<typename std::decay<Type>::type>(value, Action::SET);
    }

    /**
     * @brief Constructs an any object from a C-string by using std::string.
     * @param string [in] The C-string to construct the any object from.
     */
    any(const char* string);

    /**
     * @brief  Copies another any object.
     * @param  other [in] The other any object to copy.
     * @return Reference to this object for chaining assignments.
     */
    any& operator=(const any& other);

    /**
     * @brief  Moves from another any object.
     * @param  other [in] The other any object to move from.
     * @return Reference to this object for chaining assignments.
     */
    any& operator=(any&& other) noexcept;

    /**
     * @brief  Assigns a new value of arbitrary type to this any object.
     * @param  value [in] The new value to be stored in this object.
     * @return Reference to this object for chaining assingments.
     */
    template <class Type>
    any& operator=(Type&& value)
    {
        // Clearing previously stored value and assigning new value
        helper<typename std::decay<Type>::type>(value, Action::SET);
        return *this;
    }

    /**
     * @brief  Assigns a new C-string to this any object.
     * @param  value [in] The string to store in this object.
     * @return Reference to this object for chaining assignments.
     */
    any& operator=(const char* value);

    /**
     * Destroys the any object and deallocates all used resources.
     */
    virtual ~any();

    /**
     * @brief Clears the value stored in this object, making it empty again.
     */
    void reset() noexcept;

    /**
     * @brief  Queries whether this object stores a value.
     * @return True when a value is stored in the object.
     */
    bool has_value() const noexcept;

    /**
     * @brief  Determines whether the any object stores the specified type.
     * @return True when the type stored is identical to the template parameter.
     */
    template <class Type>
    bool hasType() noexcept
    {
        try {
            // Getting the type we want to check against
            using type = typename std::decay<Type>::type;

            // Attempting to read with that type
            Type dummy = helper<type>(Type{}, Action::GET);

            // Suppressing compiler warning for unused dummy
            (void)(dummy);
        }
        catch(...) { return false; }

        // No invalid conversion exception occured, the types are matching
        return true;
    }

#if defined(RTTI_ENABLED)

    /**
     * @brief  Queries the type information for the stored value.
     * @return The type information object for the stored value.
     */
    const std::type_info& type() const noexcept;

#endif

    /**
     * @brief Converts this any object to the specified type when applicable,
     *        or throws an exception on invalid type conversions.
     */
    template <class Type>
    explicit operator Type()
    {
        // Returning a copy of the stored value or throwing exception on invalid conversion
        return helper<typename std::decay<Type>::type>(Type{}, Action::GET);
    }

private:

    /**
     * @brief The Action enum defines the possible operations on the stored values.
     */
    enum class Action { GET, SET };

    /**
     * @brief Deletes the value contained within the NodeValue object.
     * @param object [in] Pointer to the value, type safety is maintained via the VTABLE.
     */
    template <class Type>
    static void deleteFunction(void* object)
    {
        // Checking if the value was stored with Small-Buffer-Optimalization
        if (sizeof(Type) <= sizeof(void*)) {

            // Invoking the destructor of the value for manual cleanup,
            // no memory deallocation needs to take place
            static_cast<Type*>(object)->~Type();
        }

        // Checking if the value was stored without Small-Buffer-Optimalization
        else {

            // Calling the destructor and releasing memory of the value
            delete static_cast<Type*>(object);
        }
    }

    /**
     * @brief  Makes a dynamic copy of the value contained within the any object.
     * @param  object [in] Pointer to the any object, to make the copy from.
     * @param  value  [in] Pointer to the value, type safety is maintained via the VTABLE.
     */
    template <class Type>
    static auto cloneFunction(any* object, const void* value) -> typename std::enable_if<(sizeof(Type) > sizeof(void*)), void>::type
    {
        // Making a dynamically allocated copy (not using Small-Object-Optimalization)
        object->m_object = new Type(*static_cast<const Type*>(value));
    }

    /**
     * @brief  Makes an in-place copy of the value contained within the any object.
     * @param  object [in] Pointer to the any object, to make the copy from.
     * @param  value  [in] Pointer to the value, type safety is maintained via the VTABLE.
     */
    template <class Type>
    static auto cloneFunction(any* object, const void* value) -> typename std::enable_if<(sizeof(Type) <= sizeof(void*)), void>::type
    {
        // Making an in-place copy of the value stored instead of the m_object pointer
        new (&(object->m_object)) Type(*static_cast<const Type*>(value));
    }

    /**
     * @brief  Prints the value stored in the any object to the specified std::ostream.
     * @param  stream [in] The stream to print the value to.
     * @param  object [in] Pointer to the any object to print.
     * @return Reference to the stream, for chaining print statements.
     */
    template <class Type>
    static auto printFunction(std::ostream& stream, const any* object) -> typename std::enable_if<is_printable<Type>::value, std::ostream&>::type
    {
        if(sizeof(Type) <= sizeof(void*)) return stream << *reinterpret_cast<const Type*>(&(object->m_object));

        return stream << *static_cast<Type*>(object->m_object);
    }

    /**
     * @brief  Prints question marks to the specified stream (used only for unprintable value types).
     * @param  stream [in] The stream to print the value to.
     * @param  object [in] Pointer to the any object to print.
     * @return Reference to the stream, for chaining print statements.
     */
    template <class Type>
    static auto printFunction(std::ostream& stream, const any* object) -> typename std::enable_if<!is_printable<Type>::value, std::ostream&>::type
    {
        // Can not print the value, because the type does not support it
        return stream << "???";

        // Suppress compiler warning for unused variable
        (void)(object);
    }

    // Friend declaration for printing any objects to an output stream
    friend std::ostream& operator<<(std::ostream&, const any&);

    /**
     * @brief  Helper function to get and set values contained within the any object,
     *         differentiated based on the template type parameter.
     * @param  value  [in] Used as input for writing, unused when reading.
     * @param  action [in] The action to perform: read (GET) and write (SET)
     * @return The value contained on reading, the supplied value on writing.
     */
    template <class Type>
    Type helper(Type value, Action action)
    {
        // Static Virtual Table unique to every type used with any objects.
        // It stores function pointers for deleting and copying specific types
        // for the contained values and other type specific information
        static VTableTyped<Type> vtable;

        switch(action) {
        case Action::GET:

#if defined(EXCEPTIONS_ENABLED)

            // Check if the types are matching by comparing VTABLE pointers
            if(m_vtable != &vtable) throw std::bad_cast();

#endif

            // Returning the stored object (Small-Object-Optimalization)
            if(sizeof(Type) <= sizeof(void*)) return *reinterpret_cast<Type*>(&m_object);

            // Returning the the stored object (No optimalization)
            return *(static_cast<Type*>(m_object));

        case Action::SET:

            // Deleting the currently stored object with Small-Buffer-Optimalization
            if(m_vtable && m_vtable->m_size <= sizeof(void*)) {
                m_vtable->m_deleteFunction(&m_object);
            }

            // Deleting the currently stored object without Small-Buffer-Optimalization
            if(m_vtable && m_vtable->m_size > sizeof(void*)) {
                m_vtable->m_deleteFunction(m_object);
            }

            // Updating VTABLE to refer to the the new type
            m_vtable = &vtable;

            // Making a copy of the supplied value
            m_vtable->m_cloneFunction(this, &value);

            return value;
        };

        // Suppress compiler warning, should never get here...
        return value;
    }

    /**
     * @brief The VTable class implements a common base class for type-specific
     *        virtual tables. It defines the common function pointer signatures.
     */
    class VTable {
    public:

        /**
         * Function pointer signature for deleting values.
         */
        using deleteFunction = void (*)(void*);

        /**
         * Function pointer signature for copying values.
         */
        using cloneFunction  = void (*)(any*, const void*);

        /**
         * Function pointer signature for printing values.
         */
        using printFunction  = std::ostream& (*)(std::ostream&, const any*);

#if defined(RTTI_ENABLED)

        /**
         * Alias for the type information descriptor type.
         */
        using typeInfo = const std::type_info;

        /**
         * @brief Constructs a VTable object with the specified function pointers.
         * @param fpDelete [in] Pointer to the function used for deleting values.
         * @param fpClone  [in] Pointer to the function used for copying values.
         * @param typeInfo [in] Pointer to the type information data.
         */
        VTable(deleteFunction fpDelete, cloneFunction fpClone, printFunction fpPrint,
               std::size_t size, typeInfo* typeInfo);

        deleteFunction  m_deleteFunction; /**< Function pointer for deleting values. */
        cloneFunction   m_cloneFunction;  /**< Function pointer for copying values.  */
        printFunction   m_printFunction;  /**< Function pointer for printing values. */
        std::size_t     m_size;           /**< The byte-size of the referenced type. */
        typeInfo*       m_typeInfo;       /**< Pointer to the type information.      */

#else

        /**
		 * @brief Constructs a VTable object with the specified function pointers.
		 * @param fpDelete [in] Pointer to the function used for deleting values.
		 * @param fpClone  [in] Pointer to the function used for copying values.
		 */
		VTable(deleteFunction fpDelete, cloneFunction fpClone, printFunction fpPrint,
			   std::size_t size);

		deleteFunction  m_deleteFunction; /**< Function pointer for deleting values. */
		cloneFunction   m_cloneFunction;  /**< Function pointer for copying values.  */
		printFunction   m_printFunction;  /**< Function pointer for printing values. */
		std::size_t     m_size;           /**< The byte-size of the referenced type. */

#endif

    };

    /**
     * @brief The VTableTyped class implements a type-specific VTABLE. It is used for
     *        providing the type-safe delete, copy and print operations.
     */
    template <class Type>
    class VTableTyped : public VTable {
    public:

        /**
         * @brief Constructs a type-specific VTABLE.
         */
        VTableTyped() : VTable(
            any::deleteFunction<typename std::decay<Type>::type>,
            any::cloneFunction<typename std::decay<Type>::type>,
            any::printFunction<typename std::decay<Type>::type>,

#if defined(RTTI_ENABLED)
			sizeof(Type), &typeid(Type))
#else
    		sizeof(Type))
#endif
        {}
    };

    void*   m_object; /**< Pointer to the actual value stored by the NodeValue. */
    VTable* m_vtable; /**< Pointer to the virtual table for the current type.   */
};

/**
 * @brief  Prints an any object to the specified output stream.
 * @param  stream [in] The output stream to print the any object to.
 * @param  node   [in] The any object to print to the output stream.
 * @return Reference to the output stream for chaining print statements.
 */
std::ostream& operator<<(std::ostream& stream, const any& node);

#endif // DATAFLOW_ANY_HPP_INCLUDED
