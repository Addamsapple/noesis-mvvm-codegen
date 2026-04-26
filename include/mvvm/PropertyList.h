#pragma once

#include <cstddef>
#include <iterator>

#ifndef MVVM_MAX_INHERITANCE_DEPTH
    #define MVVM_MAX_INHERITANCE_DEPTH 16
#endif

namespace mvvm {

class BaseProperty;

class PropertyList {
public:
    struct Entry {
        const char * name;
        const BaseProperty * pProperty;
    };

    class Iterator;

    PropertyList() : PropertyList(nullptr, 0, nullptr) {}

    PropertyList(const Entry * properties, size_t size, const PropertyList * pParent) :
        _properties(properties),
        _size(size),
        _pParent(pParent) {}

    Iterator begin() const;
    Iterator end() const;

private:
    const Entry * const _properties;
    const size_t _size;
    const PropertyList * const _pParent;
};

// iterator

class PropertyList::Iterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = PropertyList::Entry;
    using pointer = const PropertyList::Entry *;
    using reference = const PropertyList::Entry &;
    using iterator_category = std::forward_iterator_tag;

    Iterator(const PropertyList * pProperties);

    Iterator & operator++();

    const PropertyList::Entry & operator*() const;
    const PropertyList::Entry * operator->() const;

    bool operator==(const Iterator & other) const;
    bool operator!=(const Iterator & other) const;

private:
    const PropertyList * _lists[MVVM_MAX_INHERITANCE_DEPTH];
    size_t _numLists;
    size_t _propertyIndex;
};

}

#undef MVVM_MAX_INHERITANCE_DEPTH

