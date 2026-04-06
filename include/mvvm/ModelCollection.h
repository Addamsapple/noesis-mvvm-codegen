#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include <mvvm/BaseModelCollection.h>

namespace mvvm {

template<typename T>
class ModelCollection : public BaseModelCollection {
public:
    void Insert(T value, uint32_t index) {
        _items.insert(_items.begin() + index, std::move(value));
        NotifySubscribers(Event::ItemAdded, 0, nullptr, index, &_items[index]);
    }

    void Add(T value) {
        Insert(std::move(value), _items.size());
    }

    void Move(uint32_t oldIndex, uint32_t newIndex) {
        if (newIndex == oldIndex)
            return;

        if (oldIndex < newIndex)
            std::rotate(_items.begin() + oldIndex, _items.begin() + oldIndex + 1, _items.begin() + newIndex + 1);
        else
            std::rotate(_items.begin() + newIndex, _items.begin() + oldIndex, _items.begin() + oldIndex + 1);

        NotifySubscribers(Event::ItemMoved, oldIndex, nullptr, newIndex, nullptr);
    }

    void Remove(uint32_t index) {
        auto iter = _items.begin() + index;
        auto oldValue = std::move(*iter);
        _items.erase(iter);
        NotifySubscribers(Event::ItemRemoved, index, &oldValue, 0, nullptr);
    }

    const T & Get(uint32_t index) { return _items[index]; }

    void Set(T value, uint32_t index) {
        auto oldValue = std::move(_items[index]);
        _items[index] = std::move(value);
        NotifySubscribers(Event::ItemReplaced, 0, &oldValue, index, &_items[index]);
    }

    void Clear() {
        NotifySubscribers(Event::PreCleared, 0, nullptr, 0, nullptr);
        _items.clear();
        NotifySubscribers(Event::Cleared, 0, nullptr, 0, nullptr);
    }

    static T * const Cast(void * const pValue) { return static_cast<T * const>(pValue); }

    uint32_t Size() const { return _items.size(); }

    typename std::vector<T>::const_iterator begin() { return _items.begin(); }
    typename std::vector<T>::const_iterator end() { return _items.end(); }

private:
    std::vector<T> _items;
};

}
