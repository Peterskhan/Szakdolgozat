#pragma once
#ifndef DATAFLOW_NODE_HPP_INCLUDED
#define DATAFLOW_NODE_HPP_INCLUDED

// Standard includes
#include <utility>
#include <stdexcept>
#include <iterator>

// Project includes
#include "any.hpp"


/**
 * @brief The Node class implements a tree-hierarchy of any objects.
 */
class Node : public any {
public:

    // Forward declaration for the iterator classes
    class iterator;
    class const_iterator;

    /**
     * @brief Constructs a Node with no value and the specified parent Node.
     * @param parent [in] Pointer to the parent Node (NOT linked from the parent side)!
     */
    Node(Node* parent = nullptr) noexcept;

    /**
     * @brief Constructs an empty Node with the specified name and parent.
     * @param name   [in] The name of the Node to create.
     * @param parent [in] Pointer to the parent Node (NOT linked from the parent side)!
     */
    Node(const std::string& name, Node* parent = nullptr);

    /**
     * @brief Constructs a Node containing a value, with the specified name and parent.
     * @param name   [in] The name of the Node to create.
     * @param value  [in] The value to store in the created Node.
     * @param parent [in] Pointer to the parent Node (NOT linked from the parent side)!
     */
    template <class Type>
    Node(const std::string& name, Type&& value, Node* parent = nullptr)
        : any(value), m_name(name), m_parent(parent), m_children(nullptr), m_next(nullptr)
    {
        // Nothing to do here...
    }

    /**
     * @brief Constructs a Node by copying another Node.
     * @param other  [in] The other Node to copy.
     * @param parent [in] Pointer to the Node adopting the created Node.
     */
    Node(const Node& other, Node* parent = nullptr);

    /**
     * @brief  Copy assigns this Node to another Node.
     * @param  other [in] The other Node to copy.
     * @return Reference to this Node after the assignment.
     */
    Node& operator=(const Node& other);

    /**
     * @brief  Assigns a new value to be stored in the Node.
     * @param  value [in] The value to store in the Node.
     * @return Reference to this Node for chaining assignments.
     */
    template <class Type>
    auto operator=(Type&& value) -> typename std::enable_if<!std::is_same<typename std::decay<Type>::type, Node>::value, Node&>::type
    {
    	any::operator=(value);
        return *this;
    }

    /**
     * @brief Recursively destroys this Node and the dependent Nodes.
     */
    ~Node();

