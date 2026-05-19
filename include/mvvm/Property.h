#pragma once

#include <cassert>
#include <functional>
#include <string>

#include <mvvm/BaseProperty.h>
#include <mvvm/SharedPtr.h>
#include <mvvm/Value.h>
#include <mvvm/ValueTraits.h>
#include <mvvm/ValueTypeOf.h>

namespace mvvm {

class BaseModelCollection;
class Model;

namespace detail {

template<typename T, typename U>
using Getter = typename ValueHandle<U>::Type (T::*)() const;

template<typename T, typename U>
using Setter = void (T::*)(typename ValueHandle<U>::Type);

// for tag dispatch
struct Null {};

}

template<typename T>
class Property final : public BaseProperty {
public:
    using Subscriber = std::function<void (const T & oldValue, const T & newValue)>;

    Property(detail::Null) : _pGetter(nullptr), _pSetter(nullptr) {}

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

    Value Get(const Model & model) const override {
        if constexpr(ValueTypeOfV<T> == ValueType::Model)
            return std::invoke(_pGetter, model)
                .template StaticCast<mvvm::Model>();
        else if constexpr(ValueTypeOfV<T> == ValueType::Collection)
            return std::invoke(_pGetter, model)
                .template StaticCast<mvvm::BaseModelCollection>();
        else
            return std::invoke(_pGetter, model); // TODO: strings are copied
    }

    void Set(Model & model, const Value & value) const override {
        assert(value.Type() == ValueTypeOfV<T>);

        if constexpr(ValueTypeOfV<T> == ValueType::Model)
            std::invoke(_pSetter, model, value.As<SharedPtr<Model>>()
                .DynamicCast<typename T::Type>());
        else if constexpr(ValueTypeOfV<T> == ValueType::Collection)
            std::invoke(_pSetter, model, value.As<SharedPtr<BaseModelCollection>>()
                .DynamicCast<typename T::Type>());
        else
            std::invoke(_pSetter, model, value.As<T>()); // TODO: strings are copied
    }

    ValueType Type() const override { return ValueTypeOfV<T>; }

private:
    const detail::Getter<Model, T> _pGetter;
    const detail::Setter<Model, T> _pSetter;
};

}
