#include <mvvm/PropertyList.h>

namespace mvvm {

PropertyList::Iterator PropertyList::begin() const {
    return { this };
}

PropertyList::Iterator PropertyList::end() const {
    return { nullptr };
}

// iterator

PropertyList::Iterator::Iterator(const PropertyList * pProperties) : _numLists(0), _propertyIndex(0) {
    while (pProperties != nullptr) {
        if (pProperties->_size != 0) {
            _lists[_numLists] = pProperties;
            ++_numLists;
        }
        pProperties = pProperties->_pParent;
    }
}

PropertyList::Iterator & PropertyList::Iterator::operator++() {
    if (_propertyIndex < _lists[_numLists - 1]->_size - 1)
        ++_propertyIndex;
    else {
        --_numLists;
        _propertyIndex = 0;
    }
    return *this;
}

const PropertyList::Entry & PropertyList::Iterator::operator*() const {
    return _lists[_numLists - 1]->_properties[_propertyIndex];
}

const PropertyList::Entry * PropertyList::Iterator::operator->() const {
    return _lists[_numLists - 1]->_properties + _propertyIndex;
}

bool PropertyList::Iterator::operator==(const Iterator & other) const {
    return _propertyIndex == other._propertyIndex
        && _numLists == other._numLists;
}

bool PropertyList::Iterator::operator!=(const Iterator & other) const {
    return !(*this == other);
}

}
