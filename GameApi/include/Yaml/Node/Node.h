//
// Created by Michal_Marszalek on 08.10.2020.
//

#ifndef RTS_GAME_NODE_H
#define RTS_GAME_NODE_H


#include "NodeType.h"
#include "EmitterStyle.h"
#include <string>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include "NodeIterator.h"

class Emitter;

class Node {
    using Map = std::list<std::pair<Node, Node>>;
    using ItMap = std::list<std::pair<const Node, Node>>;
public:
    using iterator = NodeIterator<std::pair<const Node, Node>, Map::iterator>;
    using const_iterator = NodeIterator<const std::pair<const Node, Node>, Map::const_iterator>;

    Node();

    ~Node();

    Node(const Node &);

    template<typename T>
    explicit Node(const T &);

//region Is
    NodeType Type() const;

    bool IsNull() const { return Type() == NodeType::Null; }

    bool IsScalar() const { return Type() == NodeType::Scalar; }

    bool IsSequence() const { return Type() == NodeType::Sequence; }

    bool IsMap() const { return Type() == NodeType::Map; }
//endregion

    std::string Scalar() const;

    template<typename T>
    T as() const;

    template<typename T, typename Y>
    T as(const Y &fallback) const;

    template<typename T>
    Node &operator=(const T &rhs);

    Node &operator=(const Node &rhs);

    /// Clears data
    void clear();

    /// Clears this node from aliases
    void reset(const Node &rhs = Node());

    // size/iterator
    std::size_t size() const;

    const_iterator begin() const;

    iterator begin();

    const_iterator end() const;

    iterator end();

    // sequence
    template<typename T>
    void push_back(const T &rhs);

    void push_back(const Node &rhs);

    // indexing

    template<typename Key>
    Node &operator[](const Key &key);

    template<typename Key>
    const Node &operator[](const Key &key) const;

    template<typename Key>
    bool remove(const Key &key);

    Node &operator[](const Node &key);

    const Node &operator[](const Node &key) const;

    bool remove(const Node &key);

    template<typename Key, typename Value>
    void force_insert(const Key &key, const Value &value);

    EmitterStyle &style();

    bool operator==(const Node &) const;

    template<typename Key>
    bool operator==(const Key &) const;

    template<typename Key>
    bool operator!=(const Key &) const;

private:
    struct internal_data {
        EmitterStyle style{EmitterStyle::Default};
        NodeType type{NodeType::Null};

        std::string scalar;

        Map map;
    };

    std::shared_ptr<internal_data> data;

    template<typename Key>
    bool equal(const Key &) const;

    friend class Emitter;
};


#include <Yaml/Node/Node.inl>

#endif //RTS_GAME_NODE_H

#include <Yaml/Node/convert.h>
