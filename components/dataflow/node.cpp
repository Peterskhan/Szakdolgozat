#include "node.hpp"

Node::Node(Node* parent) noexcept
    : m_name(""), m_parent(parent), m_children(nullptr), m_next(nullptr)
{
    // Nothing to do here...
}

Node::Node(const std::string& name, Node* parent)
    : m_name(name), m_parent(parent), m_children(nullptr), m_next(nullptr)
{
    // Nothing to do here...
}

Node::Node(const Node& other, Node* parent)
    : any(static_cast<const any&>(other)),
      m_name(other.m_name), m_parent(parent), m_children(nullptr), m_next(nullptr)
{
#if defined(EXCEPTIONS_ENABLED)

    try {

        // Recursively copying child nodes
        if(other.m_children) {
            m_children = new Node(*other.m_children, this);
        }

        // Recursively copying sibling nodes
        if(other.m_next) {
            m_next = new Node(*other.m_next, parent);
        }
    }
    catch(std::bad_alloc&)
    {
        // Deleting all allocated resources
        delete m_children;
        delete m_next;

        // Delegating the exception up to the caller
        throw;
    }

#else

    // Recursively copying child nodes
    if(other.m_children) {
        m_children = new Node(*other.m_children, this);
    }

    // Recursively copying sibling nodes
    if(other.m_next) {
        m_next = new Node(*other.m_next, parent);
    }

#endif
}

Node& Node::operator=(const Node& other)
{
    // Clearing the currently stored value and deleting childs
    this->clear();

    // Copying the name of the other Node
    m_name = other.m_name;

    // Making a copy from the other Node's value
    any::operator=(static_cast<const any&>(other));

    // Making a copy from the other Node's children
    if(other.m_children) m_children = new Node(*other.m_children, this);

    return *this;
}

Node::~Node()
{
    // Recursively deleting dependent Nodes
    delete m_children;
    delete m_next;
}

Node& Node::add(const std::string& name)
{
    // Pointer to hold onto the new node
    Node* newNode = nullptr;

    // Creating new child node
    newNode = new Node(name, this);

    // Iterating to the end of the child elements list
    Node* lastNode = m_children;
    while(m_children != nullptr && lastNode->m_next != nullptr) {
        lastNode = lastNode->m_next;
    }

    // Adding new node to the end of the child element list
    if(lastNode != nullptr) lastNode->m_next = newNode;
    else                    m_children = newNode;

    return *newNode;
}

Node& Node::operator[](const std::string& name)
{
    // Pointer to the current child node being examined
    Node* node = m_children;

    // Iterating through the children of this node
    while(node != nullptr) {

        // Checking if the name of the current child node matches with the search
        if(node->m_name == name) return *node;

        // Moving on to the next child node
        node = node->m_next;
    }

    return add(name);
}

const Node& Node::operator[](const std::string& name) const
{
    // Pointer to the current child node being examined
    Node* node = m_children;

    // Iterating through the children of this node
    while(node != nullptr) {

        // Checking if the name of the current child node matches with the search
        if(node->m_name == name) return *node;

        // Moving on to the next child node
        node = node->m_next;
    }

#if defined(EXCEPTIONS_ENABLED)

    // Have not found the searched node, throw an exception
    throw std::out_of_range("Node not found.");

#else

    return *node;

#endif
}

Node& Node::operator[](std::size_t index)
{
    // Checking if the first child Node exists, creating it if it doesn't
    if(m_children == nullptr) m_children = new Node(this);

    // Pointer to the current child Node
    Node* node = m_children;

    // Iterating the list of child Nodes
    for(std::size_t i = 0; i < index; i++)
    {
        // Checking if a next Node exists, creating it if it doesn't
        if(node->m_next == nullptr) node->m_next = new Node(this);

        // Moving on, to the next Node
        node = node->m_next;
    }

    // Returning the Node with the specified index
    return *node;
}

