//
// Created by Michal_Marszalek on 12.10.2020.
//

#ifndef RTS_GAME_NODEITERATOR_H
#define RTS_GAME_NODEITERATOR_H

#include <iterator>

template<typename T, typename Y>
class NodeIterator {
public:

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = typename Y::difference_type;
    using pointer = T *;
    using reference = T &;

public:

    template<typename X>
    NodeIterator(const X &p) : m_ptr(p) {};

    NodeIterator(const NodeIterator &) = default;

    NodeIterator &operator=(const NodeIterator &) = default;

    ~NodeIterator() {
        static_assert(sizeof(value_type) == sizeof(typename Y::value_type), "Wrong sizes");
        static_assert(std::is_same_v<std::remove_cv_t<typename T::first_type>,
                typename Y::value_type::first_type>, "Wrong type");
        static_assert(std::is_same_v<std::remove_cv_t<typename T::second_type>,
                typename Y::value_type::second_type>, "Wrong type");
    }

    bool operator!=(const NodeIterator &rhs) {
        return m_ptr != rhs.m_ptr;
    }

    bool operator==(const NodeIterator &rhs) {
        return m_ptr == rhs.m_ptr;
    }

    T &operator*() const { return *(T *) &m_ptr.operator*(); }

    T *operator->() const { return (T *) m_ptr.operator->(); }

    NodeIterator &operator++() {
        ++m_ptr;
        return *this;
    }

    NodeIterator operator++(int) {
        auto i = *this;
        ++m_ptr;
        return i;
    }

    NodeIterator &operator--() {
        --m_ptr;
        return *this;
    }

    NodeIterator operator--(int) {
        auto i = *this;
        --m_ptr;
        return i;
    }

protected:

    Y m_ptr;
};


#endif //RTS_GAME_NODEITERATOR_H
