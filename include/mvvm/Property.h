#pragma once

#include <cassert>
#include <functional>
#include <string>

#include <mvvm/BaseProperty.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/ValueTypeOf.h>

namespace mvvm {

class BaseModelCollection;
class Model;

namespace detail {

template<typename T>
struct ValueHandle {
    using Type = T;
};

template<>
struct ValueHandle<std::string> {
    using Type = const std::string &;
};

template<typename T>
struct ValueHandle<SharedPtr<T>> {
    using Type = const SharedPtr<T> &;
};

template<typename T, typename U>
using Getter = typename ValueHandle<U>::Type (T::*)() const;

template<typename T, typename U>
using Setter = void (T::*)(typename ValueHandle<U>::Type);

}

template<typename T>
class Property final : public BaseProperty {
public:
    using Subscriber = std::function<void (const T & oldValue, const T & newValue)>;

    Property() : _pGetter(nullptr), _pSetter(nullptr) {}

    template<typename U>
    Property(detail::Getter<U, T> pGetter, detail::Setter<U, T> pSetter = nullptr) :
        _pGetter(static_cast<detail::Getter<Model, T>>(pGetter)),
        _pSetter(static_cast<detail::Setter<Model, T>>(pSetter))
    {
        assert(pGetter);
    }

    // TODO: use ValueHandle helper?
    static const T & Cast(const void * pValue) {
        return *static_cast<const T *>(pValue);
    }

    void Get(const Model & model, void * pValue) const override {
        if constexpr(ValueTypeOfV<T> == ValueType::Model)
            *static_cast<SharedPtr<Model> *>(pValue) = std::invoke(_pGetter, model);
        else if constexpr(ValueTypeOfV<T> == ValueType::Collection)
            *static_cast<SharedPtr<BaseModelCollection> *>(pValue) = std::invoke(_pGetter, model);
        else
            *static_cast<T *>(pValue) = std::invoke(_pGetter, model); // TODO: strings are copied
    }

    void Set(Model & model, const void * pValue) const override {
        if constexpr(ValueTypeOfV<T> == ValueType::Model) {
            auto pCastValue = static_cast<const SharedPtr<Model> *>(pValue)
                ->DynamicCast<typename T::Type>();
            std::invoke(_pSetter, model, pCastValue);
        } else if constexpr(ValueTypeOfV<T> == ValueType::Collection) {
            auto pCastValue = static_cast<const SharedPtr<BaseModelCollection> *>(pValue)
                ->DynamicCast<typename T::Type>();
            std::invoke(_pSetter, model, pCastValue);
        } else
            std::invoke(_pSetter, model, *static_cast<const T *>(pValue));
    }

    ValueType Type() const override { return ValueTypeOfV<T>; }

private:
    const detail::Getter<Model, T> _pGetter;
    const detail::Setter<Model, T> _pSetter;
};

}