const Node& Node::operator[](std::size_t index) const
{
    const Node* node = m_children;
    for(std::size_t i = 0; i < index; i++)
    {
        node = node->m_next;

#if defined(EXCEPTIONS_ENABLED)
        if(node == nullptr) throw std::out_of_range("Node not found at index.");
#endif
    }

    return *node;
}

void Node::clear() noexcept
{
    // Clearing the value from this Node
    reset();

    // Recursively deleting child Nodes
    delete m_children;
    m_children = nullptr;
}

std::size_t Node::child_count() const noexcept
{
    // Initializing child counter and iteration pointer
    std::size_t count = 0;
    Node* node = m_children;

    // Iterating the list of child Nodes
    while(node != nullptr) {
        node = node->m_next;
        count++;
    }

    // Returning the number of child nodes counted
    return count;
}

bool Node::has_child(const std::string& name) const noexcept
{
    // Pointer to the current child node being examined
    Node* node = m_children;

    // Iterating through the children of this node
    while(node != nullptr) {

        // Checking if the name of the current child node matches with the search
        if(node->m_name == name) return true;

        // Moving on to the next child node
        node = node->m_next;
    }

    return false;
}

std::string& Node::name() noexcept
{
    // Returning the reference for the name of this Node (read-write access)
    return m_name;
}

const std::string& Node::name() const noexcept
{
    // Returning the reference for the name of this Node (read-write access)
    return m_name;
}

Node::iterator Node::begin()
{
    // Returning iterator referencing this Node
    return iterator(this);
}

Node::iterator Node::end()
{
    // Returning past-the-end iterator
    return iterator(nullptr);
}

// Node::iterator

Node::iterator::iterator(Node* node) noexcept
    : m_node(node), m_level(0)
{
    // Nothing to do here...
}

Node::iterator::iterator(const iterator& other) noexcept
    : m_node(other.m_node), m_level(other.m_level)
{
    // Nothing to do here...
}

Node::iterator::iterator(iterator&& other) noexcept
    : m_node(other.m_node), m_level(other.m_level)
{
    // Clearing the other iterator
    other.m_node = nullptr;
    other.m_level = 0;
}

Node::iterator& Node::iterator::operator=(const iterator& other) noexcept
{
    // Making a copy from the other iterator
    m_node = other.m_node;
    m_level = other.m_level;

    return *this;
}

Node::iterator& Node::iterator::operator=(iterator&& other) noexcept
{
    // Making a copy from the other iterator
    m_node = other.m_node;
    m_level = other.m_level;

    // Clearing the other iterator
    other.m_node = nullptr;
    other.m_level = 0;

    return *this;
}

Node::iterator& Node::iterator::operator++() noexcept
{
    // The past-the-end iterator does not increment
    if(m_node == nullptr) return *this;

    // Moving on to the first child of the current node if possible
    if(m_node->m_children != nullptr) {
        m_node = m_node->m_children;
        m_level++;
        return *this;
    }

    // Moving on to the next node on the same hierarchy level
    if(m_node->m_next != nullptr) {
        m_node = m_node->m_next;
        return *this;
    }

    // Moving back on the chain of parents until it has a same
    // hierarchy level neighbour
    while(m_node->m_parent != nullptr) {

        // Moving back to the parent of the current Node
        m_node = m_node->m_parent;
        m_level--;

        // If found an unvisited neighbour, move to that branch
        if(m_node->m_next != nullptr) {
            m_node = m_node->m_next;
            return *this;
        }
    }

    // If we get here, we are back at the root and finished the traversal
    m_node = nullptr;
    return *this;
}

Node::iterator Node::iterator::operator++(int) noexcept
{
    // Making a copy reflecting the current state
    iterator current = *this;

    // Incrementing the iterator to the next Node
    ++(*this);

    // Returning the copy from before the increment
    return current;
}

Node& Node::iterator::operator*() const
{
    // Dereferencing this iterator for the referenced Node
    return *m_node;
}

Node* Node::iterator::operator->() const noexcept
{
    // Returning a pointer for the currently referenced Node
    return m_node;
}

