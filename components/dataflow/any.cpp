#include "any.hpp"

any::any() noexcept : m_object(nullptr), m_vtable(nullptr) {}

any::any(const any& other) : m_object(nullptr), m_vtable(other.m_vtable)
{
    // Checking if there is a value to copy without Small-Buffer-Optimalization
    if(other.m_vtable && m_vtable->m_size > sizeof(void*)) {
        m_vtable->m_cloneFunction(this, other.m_object);
    }

    // Checking if there is a value to copy with Small-Buffer-Optimalization
    else if(other.m_vtable && m_vtable->m_size <= sizeof(void*)) {
        m_vtable->m_cloneFunction(this, &other.m_object);
    }
}

any::any(any&& other) noexcept : m_object(other.m_object), m_vtable(other.m_vtable)
{
    other.m_object = nullptr;
    other.m_vtable = nullptr;
}

any::any(const char* string) : any((std::string) string) {}

any& any::operator=(const any& other)
{
    // Checking self-assignment
    if(&other == this) return *this;

    // Checking if there is a value to delete without Small-Buffer-Optimalization
    if(m_vtable && m_vtable->m_size > sizeof(void*)) {
        m_vtable->m_deleteFunction(m_object);
    }

    // Checking if there is a value to copy with Small-Buffer-Optimalization
    else if(m_vtable && m_vtable->m_size <= sizeof(void*)) {
        m_vtable->m_deleteFunction(&m_object);
    }

    // Making a copy of the other NodeValue's VTABLE pointer
    m_vtable = other.m_vtable;

    // Checking if there is a value to copy without Small-Buffer-Optimalization
    if(other.m_vtable && m_vtable->m_size > sizeof(void*)) {
        m_vtable->m_cloneFunction(this, other.m_object);
    }

    // Checking if there is a value to copy with Small-Buffer-Optimalization
    else if(other.m_vtable && m_vtable->m_size <= sizeof(void*)) {
        m_vtable->m_cloneFunction(this, &other.m_object);
    }

    // Return reference to this object for chaining assignments
    return *this;
}

any& any::operator=(any&& other) noexcept
{
    // Checking self-assignment
    if(&other == this) return *this;

    // Checking if there is a value to delete without Small-Buffer-Optimalization
    if(m_vtable && m_vtable->m_size > sizeof(void*)) {
        m_vtable->m_deleteFunction(m_object);
    }

    // Checking if there is a value to copy with Small-Buffer-Optimalization
    else if(m_vtable && m_vtable->m_size <= sizeof(void*)) {
        m_vtable->m_deleteFunction(&m_object);
    }

    // Moving data from the other any object
    m_vtable = other.m_vtable;
    m_object = other.m_object;

    // Clearing the other any object
    other.m_vtable = nullptr;
    other.m_object = nullptr;

    return *this;
}

any& any::operator=(const char* value)
{
    // Delegating to the std::string specialized assignment
    return operator=((std::string) value);
}

any::~any()
{
    reset();
}

void any::reset() noexcept
{
    // Deleting the stored value with Small-Buffer-Optimalization
    if(m_vtable && m_vtable->m_size <= sizeof(void*)) m_vtable->m_deleteFunction(&m_object);

    // Deleting the stored value without Small-Buffer-Optimalization
    else if(m_vtable && m_vtable->m_size > sizeof(void*)) m_vtable->m_deleteFunction(m_object);

    // Resetting object and VTABLE pointers
    m_vtable = nullptr;
    m_object = nullptr;
}

bool any::has_value() const noexcept
{
    return (m_vtable != nullptr);
}

#if defined(RTTI_ENABLED)

const std::type_info& any::type() const noexcept
{
    // Checking if no value is stored in the any object
    if(m_vtable == nullptr) return typeid(void);

    // Returning the type information
    return *m_vtable->m_typeInfo;
}

any::VTable::VTable(deleteFunction fpDelete, cloneFunction fpClone, printFunction fpPrint, std::size_t size, typeInfo* typeInfo)
    : m_deleteFunction(fpDelete), m_cloneFunction(fpClone), m_printFunction(fpPrint), m_size(size), m_typeInfo(typeInfo)
{}

#else

any::VTable::VTable(deleteFunction fpDelete, cloneFunction fpClone, printFunction fpPrint, std::size_t size)
    : m_deleteFunction(fpDelete), m_cloneFunction(fpClone), m_printFunction(fpPrint), m_size(size)
{}

#endif

std::ostream& operator<<(std::ostream& stream, const any& node)
{
    // Checking if the any object stores anything
    if(node.m_vtable == nullptr) {
        stream << "null";
        return stream;
    }

    // Delegating the printing to the type specific printer
    return node.m_vtable->m_printFunction(stream, &node);
}


