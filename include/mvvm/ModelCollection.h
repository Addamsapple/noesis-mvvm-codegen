#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

#include <mvvm/BaseModelCollection.h>
#include <mvvm/BaseModelPtrCollection.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/ValueTypeOf.h>

namespace mvvm {

class Model;

template<typename T>
class ModelCollection;

template<typename T>
class ModelPtrCollection;

namespace detail {

template<typename T, typename = void>
struct ModelCollectionParent {
    using Type = BaseModelCollection;
};

template<typename T>
struct ModelCollectionParent<SharedPtr<T>, std::enable_if_t<std::is_base_of_v<Model, T>>> {
    using Type = ModelPtrCollection<T>;
};

template<typename T>
using ModelCollectionParentT = typename ModelCollectionParent<T>::Type;

}

// TODO: similar interface needed to traverse nested collections
template<typename T>
class ModelPtrCollection : public BaseModelPtrCollection {
public:
    void InsertModel(SharedPtr<Model> pValue, uint32_t index) override {
        assert(!pValue || pValue.DynamicCast<T>());
        static_cast<ModelCollection<SharedPtr<T>> *>(this)
            ->Insert(pValue.StaticCast<T>(), index);
    }

    void AddModel(SharedPtr<Model> pValue) override {
        assert(!pValue || pValue.DynamicCast<T>());
        static_cast<ModelCollection<SharedPtr<T>> *>(this)
            ->Add(pValue.StaticCast<T>());
    }

    SharedPtr<Model> GetModel(uint32_t index) override {
        return static_cast<ModelCollection<SharedPtr<T>> *>(this)->Get(index);
    }

    void SetModel(SharedPtr<Model> pValue, uint32_t index) override {
        assert(!pValue || pValue.DynamicCast<T>());
        static_cast<ModelCollection<SharedPtr<T>> *>(this)
            ->Set(pValue.StaticCast<T>(), index);
    }
};

template<typename T>
class ModelCollection : public detail::ModelCollectionParentT<T> {
public:
    void Insert(T value, uint32_t index) {
        _items.insert(_items.begin() + index, std::move(value));
        this->_NotifySubscribers(BaseModelCollection::Event::ItemAdded, 0, nullptr, index, &_items[index]);
    }

    void Add(T value) {
        Insert(std::move(value), _items.size());
    }

    void Move(uint32_t oldIndex, uint32_t newIndex) override {
        if (newIndex == oldIndex)
            return;

        if (oldIndex < newIndex)
            std::rotate(_items.begin() + oldIndex, _items.begin() + oldIndex + 1, _items.begin() + newIndex + 1);
        else
            std::rotate(_items.begin() + newIndex, _items.begin() + oldIndex, _items.begin() + oldIndex + 1);

        this->_NotifySubscribers(BaseModelCollection::Event::ItemMoved, oldIndex, nullptr, newIndex, nullptr);
    }

    void Remove(uint32_t index) override {
        auto iter = _items.begin() + index;
        auto oldValue = std::move(*iter);
        _items.erase(iter);
        this->_NotifySubscribers(BaseModelCollection::Event::ItemRemoved, index, &oldValue, 0, nullptr);
    }

    const T & Get(uint32_t index) { return _items[index]; }

    void Set(T value, uint32_t index) {
        auto oldValue = std::move(_items[index]);
        _items[index] = std::move(value);
        this->_NotifySubscribers(BaseModelCollection::Event::ItemReplaced, 0, &oldValue, index, &_items[index]);
    }

    void Clear() override {
        this->_NotifySubscribers(BaseModelCollection::Event::PreCleared, 0, nullptr, 0, nullptr);
        _items.clear();
        this->_NotifySubscribers(BaseModelCollection::Event::Cleared, 0, nullptr, 0, nullptr);
    }

    uint32_t Size() const override { return _items.size(); }

    ValueType Type() const override { return ValueTypeOfV<T>; }

    typename std::vector<T>::const_iterator begin() const { return _items.begin(); }
    typename std::vector<T>::const_iterator end() const { return _items.end(); }

private:
    std::vector<T> _items;
};

}