bool Node::iterator::operator==(const iterator& other) const noexcept
{
    // Checking if the iterators reference the same Node
    return (m_node == other.m_node);
}

bool Node::iterator::operator!=(const iterator& other) const noexcept
{
    // Checking if the iterators reference the same Node
    return !(m_node == other.m_node);
}

void Node::iterator::swap(iterator& other) noexcept
{
    // Swapping contents of the iterators
    std::swap(m_node, other.m_node);
    std::swap(m_level, other.m_level);
}

std::size_t Node::iterator::level() const noexcept
{
    // Returning the relative depth of the currently referenced Node
    // from the starting Node of the traversal
    return m_level;
}

// Node::const_iterator

Node::const_iterator::const_iterator(Node* node) noexcept
    : m_node(node), m_level(0)
{
    // Nothing to do here...
}

Node::const_iterator::const_iterator(const const_iterator& other) noexcept
    : m_node(other.m_node), m_level(other.m_level)
{
    // Nothing to do here...
}

Node::const_iterator::const_iterator(const_iterator&& other) noexcept
    : m_node(other.m_node), m_level(other.m_level)
{
    // Clearing the other iterator
    other.m_node = nullptr;
    other.m_level = 0;
}

Node::const_iterator& Node::const_iterator::operator=(const const_iterator& other) noexcept
{
    // Making a copy from the other iterator
    m_node = other.m_node;
    m_level = other.m_level;

    return *this;
}

Node::const_iterator& Node::const_iterator::operator=(const_iterator&& other) noexcept
{
    // Making a copy from the other iterator
    m_node = other.m_node;
    m_level = other.m_level;

    // Clearing the other iterator
    other.m_node = nullptr;
    other.m_level = 0;

    return *this;
}

Node::const_iterator& Node::const_iterator::operator++() noexcept
{
    // The past-the-end iterator does not increment
    if(m_node == nullptr) return *this;

    // Moving on to the first child of the current node if possible
    if(m_node->m_children != nullptr) {
        m_node = m_node->m_children;
        m_level++;
        return *this;
    }

    // Moving on to the next node on the same hierarchy level
    if(m_node->m_next != nullptr) {
        m_node = m_node->m_next;
        return *this;
    }

    // Moving back on the chain of parents until it has a same
    // hierarchy level neighbour
    while(m_node->m_parent != nullptr) {

        // Moving back to the parent of the current Node
        m_node = m_node->m_parent;
        m_level--;

        // If found an unvisited neighbour, move to that branch
        if(m_node->m_next != nullptr) {
            m_node = m_node->m_next;
            return *this;
        }
    }

    // If we get here, we are back at the root and finished the traversal
    m_node = nullptr;
    return *this;
}

Node::const_iterator Node::const_iterator::operator++(int) noexcept
{
    // Making a copy reflecting the current state
    const_iterator current = *this;

    // Incrementing the iterator to the next Node
    ++(*this);

    // Returning the copy from before the increment
    return current;
}

const Node& Node::const_iterator::operator*() const
{
    // Dereferencing this iterator for the referenced Node
    return *m_node;
}

const Node* Node::const_iterator::operator->() const noexcept
{
    // Returning a pointer for the currently referenced Node
    return m_node;
}

bool Node::const_iterator::operator==(const const_iterator& other) const noexcept
{
    // Checking if the iterators reference the same Node
    return (m_node == other.m_node);
}

bool Node::const_iterator::operator!=(const const_iterator& other) const noexcept
{
    // Checking if the iterators reference the same Node
    return !(m_node == other.m_node);
}

void Node::const_iterator::swap(const_iterator& other) noexcept
{
    // Swapping contents of the iterators
    std::swap(m_node, other.m_node);
    std::swap(m_level, other.m_level);
}

std::size_t Node::const_iterator::level() const noexcept
{
    // Returning the relative depth of the currently referenced Node
    // from the starting Node of the traversal
    return m_level;
}