    /**
     * @brief Adds a named child Node to this Node with the specified value.
     * @param name  [in] The name of the child Node to add.
     * @param value [in] The value to be stored in the child.
     */
    template <class Type>
    Node& add(const std::string& name, Type&& value)
    {
        // Pointer to hold onto the new node
        Node* newNode = nullptr;

        // Creating new child node
        newNode = new Node(name, value, this);

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

    /**
     * @brief Adds an empty named child Node to this Node.
     * @param name [in] The name of the child Node to add.
     */
    Node& add(const std::string& name);

    /**
     * @brief Adds an anonymous child Node storing the specified value.
     * @param value [in] The value to be stored in the child.
     */
    template <class Type>
    Node& add(Type&& value)
    {
        // Pointer to hold onto the new node
        Node* newNode = nullptr;

        // Creating new child node
        newNode = new Node("", value, this);

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

    /**
     * @brief  Queries the child Node with the specified name, dynamically
     *         creating it if it does not exist.
     * @param  name [in] The name of the child Node to query or create.
     * @return Reference to the child Node.
     */
    Node& operator[](const std::string& name);

    /**
     * @brief  Queries the child Node with the specified name, throws
     *         std::out_of_rage if it does not exist.
     * @param  name [in] The name of the child Node to query.
     * @return Constant reference to the child Node (read-only access).
     */
    const Node& operator[](const std::string& name) const;

    /**
     * @brief  Queries the child Node with the specified index, dynamically
     *         creating it and lower indices if they do not exist.
     * @param  index [in] The index of the child Node (starting from zero).
     * @return Reference to the child Node.
     */
    Node& operator[](std::size_t index);

    /**
     * @brief  Queries the child Node with the specified index, throws
     *         std::out_of_range if it does not exist.
     * @param  index [in] The index of the child Node (starting from zero).
     * @return Constant reference to the child Node (read-only access).
     */
    const Node& operator[](std::size_t index) const;

    /**
     * @brief Deletes all child Nodes and clears the value from this Node.
     */
    void clear() noexcept;

    /**
     * @brief  Queries the number of childs this Node has.
     * @return The number of child Nodes attached to this Node.
     */
    std::size_t child_count() const noexcept;

    /**
	 * Queries whether the Node has a child node with the specified name.
	 * @param  name [in] The name of the child node to search for.
	 * @return Truw when the Node has a direct child with the specified name.
	 */
	bool has_child(const std::string& name) const noexcept;

    /**
     * @brief  Queries the name of this Node.
     * @return Reference to the name property (read-write access).
     */
    std::string& name() noexcept;

    /**
     * @brief  Queries the name of this Node.
     * @return Constant reference to the name property.
     */
    const std::string& name() const noexcept;

    /**
     * @brief  Creates an iterator referencing this Node.
     * @return The iterator referencing this Node.
     */
    iterator begin();

    /**
     * @brief  Creates a read-only iterator referencing this Node.
     * @return The read-only iterator referencing this Node.
     */
    const_iterator cbegin();

    /**
     * @brief  Creates a past-the-end iterator.
     * @return The past-the-end iterator for terminating traversal.
     */
    iterator end();

    /**
     * @brief  Creates a constant past-the-end iterator.
     * @return The past-the-end iterator for terminating traversal.
     */
    const_iterator cend();

private:
    std::string m_name;     /**< The name of this Node for identification. */
    Node*       m_parent;   /**< Pointer to the parent Node of this Node.  */
    Node*       m_children; /**< Pointer to the list of child Nodes.       */
    Node*       m_next;     /**< Pointer to the next sibling Node.         */
};

/**
 * @brief The Node::iterator class implements a forward-iterator for
 *        iterating the hierarchy of Nodes. This iterator traverses
 *        the tree of Nodes in a depth-first fashion.
 */
class Node::iterator : std::iterator<std::forward_iterator_tag, Node> {
public:

    /**
     * @brief Constructs an iterator referencing a Node.
     * @param node [in] Pointer to the Node the iterator references.
     */
    iterator(Node* node = nullptr) noexcept;

    /**
     * @brief Constructs an iterator by copying another iterator.
     * @param other [in] The other iterator to copy.
     */
    iterator(const iterator& other) noexcept;

    /**
     * @brief Constructs an iterator by moving from another iterator.
     * @param other [in] The other iterator to move from.
     */
    iterator(iterator&& other) noexcept;

    /**
     * @brief  Copy assigns this iterator from another iterator.
     * @param  other [in] The other iterator to copy.
     * @return Reference to this iterator after the copy.
     */
    iterator& operator=(const iterator& other) noexcept;

    /**
     * @brief  Move assigns this iterator from another iterator.
     * @param  other [in] The other iterator to move from.
     * @return Reference to this iterator after the move.
     */
    iterator& operator=(iterator&& other) noexcept;

    /**
     * @brief  Pre-increments this iterator to reference the next Node.
     * @return Reference to this iterator after the increment.
     */
    iterator& operator++() noexcept;

    /**
     * @brief  Post-increments this iterator to reference the next Node.
     * @return A copy of this iterator prior to the increment.
     */
    iterator operator++(int) noexcept;

    /**
     * @brief  Dereferences this iterator.
     * @return Reference to the Node referenced by this iterator.
     */
    Node& operator*() const;

    /**
     * @brief  Dereferences this iterator.
     * @return Pointer to the Node referenced by this iterator.
     */
    Node* operator->() const noexcept;

    /**
     * @brief  Equality-compares this iterator to another iterator.
     * @param  other [in] The other iterator to compare to.
     * @return True when the iterators reference the same Node.
     */
    bool operator==(const iterator& other) const noexcept;

    /**
     * @brief  Equality-compares this iterator to another iterator.
     * @param  other [in] The other iterator to compare to.
     * @return True when the iterators do not reference the same Node.
     */
    bool operator!=(const iterator& other) const noexcept;

    /**
     * @brief  Swaps the content of this iterator with the other iterator.
     * @param  other [in] The other iterator to swap contents with.
     */
    void swap(iterator& other) noexcept;

    /**
     * @brief  Queries the current depth of the referenced Node relative to
     *         the starting Node of creation for this iterator.
     * @return The relative depth of the referenced Node.
     */
    std::size_t level() const noexcept;

private:
    Node*       m_node;  /**< Pointer to the Node referenced by this iterator. */
    std::size_t m_level; /**< The relative depth from the starting Node.       */
};

/**
 * @brief The Node::const_iterator class implements a forward-iterator for
 *        iterating the hierarchy of Nodes. This read-only iterator traverses
 *        the tree of Nodes in a depth-first fashion.
 */
class Node::const_iterator : std::iterator<std::forward_iterator_tag, Node> {
public:

    /**
     * @brief Constructs an iterator referencing a Node.
     * @param node [in] Pointer to the Node the iterator references.
     */
    const_iterator(Node* node = nullptr) noexcept;

    /**
     * @brief Constructs an iterator by copying another iterator.
     * @param other [in] The other iterator to copy.
     */
    const_iterator(const const_iterator& other) noexcept;

    /**
     * @brief Constructs an iterator by moving from another iterator.
     * @param other [in] The other iterator to move from.
     */
    const_iterator(const_iterator&& other) noexcept;

    /**
     * @brief  Copy assigns this iterator from another iterator.
     * @param  other [in] The other iterator to copy.
     * @return Reference to this iterator after the copy.
     */
    const_iterator& operator=(const const_iterator& other) noexcept;

    /**
     * @brief  Move assigns this iterator from another iterator.
     * @param  other [in] The other iterator to move from.
     * @return Reference to this iterator after the move.
     */
    const_iterator& operator=(const_iterator&& other) noexcept;

    /**
     * @brief  Pre-increments this iterator to reference the next Node.
     * @return Reference to this iterator after the increment.
     */
    const_iterator& operator++() noexcept;

    /**
     * @brief  Post-increments this iterator to reference the next Node.
     * @return A copy of this iterator prior to the increment.
     */
    const_iterator operator++(int) noexcept;

    /**
     * @brief  Dereferences this iterator.
     * @return Reference to the Node referenced by this iterator.
     */
    const Node& operator*() const;

    /**
     * @brief  Dereferences this iterator.
     * @return Pointer to the Node referenced by this iterator.
     */
    const Node* operator->() const noexcept;

    /**
     * @brief  Equality-compares this iterator to another iterator.
     * @param  other [in] The other iterator to compare to.
     * @return True when the iterators reference the same Node.
     */
    bool operator==(const const_iterator& other) const noexcept;

    /**
     * @brief  Equality-compares this iterator to another iterator.
     * @param  other [in] The other iterator to compare to.
     * @return True when the iterators do not reference the same Node.
     */
    bool operator!=(const const_iterator& other) const noexcept;

    /**
     * @brief  Swaps the content of this iterator with the other iterator.
     * @param  other [in] The other iterator to swap contents with.
     */
    void swap(const_iterator& other) noexcept;

    /**
     * @brief  Queries the current depth of the referenced Node relative to
     *         the starting Node of creation for this iterator.
     * @return The relative depth of the referenced Node.
     */
    std::size_t level() const noexcept;

private:
    Node*       m_node;  /**< Pointer to the Node referenced by this iterator. */
    std::size_t m_level; /**< The relative depth from the starting Node.       */
};

#endif // DATAFLOW_NODE_HPP_INCLUDED
